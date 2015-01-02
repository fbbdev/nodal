/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Fabio Massaioli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "compiler.h"
#include "node.h"
#include "nodes/output.h"

#include <nodal/passes/cycle_detection.h>
#include <nodal/passes/dead_branch_removal.h>
#include <nodal/passes/topological_sort.h>

#include <boost/graph/graph_utility.hpp>

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace nodal;

class print_graph_pass : public pass
{
public:
  print_graph_pass(std::string const& title) : title(title) {}

  void run(graph& graph, any& data) const override
  {
    if (!title.empty())
      std::cout << title << std::endl;

    boost::print_graph(graph, boost::get(boost::vertex_name, graph));

    std::cout << std::endl;
  };

private:
  std::string title;
};

class type_check_pass : public pass
{
public:
  void run(graph& graph, any& data) const override
  {
    auto nodes = graph.nodes();
    for (auto n = nodes.first; n != nodes.second; ++n)
    {
      if (!dynamic_cast<::node const*>((*n)->node()))
        throw compiler_error("Invalid node found in graph");
    }
  }
};

class compilation_pass : public pass
{
public:
  struct compiled_node
  {
    using link = std::pair<std::size_t, std::size_t>;

    compiled_node(node_fn&& fn,
                  std::size_t input_offset,
                  std::vector<link>&& links = {})
      : fn(std::move(fn)), input_offset(input_offset), links(std::move(links))
      {}

    node_fn fn;
    std::size_t input_offset;
    std::vector<std::pair<std::size_t, std::size_t>> links;
  };

  void run(graph& graph, any& data) const override
  {
    auto& sorted_nodes = data.cast<std::forward_list<graph_node*>>();

    std::size_t input_offset = 0;
    std::size_t graph_output_count = 0;
    std::vector<compiled_node> compiled_nodes;

    std::unordered_map<graph_node*, std::size_t> node_to_index;
    std::size_t i = 0;

    for (auto n: sorted_nodes) {
      compiled_nodes.emplace_back(
        n->node()->cast<::node>()->compile(n->property_data()),
        input_offset);

      node_to_index[n] = i++;

      auto links = graph.input_links(n);
      for (auto link = links.first; link != links.second; ++link) {
        compiled_nodes[node_to_index[link->source_node]].links.emplace_back(
          link->source_socket, link->target_socket + input_offset);
      }

      input_offset += n->node()->input_count();

      if (typeid(n->node()) == typeid(output_node*))
        graph_output_count = std::max(
          graph_output_count, n->property_data()->field<std::size_t>(0));
    }

    ++graph_output_count;

    std::shared_ptr<double> input_data(new double[input_offset],
                                       std::default_delete<double[]>());

    graph_fn result =
      [graph_output_count, input_data, compiled_nodes](double* graph_inputs)
      {
        context context = {
          graph_inputs,
          graph_output_count ? new double[graph_output_count] : nullptr
        };

        for (auto const& c: compiled_nodes)
        {
          auto outputs = c.fn(&input_data.get()[c.input_offset], context);

          for (auto const& l: c.links)
            input_data.get()[l.second] = outputs[l.first];

          delete outputs;
        }

        return context.outputs;
      };

    data = std::move(result);
  }
};

compiler create_compiler()
{
  return {
    new print_graph_pass("Compiler input:"),
    new type_check_pass,
    new cycle_detection_pass([](graph_link const&, graph const&)
    {
      throw compiler_error("Link cycle detected");
    }),
    new dead_branch_removal_pass([](graph_node* n)
    {
      return n->node()->cast<::node>()->keep();
    }),
    new print_graph_pass("Optimized graph:"),
    new topological_sort_pass<std::forward_list<graph_node*>>,
    new compilation_pass
  };
}

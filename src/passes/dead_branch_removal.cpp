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

#include "passes/dead_branch_removal.h"

#include <boost/graph/depth_first_search.hpp>

using namespace nodal;

namespace
{

  class dbr_visitor : public boost::default_dfs_visitor
  {
  public:
    dbr_visitor(std::function<bool(graph_node const* n)> const& keep)
      : keep(keep)
      {}

    void initialize_vertex(graph_node* n, graph const& g) const
    {
      n->attribute("use_count") = g.output_degree(n);
      n->attribute("dead") = false;
    }

    void finish_vertex(graph_node* n, graph const& g) const
    {
      if (keep)
        if (keep(n))
          return;

      if (n->attribute("use_count").cast<std::size_t>() < 1) {
        n->attribute("dead") = true;

        auto links = g.input_links(n);

        for (auto link = links.first; link != links.second; ++link) {
          auto& c = link->source_node->attribute("use_count");
          c = c.cast<std::size_t>() - 1;
        }
      }
    }

  private:
    std::function<bool(graph_node const* n)> const& keep;
  };

} /* namespace */

any dead_branch_removal_pass::run(graph& graph, context& ctx) const
{
  boost::depth_first_search(graph, dbr_visitor(keep),
                            boost::get(boost::vertex_color, graph));

  auto nodes = graph.nodes();
  for (auto node = nodes.first; node != nodes.second;) {
    if ((*node)->attribute("dead"))
      node = graph.remove(node);
    else
      ++node;
  }

  return {};
}

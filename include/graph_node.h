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

#ifndef __NODAL_GRAPH_NODE_H__
#define __NODAL_GRAPH_NODE_H__

#include "node.h"
#include "node_data.h"

#include <boost/any.hpp>

#include <string>
#include <unordered_map>

namespace nodal
{

class graph_node
{
public:
  graph_node(class node const* node);

  graph_node(graph_node const& other);
  graph_node(graph_node&& other);

  ~graph_node();

  graph_node& operator=(graph_node const& other);
  graph_node& operator=(graph_node&& other);

  class node const* node() const { return _node; }

  node_data* input_data() { return inputs; }
  node_data const* input_data() const { return inputs; }

  node_data* property_data() { return properties; }
  node_data const* property_data() const { return properties; }

  boost::any& attribute(std::string const& key)
  {
    return attributes[key];
  }

  boost::any const& attribute(std::string const& key) const
  {
    return attributes.at(key);
  }

  bool has_attribute(std::string const& key) const
  {
    return attributes.count(key);
  }

private:
  class node const* _node;

  node_data* inputs;
  node_data* properties;

  std::unordered_map<std::string, boost::any> attributes;
};

} /* namespace nodal */

#endif /* __NODAL_GRAPH_NODE_H__ */

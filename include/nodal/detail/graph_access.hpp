/** -*- C++ -*-
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

#pragma once

#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>

#include <iterator>

namespace boost
{

template <>
struct graph_traits<nodal::graph> {
    using vertex_descriptor = nodal::graph_node*;
    using edge_descriptor = nodal::graph_link;

    using directed_category = directed_tag;
    using edge_parallel_category = allow_parallel_edge_tag;

    struct traversal_category : vertex_list_graph_tag,
                                edge_list_graph_tag,
                                bidirectional_graph_tag,
                                adjacency_graph_tag {};

    using vertex_iterator = nodal::graph::node_iterator;
    using edge_iterator = nodal::graph::link_iterator;

    using vertices_size_type = std::size_t;
    using edges_size_type = std::size_t;

    using in_edge_iterator = nodal::graph::input_link_iterator;
    using out_edge_iterator = nodal::graph::output_link_iterator;
    using degree_size_type = std::size_t;

    using adjacency_iterator = adjacency_iterator_generator<
        nodal::graph, vertex_descriptor, out_edge_iterator>::type;

    static constexpr nodal::graph_node* null_vertex() {
        return nullptr;
    }
};

inline nodal::graph::node_range vertices(nodal::graph const& g) {
    return g.nodes();
}

inline nodal::graph::link_range edges(nodal::graph const& g) {
    return g.links();
}

inline std::size_t num_vertices(nodal::graph const& g) {
    return g.node_count();
}

inline std::size_t num_edges(nodal::graph const& g) {
    return g.link_count();
}

inline nodal::graph_node* source(nodal::graph_link const& e,
                                 nodal::graph const& g) {
    return e.source_node;
}

inline nodal::graph_node* target(nodal::graph_link const& e,
                                 nodal::graph const& g) {
    return e.target_node;
}

inline nodal::graph::input_link_range in_edges(nodal::graph_node* v,
                                               nodal::graph const& g) {
    return g.input_links(v);
}

inline nodal::graph::output_link_range out_edges(nodal::graph_node* v,
                                                 nodal::graph const& g) {
    return g.output_links(v);
}

inline std::size_t in_degree(nodal::graph_node* v, nodal::graph const& g) {
    return g.input_degree(v);
}

inline std::size_t out_degree(nodal::graph_node* v, nodal::graph const& g) {
    return g.output_degree(v);
}

inline std::size_t degree(nodal::graph_node* v, nodal::graph const& g) {
    return g.degree(v);
}

inline std::pair<graph_traits<nodal::graph>::adjacency_iterator,
                 graph_traits<nodal::graph>::adjacency_iterator>
adjacent_vertices(nodal::graph_node* v, nodal::graph const& g) {
    using iterator = graph_traits<nodal::graph>::adjacency_iterator;
    auto range = out_edges(v, g);

    return { iterator(range.first, &g), iterator(range.second, &g) };
}

inline nodal::graph_node* add_vertex(nodal::graph& g) {
    return g.add((nodal::node const*) nullptr);
}

inline nodal::graph_node* add_vertex(nodal::node const* node, nodal::graph& g) {
    return g.add(node);
}

inline void clear_vertex(nodal::graph_node* v, nodal::graph& g) {
    g.unlink(v);
}

inline void remove_vertex(nodal::graph_node* v, nodal::graph& g) {
    g.remove(v);
}

inline std::pair<nodal::graph_link, bool>
add_edge(nodal::graph_node* u, nodal::graph_node* v, nodal::graph& g) {
    return { nodal::graph_link(u, 0, v, 0), false };
}

inline std::pair<nodal::graph_link, bool>
add_edge(nodal::graph_node* u, nodal::graph_node* v,
         std::pair<std::size_t, std::size_t> sockets,
         nodal::graph& g) {
    nodal::graph_link link(u, sockets.first, v, sockets.second);
    bool inserted = !g.has(link);

    return { g.link(link), inserted };
}

void remove_edge(nodal::graph_node* u, nodal::graph_node* v, nodal::graph& g);

inline void remove_edge(nodal::graph_link const& e, nodal::graph& g) {
    g.unlink(e);
}

inline void remove_edge(nodal::graph::link_iterator e_iter, nodal::graph& g) {
    g.unlink(e_iter);
}

} /* namespace boost */

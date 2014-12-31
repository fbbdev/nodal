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

#ifndef __NODAL_DETAIL_LINK_LIST_H__
#define __NODAL_DETAIL_LINK_LIST_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

namespace nodal
{

namespace detail
{

  struct source_index {};
  struct target_index {};

  namespace link_list_detail
  {
    using namespace boost;
    using namespace boost::multi_index;

    using link_list = multi_index_container<
      graph_link,
      indexed_by<
        hashed_unique<identity<graph_link>, graph_link::hash>,
        ordered_non_unique<
          tag<source_index>,
          member<graph_link, graph_node*, &graph_link::source_node>
        >,
        ordered_non_unique<
          tag<target_index>,
          member<graph_link, graph_node*, &graph_link::target_node>
        >
      >
    >;

  } /* namespace link_list_detail */

  using link_list = link_list_detail::link_list;

} /* namespace detail */

} /* namespace nodal */

#endif /* __NODAL_DETAIL_LINK_LIST_H__ */

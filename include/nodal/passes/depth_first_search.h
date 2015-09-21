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

#include "../compiler.h"

#include <boost/graph/depth_first_search.hpp>

namespace nodal
{

namespace detail
{

  template<typename Visitor>
  struct dfs_visitor
  {
    template<typename T, void (Visitor::*)(context&)> struct check {};

    template<typename T>
    static void do_set_context(T& v, context& ctx, check<T, &T::context>*)
    {
      v.context(ctx);
    }

    template<typename T>
    static void do_set_context(T& v, context& ctx, ...) {}

    static void set_context(Visitor& v, context& ctx)
    {
      do_set_context(v, ctx, nullptr);
    }
  };

}

template<typename Visitor>
class depth_first_search_pass : public pass
{
public:
  depth_first_search_pass(Visitor visitor = Visitor())
    : visitor(std::move(visitor))
    {}

  any run(graph& graph, context& ctx) const override;

private:
  Visitor visitor;
};

template<typename Visitor>
any depth_first_search_pass<Visitor>::run(graph& graph, context& ctx) const
{
  Visitor v = visitor;
  detail::dfs_visitor<Visitor>::set_context(v, ctx);

  boost::depth_first_search(graph, v, boost::get(boost::vertex_color, graph));

  return {};
}

} /* namespace nodal */

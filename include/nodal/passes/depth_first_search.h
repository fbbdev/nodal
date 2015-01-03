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

#ifndef __NODAL_PASSES_DEPTH_FIRST_SEARCH_H__
#define __NODAL_PASSES_DEPTH_FIRST_SEARCH_H__

#include "../compiler.h"

#include <boost/graph/depth_first_search.hpp>

namespace nodal
{

template<typename Visitor>
class depth_first_search_pass : public pass
{
public:
  depth_first_search_pass(Visitor visitor = Visitor()) : visitor(visitor) {}

  void run(graph& graph, any& data) const override;

private:
  Visitor visitor;
};

template<typename Visitor>
void depth_first_search_pass<Visitor>::run(graph& graph, any& data) const
{
  boost::depth_first_search(graph, visitor,
                            boost::get(boost::vertex_color, graph));
}

} /* namespace nodal */

#endif /* __NODAL_PASSES_DEPTH_FIRST_SEARCH_H__ */
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
#include "node_factory.h"

#include <nodal/graph.h>

#include <iostream>

using namespace nodal;

int main()
{
  node_factory nodes;

  // Build and compile graph

  graph g;
  auto compiler = create_compiler();

  auto in0  = g.add(nodes.input("in0", 0));
  auto in1  = g.add(nodes.input("in1", 1));
  auto in2  = g.add(nodes.input("in2", 2));
  auto add  = g.add(nodes.math("add", math_node::add));
  auto sub  = g.add(nodes.math("sub", math_node::sub));
  auto mul  = g.add(nodes.math("mul", math_node::mul));
  auto mul2 = g.add(nodes.math("mul2", math_node::mul));
  auto div  = g.add(nodes.math("div", math_node::div));
  auto div2 = g.add(nodes.math("div2", math_node::div));
  auto rnd  = g.add(nodes.random("rnd"));
  auto out0 = g.add(nodes.output("out0", 0));
  auto out1 = g.add(nodes.output("out1", 1));
  auto out2 = g.add(nodes.output("out2", 2));

  g.link(in0, 0, div, 0);
  g.link(in1, 0, div, 1);

  g.link(div, 0, add, 0);
  g.link(in2, 0, add, 1);

  g.link(add, 0, mul, 0);
  g.link(div, 0, mul, 1);

  g.link(mul, 0, out0, 0);

  g.link(add, 0, sub, 0);
  g.link(mul, 0, sub, 1);

  g.link(sub, 0, out1, 0);

  g.link(div, 0, mul2, 0);
  g.link(rnd, 0, mul2, 1);

  g.link(mul, 0, div2, 0);
  g.link(mul2, 0, div2, 1);

  g.link(div2, 0, out2, 0);

  graph_fn fn = compiler.compile(g);

  // Read input

  double inputs[3] = { 0, 0, 0 };

  while (1) {
    std::cin >> inputs[0] >> inputs[1] >> inputs[2];

    if (!std::cin) break;

    auto outputs = fn(inputs);
    std::cout << outputs[0] << ' '
              << outputs[1] << ' '
              << outputs[2]
              << std::endl;
    delete outputs;
  }

  return 0;
}

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

#include "math.h"

#include <algorithm>
#include <cmath>

using namespace nodal;

node_data* math_node::data() const
{
  return make_node_data<
    data_t,
    data_block<
      data_field<double, offsetof(data_t, first)>,
      data_field<double, offsetof(data_t, second)>
    >,
    data_block<
      data_field<function, offsetof(data_t, fn)>
    >
  >(data_t{ 0.0, 0.0, first });
}

node_fn math_node::compile(node_data* data) const
{
  switch (data->param<function>(0))
  {
    case first:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[0] };
      };

    case second:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[1] };
      };

    case min:
      return [](double* inputs, context const&)
      {
        return new double[1]{ std::min(inputs[0], inputs[1]) };
      };

    case max:
      return [](double* inputs, context const&)
      {
        return new double[1]{ std::max(inputs[0], inputs[1]) };
      };

    case add:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[0] + inputs[1] };
      };

    case sub:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[0] - inputs[1] };
      };

    case mul:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[0] * inputs[1] };
      };

    case div:
      return [](double* inputs, context const&)
      {
        return new double[1]{ inputs[0] / inputs[1] };
      };

    case pow:
      return [](double* inputs, context const&)
      {
        return new double[1]{ std::pow(inputs[0], inputs[1]) };
      };
  }

  return [](double*, context const&) -> double* { return nullptr; };
}

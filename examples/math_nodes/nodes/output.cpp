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

#include "output.h"

using namespace nodal;

node_data* output_node::data() const
{
  return struct_node_data<
    data_t,
    data_block<
      data_field<data_t, double, &data_t::value>
    >,
    data_block<
      data_field<data_t, std::size_t, &data_t::index>
    >
  >(data_t{ 0.0, 0 });
}

node_fn output_node::compile(node_data* data) const
{
  auto index = data->param<std::size_t>(0);

  return [index](double* inputs, context const& context) -> double*
  {
    context.outputs[index] = inputs[0];
    return nullptr;
  };
}

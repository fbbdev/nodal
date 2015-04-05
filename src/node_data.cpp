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

#include "node_data.h"

using namespace nodal;

void* node_data::data_ptr(std::size_t) const
{
  throw std::logic_error("node_data::data_ptr");
}

void* node_data::input_ptr(std::size_t, std::size_t) const
{
  throw std::logic_error("node_data::input_ptr(index)");
}

void* node_data::input_ptr(std::string const&, std::size_t) const
{
  throw std::logic_error("node_data::input_ptr(string)");
}

void* node_data::param_ptr(std::size_t, std::size_t) const
{
  throw std::logic_error("node_data::param_ptr(index)");
}

void* node_data::param_ptr(std::string const&, std::size_t) const
{
  throw std::logic_error("node_data::param_ptr(string)");
}

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

#include <nodal/nodal.h>

#include <iostream>
#include <set>

using namespace nodal;

struct Data
{
    int num;
    float vec[3];

    std::set<Data*> structs;
    std::vector<std::string> names;
};

std::ostream& operator<<(std::ostream& stream, Data const& data) {
    stream <<
        "{\n"
        "  num: " << data.num << ",\n"
        "  vec: [ " << data.vec[0] << ", "
                    << data.vec[1] << ", "
                    << data.vec[2] << " ],\n\n"
        "  structs: [";

    if (data.structs.size()) {
        stream << "\n    " << reinterpret_cast<void*>(*data.structs.begin());

        auto it = data.structs.begin(); ++it;
        for (auto end = data.structs.end(); it != end; ++it)
            stream << ",\n    " << reinterpret_cast<void*>(*it);
    }

    stream <<
        "\n  ],\n"
        "  names: [";

    if (data.names.size()) {
        stream << "\n    \"" << data.names.front() << '"';
        for (auto it = data.names.begin() + 1, end = data.names.end();
             it != end; ++it) {
            stream << ",\n    \"" << *it << '"';
        }
    }

    return stream <<
        "\n  ]\n"
        "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, std::vector<T> const& vector) {
    stream << "[";

    if (vector.size()) {
      stream << "\n  " << vector.front();
      for (auto it = vector.begin() + 1, end = vector.end(); it != end; ++it)
          stream << ",\n  " << *it;
    }

    return stream << "\n]";
  }

std::ostream& operator<<(std::ostream& stream,
                         std::vector<std::string> const& vector) {
    stream << "[";

    if (vector.size()) {
        stream << "\n  \"" << vector.front() << '"';
        for (auto it = vector.begin() + 1, end = vector.end(); it != end; ++it)
            stream << ",\n  \"" << *it << '"';
    }

    return stream << "\n]";
}

int main() {
    auto data = struct_node_data<
        Data,
        data_block<
            data_field<Data, int, &Data::num>,
            data_field<Data, float[3], &Data::vec>>,
        data_block<
            data_field<Data, std::set<Data*>, &Data::structs>,
            data_field<Data, std::vector<std::string>, &Data::names>>>();

    types::integer->from_real(4355.76, data, 0);
    std::cout << '"' << types::integer->as_string(data, 0) << '"' << std::endl;

    generic_type<float[3]> vector_type;

    vector_type.from_int_vector({ 11, 12, 13 }, data, 1);
    std::cout << vector_type.as_real_vector(data, 1) << std::endl;

    vector_type.from_real_vector({ 11.45, 1.5e+20, 13 }, data, 1);
    std::cout << vector_type.as_uint_vector(data, 1) << "\n"
              << vector_type.as_string_vector(data, 1) << std::endl;

    generic_type<std::set<Data*>> ptr_set_type;

    Data d1, d2, d3;
    ptr_set_type.from_object_vector<Data>({ &d1, &d2, &d3 }, data, 0, true);

    generic_type<std::vector<std::string>> string_list_type;

    string_list_type.from_bool_vector({ true, false, false, true }, data, 1, true);
    std::cout << string_list_type.as_bool_vector(data, 1, true) << std::endl;

    std::cout << data->data<Data>() << std::endl;

    delete data;
    return 0;
}

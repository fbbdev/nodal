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

#include "types.h"

namespace nodal
{

    template class generic_type<bool>;

    template class generic_type<std::int8_t>;
    template class generic_type<std::uint8_t>;
    template class generic_type<std::int16_t>;
    template class generic_type<std::uint16_t>;
    template class generic_type<std::int32_t>;
    template class generic_type<std::uint32_t>;
    template class generic_type<std::int64_t>;
    template class generic_type<std::uint64_t>;

    template class generic_type<float>;
    template class generic_type<double>;

    template class generic_type<std::string>;

    template class generic_type<void*>;

    // Singletons

    namespace types
    {

        none_type const none{};

        bool_type const boolean{};

        byte_type const byte{};
        int_type const integer{};
        uint_type const unsigned_int{};

        int16_type const int16{};
        uint16_type const uint16{};
        int32_type const int32{};
        uint32_type const uint32{};
        int64_type const int64{};
        uint64_type const uint64{};

        float_type const real{};
        float_type const& float_single = real;
        double_type const float_double{};

        string_type const string{};

        pointer_type const pointer{};

    } /* namespace types */

} /* namespace nodal */

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

#include "types.hpp"

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

} /* namespace nodal */

// Singletons

namespace
{

    nodal::types::none_type const none_tp{};

    nodal::types::bool_type const boolean_tp{};

    nodal::types::byte_type const byte_tp{};
    nodal::types::int_type const integer_tp{};
    nodal::types::uint_type const unsigned_int_tp{};

    nodal::types::int16_type const int16_tp{};
    nodal::types::uint16_type const uint16_tp{};
    nodal::types::int32_type const int32_tp{};
    nodal::types::uint32_type const uint32_tp{};
    nodal::types::int64_type const int64_tp{};
    nodal::types::uint64_type const uint64_tp{};

    nodal::types::float_type const float_single_tp{};
    nodal::types::double_type const float_double_tp{};

    nodal::types::string_type const string_tp{};

    nodal::types::pointer_type const pointer_tp{};

}  // namespace

namespace nodal { namespace types
{

    none_type const* const none = &none_tp;

    bool_type const* const boolean = &boolean_tp;

    byte_type const* const byte = &byte_tp;
    int_type const* const integer = &integer_tp;
    uint_type const* const unsigned_int = &unsigned_int_tp;

    int16_type const* const int16 = &int16_tp;
    uint16_type const* const uint16 = &uint16_tp;
    int32_type const* const int32 = &int32_tp;
    uint32_type const* const uint32 = &uint32_tp;
    int64_type const* const int64 = &int64_tp;
    uint64_type const* const uint64 = &uint64_tp;

    float_type const* const real = &float_single_tp;
    float_type const* const float_single = &float_single_tp;
    double_type const* const float_double = &float_double_tp;

    string_type const* const string = &string_tp;

    pointer_type const* const pointer = &pointer_tp;

} /* namespace types */ } /* namespace nodal */

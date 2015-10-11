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

#include "../type.h"

#include <boost/lexical_cast.hpp>

#include <type_traits>

namespace nodal
{

namespace detail
{

  template<typename T>
  struct exists
  {
    using type = void;
  };

  template<typename C, typename = void, typename = void, typename = void>
  struct is_container : std::false_type {};

  template<typename C>
  struct is_container<C, typename exists<typename C::value_type>::type,
                      typename exists<decltype(std::declval<C>().begin())>::type,
                      typename exists<decltype(std::declval<C>().begin())>::type>
    : std::true_type {};

  template<>
  struct is_container<std::string, void, void, void> : std::false_type {};

  template<typename T, bool = std::is_enum<T>::value>
  struct underlying_type
  {
    using type = T;
  };

  template<typename T>
  struct underlying_type<T, true>
  {
    using type = typename std::underlying_type<T>::type;
  };

  template<typename T, bool = is_container<T>::value>
  struct generic_type_traits
  {
    using type = typename underlying_type<
      typename std::remove_cv<T>::type>::type;

    static constexpr std::size_t size = sizeof(type);
    static constexpr std::size_t alignment = alignof(type);

    static constexpr bool is_vector = false;
    static constexpr std::size_t vector_size = 0;

    static constexpr bool is_list = false;

    static constexpr bool is_boolean = std::is_same<type, bool>::value;

    static constexpr bool is_byte = !is_boolean &&
                                    std::is_integral<type>::value &&
                                    std::is_unsigned<type>::value &&
                                    sizeof(type) == 1;

    static constexpr bool is_integer = !is_boolean &&
                                       std::is_integral<type>::value &&
                                       std::is_signed<type>::value;

    static constexpr bool is_unsigned_int = !is_boolean && !is_byte &&
                                            std::is_integral<type>::value &&
                                            std::is_unsigned<type>::value;

    static constexpr bool is_real = std::is_floating_point<type>::value;

    static constexpr bool is_string = std::is_same<type, std::string>::value;

    static constexpr bool is_object = !is_string &&
                                      (std::is_pointer<type>::value ||
                                       std::is_union<type>::value ||
                                       std::is_class<type>::value);

    static constexpr bool is_pointer = std::is_pointer<type>::value;

    static constexpr bool is_none = !is_boolean && !is_byte && !is_integer &&
                                    !is_unsigned_int && !is_real &&
                                    !is_string && !is_object;
  };

  template<typename T>
  struct generic_type_traits<T, true>
    : generic_type_traits<typename T::value_type, false>
  {
    using type = T;
    using value_type = typename T::value_type;

    static constexpr std::size_t size = sizeof(type);
    static constexpr std::size_t alignment = alignof(type);

    static constexpr bool is_list = true;
  };

  template<typename T, std::size_t N>
  struct generic_type_traits<T[N], false> : generic_type_traits<T, false>
  {
    using type = typename std::remove_cv<T>::type[N];
    using value_type = T;

    static constexpr std::size_t size = sizeof(type);
    static constexpr std::size_t alignment = alignof(type);

    static constexpr bool is_vector = true;
    static constexpr std::size_t vector_size = N;
  };

  template<typename T, std::size_t N>
  struct generic_type_traits<T[N], true> : generic_type_traits<T[N], false> {};

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_boolean, type::repr_t>::type
  generic_type_repr()
  {
    return type::boolean;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_byte, type::repr_t>::type
  generic_type_repr()
  {
    return type::byte;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_integer, type::repr_t>::type
  generic_type_repr()
  {
    return type::integer;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_unsigned_int, type::repr_t>::type
  generic_type_repr()
  {
    return type::unsigned_int;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_real, type::repr_t>::type
  generic_type_repr()
  {
    return type::real;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_object, type::repr_t>::type
  generic_type_repr()
  {
    return type::object;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_string, type::repr_t>::type
  generic_type_repr()
  {
    return type::string;
  }

  template<typename T>
  inline constexpr typename std::enable_if<
    generic_type_traits<T>::is_none, type::repr_t>::type
  generic_type_repr()
  {
    return type::none;
  }

  template<typename From, typename To>
  struct explicitly_convertible
  {
    template<typename F, typename T>
    static constexpr auto test(int) ->
      decltype(static_cast<T>(std::declval<F>()), true)
    {
        return true;
    }

    template<typename F, typename T>
    static constexpr bool test(...)
    {
        return false;
    }

    static constexpr bool value = test<From, To>(0);
  };

  template<typename To, typename From>
  struct from_or_to_string
  {
    static constexpr bool t = std::is_same<To, std::string>::value;
    static constexpr bool u = std::is_same<From, std::string>::value;
    static constexpr bool np = !std::is_pointer<To>::value;

    static constexpr bool value = np && ((t && !u) || (u && !t));
  };

  template<typename To, typename From>
  struct from_or_to_bool
  {
    static constexpr bool t = std::is_same<To, bool>::value;
    static constexpr bool u = std::is_same<From, bool>::value;

    static constexpr bool value = (t && !u) || (u && !t);
  };

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    explicitly_convertible<Type, Repr>::value &&
    explicitly_convertible<Repr, Type>::value, Repr>::type
  generic_type_cast(Type const& value)
  {
    return static_cast<Repr>(value);
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    !from_or_to_string<Repr, Type>::value &&
    !(explicitly_convertible<Type, Repr>::value &&
      explicitly_convertible<Repr, Type>::value), Repr>::type
  generic_type_cast(Type const& value)
  {
    return Repr{};
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    from_or_to_string<Repr, Type>::value &&
    !from_or_to_bool<Repr, Type>::value, Repr>::type
  generic_type_cast(Type const& value)
  {
    Repr result;

    if (!boost::conversion::try_lexical_convert(value, result))
      return Repr{};

    return result;
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    from_or_to_string<Repr, Type>::value &&
    std::is_same<Type, bool>::value, Repr>::type
  generic_type_cast(Type const& value)
  {
    return value ? "true" : "false";
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    from_or_to_string<Repr, Type>::value &&
    std::is_same<Repr, bool>::value, Repr>::type
  generic_type_cast(Type const& value)
  {
    return value == "true";
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    explicitly_convertible<Type, Repr>::value &&
    explicitly_convertible<Repr, Type>::value>::type
  generic_type_assign(Type& dest, Repr const& value)
  {
    dest = generic_type_cast<Type>(value);
  }

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    !from_or_to_string<Type, Repr>::value &&
    !(explicitly_convertible<Type, Repr>::value &&
      explicitly_convertible<Repr, Type>::value)>::type
  generic_type_assign(Type& dest, Repr const& value) {}

  template<typename Repr, typename Type>
  inline typename std::enable_if<
    from_or_to_string<Type, Repr>::value &&
    !from_or_to_bool<Type, Repr>::value>::type
  generic_type_assign(Type& dest, Repr const& value)
  {
    Type result;

    if (boost::conversion::try_lexical_convert(value, result))
      dest = result;
  }

  void generic_type_assign(std::string& dest, bool const& value)
  {
    dest = value ? "true" : "false";
  }

  void generic_type_assign(bool& dest, std::string const& value)
  {
    if (value == "true")
      dest = true;
    else if (value == "false")
      dest = false;
  }

  template<typename Type>
  inline void* generic_type_as_pointer(Type const& value)
  {
    return const_cast<Type*>(&value);
  }

  template<typename Type>
  inline void* generic_type_as_pointer(Type* const& value)
  {
    return value;
  }

  template<typename Type>
  inline void generic_type_from_pointer(Type& dest, void* pointer)
  {
    dest = *reinterpret_cast<Type*>(pointer);
  }

  template<typename Type>
  inline void generic_type_from_pointer(Type*& dest, void* pointer)
  {
    dest = reinterpret_cast<Type*>(pointer);
  }

} /* namespace detail */

} /* namespace nodal */

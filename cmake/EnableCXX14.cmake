# The MIT License (MIT)
#
# Copyright (c) 2015 Fabio Massaioli
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

function(enable_cxx14 target)
  if(NOT MSVC)
    target_compile_features("${target}" PUBLIC
      cxx_aggregate_default_initializers
      cxx_aggregate_default_initializers
      cxx_alias_templates
      cxx_alignas
      cxx_alignof
      cxx_attributes
      cxx_attribute_deprecated
      cxx_auto_type
      cxx_binary_literals
      cxx_constexpr
      cxx_contextual_conversions
      cxx_decltype_incomplete_return_types
      cxx_decltype
      cxx_decltype_auto
      cxx_default_function_template_args
      cxx_defaulted_functions
      cxx_defaulted_move_initializers
      cxx_delegating_constructors
      cxx_deleted_functions
      cxx_digit_separators
      cxx_enum_forward_declarations
      cxx_explicit_conversions
      cxx_extended_friend_declarations
      cxx_extern_templates
      cxx_final
      cxx_func_identifier
      cxx_generalized_initializers
      cxx_generic_lambdas
      cxx_inheriting_constructors
      cxx_inline_namespaces
      cxx_lambdas
      cxx_lambda_init_captures
      cxx_local_type_template_args
      cxx_long_long_type
      cxx_noexcept
      cxx_nonstatic_member_init
      cxx_nullptr
      cxx_override
      cxx_range_for
      cxx_raw_string_literals
      cxx_reference_qualified_functions
      cxx_relaxed_constexpr
      cxx_return_type_deduction
      cxx_right_angle_brackets
      cxx_rvalue_references
      cxx_sizeof_member
      cxx_static_assert
      cxx_strong_enums
      cxx_thread_local
      cxx_trailing_return_types
      cxx_unicode_literals
      cxx_uniform_initialization
      cxx_unrestricted_unions
      cxx_user_literals
      cxx_variable_templates
      cxx_variadic_macros
      cxx_variadic_templates
      cxx_template_template_parameters)
  endif()
endfunction()

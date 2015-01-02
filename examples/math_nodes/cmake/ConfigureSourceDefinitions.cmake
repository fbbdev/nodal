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

include(CMakeParseArguments)

function(configure_source_definitions template)
  set(options)
  set(oneValueArgs BASE_PATH)
  set(multiValueArgs)

  cmake_parse_arguments(ARG
                        "${options}"
                        "${oneValueArgs}"
                        "${multiValueArgs}"
                        ${ARGN})

  foreach(source IN LISTS ARG_UNPARSED_ARGUMENTS)
    if(ARG_BASE_PATH)
      get_filename_component(base_path "${ARG_BASE_PATH}" ABSOLUTE)
      get_filename_component(parsed_source "${source}" ABSOLUTE)
      file(RELATIVE_PATH parsed_source "${base_path}" "${parsed_source}")
    else()
      set(parsed_source "${source}")
    endif()

    get_filename_component(ns "${parsed_source}" DIRECTORY)
    get_filename_component(name "${parsed_source}" NAME_WE)
    get_filename_component(ext "${parsed_source}" EXT)

    string(REPLACE "/" "_" ns "${ns}")
    string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" name "${name}")
    string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" ext "${ext}")

    if(ns)
      set(ns "${ns}_")
    endif()

    string(TOUPPER "${ns}${name}${ext}" id)
    string(CONFIGURE "${template}" def @ONLY)

    set_source_files_properties("${source}" PROPERTIES
                                COMPILE_DEFINITIONS "${def}")
  endforeach()
endfunction()

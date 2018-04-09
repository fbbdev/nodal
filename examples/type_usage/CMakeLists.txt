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

cmake_minimum_required(VERSION 3.2)
project(nodal-example-type-usage CXX C)

if(NOT BUILDING_NODAL)
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../cmake")

    include(GNUInstallDirs)
    include(ConfigureSourceDefinitions)

    find_package(nodal REQUIRED)
endif()

set(SOURCES
    main.cpp
)

set(HEADERS
)

configure_source_definitions(
    "__NODAL_EXAMPLE_TYPE_USAGE_BUILDING_@id@__"
    ${SOURCES})

add_executable(type-usage ${SOURCES} ${HEADERS})

target_compile_features(type-usage PUBLIC cxx_std_14)
target_link_libraries(type-usage nodal)

set_property(TARGET type-usage
             PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

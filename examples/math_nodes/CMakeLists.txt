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

cmake_minimum_required(VERSION 3.0)
project(nodal-example-math-nodes CXX C)

if(NOT BUILDING_NODAL)
  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

  include(EnableCXX14)
  include(ConfigureSourceDefinitions)
  include(GNUInstallDirs)

  find_package(nodal REQUIRED)
endif()

set(SOURCES
  node_factory.cpp
  compiler.cpp
  main.cpp

  nodes/input.cpp
  nodes/output.cpp
  nodes/math.cpp
  nodes/random.cpp
)

set(HEADERS
  node.h
  node_factory.h
  context.h
  compiler.h

  nodes/all.h
  nodes/input.h
  nodes/output.h
  nodes/math.h
  nodes/random.h
)

configure_source_definitions(
  "__NODAL_EXAMPLE_MATH_NODES_BUILDING_@id@__"
  ${SOURCES})

add_executable(nodal-example-math-nodes ${SOURCES} ${HEADERS})

enable_cxx14(nodal-example-math-nodes)
target_link_libraries(nodal-example-math-nodes nodal)

install(TARGETS nodal-example-math-nodes
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

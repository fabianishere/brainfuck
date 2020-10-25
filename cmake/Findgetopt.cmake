# Copyright (c) 2020 Fabian Mastenbroek
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

find_path(GETOPT_INCLUDE_DIR getopt.h)
find_library(GETOPT_LIBRARY NAMES wingetopt getopt)

set(GETOPT_LIBRARIES ${GETOPT_LIBRARY})
set(GETOPT_INCLUDE_DIRS ${GETOPT_INCLUDE_DIR})

mark_as_advanced(GETOPT_INCLUDE_DIR GETOPT_LIBRARY)

if(GETOPT_LIBRARY)
    add_library(getopt SHARED IMPORTED GLOBAL)
    set_property(TARGET getopt PROPERTY IMPORTED_LOCATION ${GETOPT_LIBRARY})
else()
    add_library(getopt INTERFACE IMPORTED GLOBAL)
endif()

# Use getopt shim if needed
if(GETOPT_INCLUDE_DIR)
    set_property(TARGET getopt PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GETOPT_INCLUDE_DIRS})
else()
    set_property(TARGET getopt PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/deps/getopt")
endif()

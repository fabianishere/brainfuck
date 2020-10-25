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

find_path(EDITLINE_INCLUDE_DIR editline/readline.h)
find_library(EDITLINE_LIBRARY NAMES edit)

set(EDITLINE_LIBRARIES ${EDITLINE_LIBRARY})
set(EDITLINE_INCLUDE_DIRS ${EDITLINE_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set EDITLINE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(editline DEFAULT_MSG EDITLINE_INCLUDE_DIR EDITLINE_LIBRARY)

mark_as_advanced(EDITLINE_INCLUDE_DIR EDITLINE_LIBRARY)

if(EDITLINE_FOUND)
    add_library(editline SHARED IMPORTED GLOBAL)
    set_property(TARGET editline PROPERTY IMPORTED_LOCATION ${EDITLINE_LIBRARY})
    set_property(TARGET editline PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${EDITLINE_INCLUDE_DIRS})
endif()

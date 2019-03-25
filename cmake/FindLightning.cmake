# Copyright (c) 2019 Fabian Mastenbroek
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

find_path(LIGHTNING_INCLUDE_DIR lightning.h)
find_library(LIGHTNING_LIBRARY NAMES lightning)

set(LIGHTNING_LIBRARIES ${LIGHTNING_LIBRARY})
set(LIGHTNING_INCLUDE_DIRS ${LIGHTNING_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIGHTNING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(lightning DEFAULT_MSG LIGHTNING_INCLUDE_DIR LIGHTNING_LIBRARY)

mark_as_advanced(LIGHTNING_INCLUDE_DIR LIGHTNING_LIBRARY)

if(LIGHTNING_FOUND)
    add_library(lightning SHARED IMPORTED GLOBAL)
    set_property(TARGET lightning PROPERTY IMPORTED_LOCATION ${LIGHTNING_LIBRARY})
    set_property(TARGET lightning PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIGHTNING_INCLUDE_DIRS})
endif()

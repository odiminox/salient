/* BSD 3-Clause License
 *
 * Copyright © 2008-2022, Jice, Odiminox and the salient contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \file salient_c.c
 *
 *  To statically link a C/C++ program with salient:
 *
 *  * Add `salient_c.c` and `salient.cpp` to your list of source files to
 *    compile.
 *  * Add `vendor/` as an include directory.
 *  * Add `SDL2` to be dynamically linked, and add its include directory.
 *    You can get the development libraries from:
 *    https://www.libsdl.org/download-2.0.php
 *  * Add `GL` to be dynamically linked.
 *  * Compile and link salient's src/vendor/glad.c file.
 *  * Compile and link salient's src/vendor/lodepng.cpp file.
 *  * Compile and link `src/vendor/utf8proc/utf8proc.c`.
 *  * Link with `zlib`, and add its include directory.  You can include
 *    salient's src/vendor/zlib directory and compile all `.c` files in there
 *    to statically link it.
 *
 *  You can define `NO_OPENGL` to remove the dependency on the GL library.
 *  You can also define `SALIENT_BARE` to remove the dependency on SDL2 and GL.
 */

#include "../vendor/stb.c"
#include "base/circle.c"

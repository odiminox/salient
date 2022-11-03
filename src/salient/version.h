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

#ifndef SALIENT_VERSION_H
#define SALIENT_VERSION_H

#define SALIENT_MAJOR_VERSION 0
#define SALIENT_MINOR_VERSION 0
#define SALIENT_PATCHLEVEL 1

#define SALIENT_STRVERSION "0.0.1"

/**
    Converts version numbers into a numeric value.
    \rst
    (1, 2, 3) -> 0x10203

    .. versionadded:: 1.16
    \endrst
 */
#define SALIENT_VERSIONNUM(major, minor, patch) ((major)*0x010000 + (minor)*0x0100 + (patch))

/**
    The version of salient currently being compiled.
    \rst
    .. versionadded:: 1.16
    \endrst
 */
#define SALIENT_COMPILEDVERSION SALIENT_VERSIONNUM(SALIENT_MAJOR_VERSION, SALIENT_MINOR_VERSION, SALIENT_PATCHLEVEL)

/**
    \rst
    .. deprecated:: 1.16
    \endrst
 */
#define SALIENT_HEXVERSION SALIENT_COMPILEDVERSION

/**
    \rst
    .. deprecated:: 1.16
    \endrst
 */
#define SALIENT_TECHVERSION (SALIENT_HEXVERSION * 0x100)

/**
    \rst
    .. deprecated:: 1.16
    \endrst
 */
#define SALIENT_STRVERSIONNAME "salient " SALIENT_STRVERSION
/***************************************************************************
    @brief Returns true if the compiled version of salient is at least (major, minor).

    \rst
    .. versionadded:: 1.19
    \endrst
 */
#define SALIENT_VERSION_ATLEAST(major, minor) (SALIENT_COMPILEDVERSION >= SALIENT_VERSIONNUM(major, minor, 0))

#define SALIENT_TITLE "Salient"
#define SALIENT_VERSION "10.11"
#define SALIENT_STATUS "Alpha"
#define SALIENT_LICENCE                                                                                               \
  SALIENT_TITLE                                                                                                       \
  "\nCopyright (c) 2009-2022, Jice, Odiminox and all the Salient contributors\nAll rights "                           \
  "reserved.\n\nRedistribution and use "                                                                              \
  "in source and binary "                                                                                             \
  "forms, with or without\nmodification, are permitted provided that the following conditions are met:\n    * "       \
  "Redistributions of source code must retain the above copyright\n      notice, this list of conditions and the "    \
  "following disclaimer.\n    * Redistributions in binary form must reproduce the above copyright\n      notice, "    \
  "this list of conditions and the following disclaimer in the\n      documentation and/or other materials "          \
  "provided with the distribution.\n    * The names of Jice or any of the Salient contributors may not be used to "   \
  "endorse or promote "                                                                                               \
  "products\n      derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS "        \
  "PROVIDED BY JICE AND THE SALIENT CONTRIBUTORS \"AS IS\" AND ANY\nEXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT "   \
  "NOT LIMITED TO, "                                                                                                  \
  "THE IMPLIED\nWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\nDISCLAIMED. IN NO EVENT "     \
  "SHALL JICE, ODIMINOX OR ANY OTHER SALIENT CONTRIBUTORS BE LIABLE FOR ANY\nDIRECT, INDIRECT, INCIDENTAL, SPECIAL, " \
  "EXEMPLARY, "                                                                                                       \
  "OR CONSEQUENTIAL "                                                                                                 \
  "DAMAGES\n(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\nLOSS OF USE, DATA, OR "     \
  "PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\nON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, "          \
  "STRICT LIABILITY, OR TORT\n(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF "              \
  "THIS\nSOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
#define SALIENT_LICENCE_SHORT \
  SALIENT_TITLE "\nCopyright (c) 2009-2022, Jice, Odiminox and all the Salient contributors\nAll rights reserved."

#endif /* SALIENT_VERSION_H */

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
#ifndef SALIENT_CONFIG_H_
#define SALIENT_CONFIG_H_

/* DLL export */
#ifndef SALIENT_API
#ifdef SALIENT_STATIC
#define SALIENT_API
#elif defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
#ifdef SALIENT_EXPORTS
#ifdef __GNUC__
#define SALIENT_API __attribute__((dllexport))
#else
#define SALIENT_API __declspec(dllexport)
#endif  // __GNUC__
#else
#ifdef __GNUC__
#define SALIENT_API __attribute__((dllimport))
#else
#define SALIENT_API __declspec(dllimport)
#endif  // __GNUC__
#endif  // SALIENT_EXPORTS
#elif __GNUC__ >= 4
#define SALIENT_API __attribute__((visibility("default")))
#else
#define SALIENT_API
#endif
#endif  // SALIENT_API

#ifndef SALIENT_CAPI
#ifdef __cplusplus
#define SALIENT_CAPI extern "C" SALIENT_API
#else
#define SALIENT_CAPI SALIENT_API
#endif  // __cplusplus
#endif  // SALIENT_CAPI

// Publicly visible symbols and functions.
#define TCOD_PUBLIC SALIENT_API

// Private hidden symbols.
#if __GNUC__ >= 4
#define SALIENT_PRIVATE __attribute__((visibility("hidden")))
#else
#define SALIENT_PRIVATE
#endif  // __GNUC__ >= 4

// Cross platform deprecation.
#ifdef SALIENT_IGNORE_DEPRECATED
#define SALIENT_DEPRECATED(msg)
#define SALIENT_DEPRECATED_NOMESSAGE
#define SALIENT_DEPRECATED_ENUM
#elif defined(__cplusplus) && __cplusplus >= 201402L && !defined(__clang__)
#define SALIENT_DEPRECATED(msg) [[deprecated(msg)]]
#define SALIENT_DEPRECATED_NOMESSAGE [[deprecated]]
#define SALIENT_DEPRECATED_ENUM [[deprecated]]
#elif defined(_MSC_VER)
#define SALIENT_DEPRECATED(msg) __declspec(deprecated(msg))
#define SALIENT_DEPRECATED_NOMESSAGE __declspec(deprecated)
#define SALIENT_DEPRECATED_ENUM
#elif defined(__GNUC__)
#define SALIENT_DEPRECATED(msg) __attribute__((deprecated(msg)))
#define SALIENT_DEPRECATED_NOMESSAGE __attribute__((deprecated))
#define SALIENT_DEPRECATED_ENUM __attribute__((deprecated))
#else
#define SALIENT_DEPRECATED(msg)
#define SALIENT_DEPRECATED_NOMESSAGE
#define SALIENT_DEPRECATED_ENUM
#endif

#ifdef __GNUC__
// Tells GCC the these functions are printf-like.
#define SALIENT_PRINTF(str_index, first_arg) __attribute__((format(printf, str_index, first_arg)))
#else
#define SALIENT_PRINTF(str_index, first_arg)
#endif

#define SALIENT_FORMAT SALIENT_PRINTF

#if defined(__cplusplus) && __cplusplus >= 201703L && !defined(__clang__)
#define SALIENT_NODISCARD [[nodiscard]]
#elif defined(_MSC_VER)
#define SALIENT_NODISCARD
#elif defined(__GNUC__)
#define SALIENT_NODISCARD __attribute__((warn_unused_result))
#else
#define SALIENT_NODISCARD
#endif

#ifndef SALIENT_FALLBACK_FONT_SIZE
// The default height of the fallback font size in pixels.
#define SALIENT_FALLBACK_FONT_SIZE 16
#endif  // SALIENT_FALLBACK_FONT_SIZE

#endif  // SALIENT_CONFIG_H_

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
#pragma once
#include <fmt/printf.h>

#include <string_view>
#include <vector>

#include "engine/engine.hpp"

namespace logger {
/**
 * Type of message placed in the log.
 */
enum LogType { LOGTYPE_INFO, LOGTYPE_NOTICE, LOGTYPE_WARNING, LOGTYPE_ERROR, LOGTYPE_FATAL };

/**
 * The results of closing a log block.
 */
enum LogResult { LOGRESULT_FAILURE, LOGRESULT_SUCCESS, LOGRESULT_NONE };

/**
 * The message log, used for debugging. It logs messages in a nested hierarchy, showing not only the place and time of
 * saving the message, but also the caller-callee dependency (if used in both the caller and callee methods).
 */
class Log {
  friend int engine::Engine::run();

 private:
  /**
   * A single message in the log. Used internally by the log.
   */
  struct LogMessage {
    std::string msg;
    uint32_t time;
    LogResult result;
    LogType logType;
    int indent;
  };
  /**
   * A pointer to the file stream where the log messages are output.
   */
  static inline FILE* out{};
  /**
   * The indent level, marking block nesting in the log.
   */
  static inline int indent{};
  /**
   * A list of log messages.
   */
  static inline std::vector<LogMessage> messages{};
  /**
   * Saves the logged messages to a log file.
   */
  static void save();
  /**
   * Initialises the log, creating a new file stream to write the log messages to.
   */
  static void initialise();
  /**
   * Outputs a log message to the file stream.
   * @param type the type of log message (info, notice, warning, etc.)
   * @param result in block closing messages, the end result (success/failure)
   * @param indent in block closing/opening messages, the indent level shift. Zero in all other cases.
   * @param str the message string
   * @return the index number of the message that has been added to the log
   */
  static int output(LogType type, LogResult result, int indent, const char* str);
  /**
   * Outputs a log message to the file stream.
   * @param type the type of log message (info, notice, warning, etc.)
   * @param result in block closing messages, the end result (success/failure)
   * @param indent in block closing/opening messages, the indent level shift. Zero in all other cases.
   * @param str the message string
   * @return the index number of the message that has been added to the log
   */
  static int output(LogType type, LogResult result, int indent, std::string str);

 public:
  /**
   * Opens a block in the log (increases the indent). The log level of this message is <code>INFO</code>.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int openBlock(const S& str, const T&... args) {
    return openBlock(fmt::sprintf(str, args...));
  }
  /**
   * Opens a block in the log (increases the indent). The log level of this message is <code>INFO</code>.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int openBlock(std::string str);
  /**
   * Puts a message with the log level <code>INFO</code> in the log.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int info(const S& str, const T&... args) {
    return info(fmt::sprintf(str, args...));
  }
  /**
   * Puts a message with the log level <code>INFO</code> in the log.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int info(std::string str);
  /**
   * Puts a message with the log level <code>NOTICE</code> in the log.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int notice(const S& str, const T&... args) {
    return notice(fmt::sprintf(str, args...));
  }
  /**
   * Puts a message with the log level <code>NOTICE</code> in the log.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int notice(std::string str);
  /**
   * Puts a message with the log level <code>WARNING</code> in the log.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int warning(const S& str, const T&... args) {
    return warning(fmt::sprintf(str, args...));
  }
  /**
   * Puts a message with the log level <code>NOTICE</code> in the log.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int warning(std::string str);
  /**
   * Puts a message with the log level <code>ERROR</code> in the log.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int error(const S& str, const T&... args) {
    return error(fmt::sprintf(str, args...));
  }
  /**
   * Puts a message with the log level <code>ERROR</code> in the log.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int error(std::string str);
  /**
   * Puts a message with the log level <code>FATAL ERROR</code> in the log.
   * @param str the log message string, <code>printf</code>-like formatted
   * @param ... optional parametres for the message formatting
   * @return the index number of the message that has been added to the log
   */
  template <typename S, typename... T>
  static int fatalError(const S& str, const T&... args) {
    return fatalError(fmt::sprintf(str, args...));
  }
  /**
   * Puts a message with the log level <code>FATAL ERROR</code> in the log.
   * @param str the log message string, as a C++ string
   * @return the index number of the message that has been added to the log
   */
  static int fatalError(std::string str);
  /**
   * Closes a block in the log (decreases the indent). The log level is <code>INFO</code>.
   * @param result (optional) the result of the block (success or failure). A failure will typically be preceded with a
   * logged message describing the reason for the block to end with a failure.
   * @return the index number of the message that has been added to the log
   */
  static int closeBlock(LogResult result = LOGRESULT_NONE);
  /**
   * Returns the number of messages that have been logged so far. This includes all messages, regardless of their log
   * level.
   * @return the total number of messages in the log
   */
  static int size() { return static_cast<int>(messages.size()); }
  /**
   * Returns the number of messages of a given type that have been logged so far. Ignores all other message types.
   * @param type type of log message
   * @return the total number of messages of the specified type in the log
   */
  static int size(LogType type);
  /**
   * Retrieves a message text from the message log.
   * @param idx the index of the message in the log. If left at default, the last logged message will be returned.
   * @return the message text of the desired message
   */
  static std::string get(int idx = -1);
};
}  // namespace logger

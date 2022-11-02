/*
 * Umbra
 * Copyright (c) 2009 Mingos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of Mingos may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MINGOS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MINGOS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "logger/log.hpp"

#include <SDL_timer.h>
#include <fmt/printf.h>
#include <stdarg.h>
#include <stdio.h>

#include <libtcod/libtcod.hpp>

#include "config/config.hpp"
#include "version.hpp"

namespace logger {
constexpr std::array logTypeString{"INF.", "NOT.", "WAR.", "ERR.", "FAT."};

constexpr std::array logTypeStringFull{"INFO", "NOTICE", "WARNING", "ERROR", "FATAL ERROR"};

constexpr std::array logTypeStringLong{"INFO", "NOTIFICATION", "WARNING", "ERROR", "FATAL ERROR"};

constexpr std::array resultString{"[END BLOCK: FAILURE]", "[END BLOCK: SUCCESS]", "[END BLOCK]"};

void Log::initialise() {
  out = fopen("log.txt", "w");
  fmt::fprintf(
      out,
      UMBRA_TITLE " ver. " UMBRA_VERSION " (" UMBRA_STATUS
                  ") Log file, Running time on creation: %dms.\n"
                  "---===---\n"
                  "INF. = INFORMATION. Informative message.\n"
                  "NOT. = NOTICE. Something unexpected that does not affect the program execution.\n"
                  "WAR. = WARNING. An error that may potentially provoke some misbehaviour.\n"
                  "ERR. = ERROR. An error that is guaranteed to provoke some misbehaviour.\n"
                  "FAT. = FATAL ERROR. An error that prevents the program from continuing.\n"
                  "---===---",
      SDL_GetTicks());
  fflush(out);
}

void Log::save() {
  indent = 0;
  Log::info("Log file saved.");
  if (out != NULL) fclose(out);
}

int Log::output(LogType type, LogResult res, int ind, std::string str) {
  if (config::Config::logLevel > (config::LogLevel)type) return 0;
  if (out == NULL) {
    initialise();
    if (res >= LOGRESULT_FAILURE && indent == 0) {
      return error("Log::closeBlock | Tried to close a block, but it hasn't been opened in the first place.");
    }
  }
  // create the message
  auto msg = LogMessage{std::move(str), SDL_GetTicks(), res, type, indent};
  // create the arrows marking the indent level
  std::string arrows;
  for (int i = 0; i < indent; ++i)
    if (i < indent - 1)
      arrows += "|   ";
    else {
      if (res >= LOGRESULT_FAILURE)
        arrows += "\\---";
      else
        arrows += "|   ";
    }
  // if result is a negative number, then it's not a block close
  if (res < LOGRESULT_FAILURE)
    fmt::fprintf(out, "\n%s %06d %s%s", logTypeString[msg.logType], msg.time, arrows, msg.msg);
  // else we're closing a block (no message, just block close notification)
  else
    fmt::fprintf(out, "\n%s %06d %s%s", logTypeString[msg.logType], msg.time, arrows, resultString[msg.result]);
  fflush(out);
  indent += ind;
  messages.emplace_back(msg);
  return static_cast<int>(messages.size());
}

int Log::output(LogType type, LogResult res, int ind, const char* str) {
  assert(str);
  return output(type, res, ind, std::string(str));
}

int Log::openBlock(std::string str) { return output(LOGTYPE_INFO, (LogResult)(-1), 1, std::move(str)); }

int Log::info(std::string str) { return output(LOGTYPE_INFO, (LogResult)(-1), 0, std::move(str)); }

int Log::notice(std::string str) { return output(LOGTYPE_NOTICE, (LogResult)(-1), 0, std::move(str)); }

int Log::warning(std::string str) { return output(LOGTYPE_WARNING, (LogResult)(-1), 0, std::move(str)); }

int Log::error(std::string str) { return output(LOGTYPE_ERROR, (LogResult)(-1), 0, std::move(str)); }

int Log::fatalError(std::string str) { return output(LOGTYPE_FATAL, (LogResult)(-1), 0, std::move(str)); }

int Log::closeBlock(LogResult result) { return output(LOGTYPE_INFO, result, -1, ""); }

int Log::size(LogType type) {
  if (type < LOGTYPE_INFO || type > LOGTYPE_FATAL) {
    error("Log::size | Specified an invalid log message type.");
    return 0;
  }
  int count = 0;
  for (LogMessage& msg : messages) {
    if (msg.logType == type) ++count;
  }
  return count;
}

std::string Log::get(int idx) {
  LogMessage* msg{};
  if (idx == -1) {
    if (messages.size() > 0)
      msg = &messages.back();
    else
      return "No messages logged.";
  } else if (idx < -1 || idx >= static_cast<int>(messages.size())) {
    error("Log::get | Tried to retrieve a message with index %d, but such an index does not exist.", idx);
    msg = &messages.back();
  } else
    msg = &messages.at(idx);
  return fmt::format("{}: {}", logTypeStringFull.at(msg->logType), msg->msg);
}
}  // namespace logger

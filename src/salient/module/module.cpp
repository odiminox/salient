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
#include "module/module.hpp"

#include <SDL_timer.h>
#include <libtcod/parser.h>

#include <libtcod/sys.hpp>

#include "engine/engine.hpp"
#include "logger/log.hpp"

namespace module {
void Module::setActive(bool active) {
  if (status_ == UNINITIALISED) {
    onInitialise();
    status_ = INACTIVE;
  }
  if (active && status_ == INACTIVE) {
    status_ = ACTIVE;
    onActivate();
  } else if (!active && status_ >= ACTIVE) {
    status_ = INACTIVE;
    onDeactivate();
  }
}

void Module::setPause(bool paused) {
  if (status_ == UNINITIALISED) {
    onInitialise();
    status_ = INACTIVE;
  }
  if (paused && status_ != PAUSED) {
    status_ = PAUSED;
    onPause();
  } else if (!paused && status_ == PAUSED) {
    status_ = ACTIVE;
    onResume();
  }
}

void Module::initialiseTimeout() {
  if (timeout_ == 0)
    return;
  else
    timeout_end_ = SDL_GetTicks() + timeout_;
}

void Module::setFallback(const char* module_name) {
  Module* mod = getEngine()->getModule(module_name);
  if (mod) {
    setFallback(mod->getID());
  } else {
    logger::Log::error("Module::setFallback | Unknown module \"%s\".", module_name);
  }
}

void Module::setParameter(std::string_view param_name, TCOD_value_t value) {
  for (ModuleParameter& it : params_) {
    if (it.name == param_name) {
      // already exists. update value
      // this happens when value is overriden in module.cfg
      it.value = value;
      return;
    }
  }
  params_.emplace_back(ModuleParameter{std::string{param_name}, value});
}

Module::ModuleParameter& Module::getParameter(std::string_view param_name) {
  static ModuleParameter def = {"", {0}};
  for (ModuleParameter& it : params_) {
    if (it.name == param_name) return it;
  }
  return def;
}

auto Module::getEngine() -> engine::Engine* { return engine::Engine::getInstance(); }
}  // namespace module

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

#include <SDL.h>
#include <stdio.h>

#include "circle.hpp"
#include "credits.hpp"
#include "demo.hpp"
#include "globals.hpp"
#include "matrix.hpp"
#include "panel.hpp"
#include "rabbit.hpp"

engine::Engine salient_engine("data/cfg/salient.txt", engine::REGISTER_ALL);

class ModuleFactory : public module::ModuleFactory {
 public:
  module::Module* createModule(const char* name) {
    if (strcmp(name, "matrix") == 0)
      return new Matrix();
    else if (strcmp(name, "demo") == 0)
      return new Demo();
    else if (strcmp(name, "rabbit") == 0)
      return new RabbitWidget();
    else if (strcmp(name, "panel") == 0)
      return new Panel();
    else if (strcmp(name, "credits") == 0)
      return new Credits();
    else if (strcmp(name, "circle") == 0)
      return new Circle();
    else
      return NULL;
  }
};

int main(int argc, char* argv[]) {
  // set window title
  salient_engine.setWindowTitle("Salient demo");
  salient_engine.setKeyboardMode(engine::KEYBOARD_SDL);
  // initialise and run the engine
  if (salient_engine.loadModuleConfiguration("data/cfg/module.txt", new ModuleFactory()) && salient_engine.initialise())
    return salient_engine.run();
  else
    return 1;
}

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
#include "demo.hpp"

#include <stdio.h>

bool Demo::update() {
  if (getActive()) {
    int i, j;
    for (i = 0; i < getEngine()->getRootWidth(); i++)
      for (j = 0; j < getEngine()->getRootHeight(); j++) {
        float f[2];
        f[0] = 8.0f * i / getEngine()->getRootWidth();
        f[1] = (8.0f * j / getEngine()->getRootHeight()) + offset;
        int val = (uint8_t)((noise.get(f) + 1.0f) * 24.0f);
        img->putPixel(i, j, TCODColor(0, val / 2, val));
      }
    offset += 0.01f;
    return true;
  } else
    return false;
}

void Demo::render() {
  using namespace std::string_literals;
  img->blit(
      TCODConsole::root,
      static_cast<float>(getEngine()->getRootWidth() / 2),
      static_cast<float>(getEngine()->getRootHeight() / 2));
  tcod::print(*TCODConsole::root, {1, 3}, "Read custom parameters from module.txt:", tcod::ColorRGB{255, 255, 255}, {});
  tcod::print(
      *TCODConsole::root, {1, 5}, "chainParam1:"s + getStringParam("chainParam1"), tcod::ColorRGB{255, 255, 255}, {});
  tcod::print(
      *TCODConsole::root, {1, 6}, "chainParam2:"s + getStringParam("chainParam2"), tcod::ColorRGB{255, 255, 255}, {});
  tcod::print(
      *TCODConsole::root, {1, 7}, "chainParam3:"s + getStringParam("chainParam3"), tcod::ColorRGB{255, 255, 255}, {});
  tcod::print(
      *TCODConsole::root, {1, 8}, "moduleParam:"s + getStringParam("moduleParam"), tcod::ColorRGB{255, 255, 255}, {});
}

void Demo::onEvent(const SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    switch (ev.key.keysym.sym) {
      case SDLK_SPACE:
        getEngine()->activateModule(666);
        break;
      case SDLK_RETURN:
      case SDLK_RETURN2:
      case SDLK_KP_ENTER:
        logger::Log::error("This is a test error");
        getEngine()->displayError();
        break;
      case SDLK_ESCAPE:
        getEngine()->deactivateAll();
        break;
      default:
        break;
    }
  }
}

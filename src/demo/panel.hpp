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
#ifndef PANEL_HPP
#define PANEL_HPP
#include <salient/salient.h>

#include <libtcod/libtcod.hpp>

#include "globals.hpp"

class Panel : public widget::Widget {
 public:
  Panel() {
    rect.set(posx, posy, width, height);
    bQuit.onMouseClick.connect(this, &Panel::onQuit);
  }
  bool update() override;
  void render() override;
  void onEvent(const SDL_Event& ev) override {
    widget::Widget::onEvent(ev);
    bQuit.onEvent(ev);
    return;
  }
  void onQuit(widget::Widget*, events::Event) { salient_engine.deactivateAll(true); }

 private:
  int width{24};
  int height{48};
  int posx{0};
  int posy{(getEngine()->getRootHeight() - height) / 2};
  uint32_t delay{3000};
  uint64_t lastHover{0};  // the time the mouse last hovered over the panel
  TCODConsole panel{width, height};
  widget::Button bQuit{this, 2, 2, 20, 3, "Quit"};
};

#endif

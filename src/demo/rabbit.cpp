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
#include "rabbit.hpp"

#include "globals.hpp"

RabbitWidget::RabbitWidget() {
  rect.set(salient_engine.getRootWidth() / 2 - 12, salient_engine.getRootHeight() / 2 - 6, 24, 12);
  setDragZone(0, 0, 24, 1);
  button.set(this, 10, 7, 4, 3, "OK");
}

void RabbitWidget::onInitialise() {
  // connect signals to their respective slots
  button.onMouseClick.connect(this, &RabbitWidget::onNextDemo);
}

void RabbitWidget::onNextDemo(widget::Widget*, events::Event) {
  setActive(false);
  salient_engine.getModule("matrix")->setActive(false);
}

void RabbitWidget::render() {
  rabbit.setDefaultForeground(TCODColor::white);
  rabbit.setDefaultBackground(TCODColor::black);
  rabbit.printFrame(0, 0, 24, 12, true, TCOD_BKGND_SET, "Wake up, Neo");
  rabbit.printRectEx(
      12, 2, 24, 6, TCOD_BKGND_NONE, TCOD_CENTER, "The Matrix has you. Press OK to follow the white rabbit.");
  if (dragZone.mouseHover || isDragging) {
    rabbit.setDefaultBackground(TCODColor::lightRed);
    rabbit.rect(5, 0, 14, 1, false, TCOD_BKGND_SET);
  }
  if (button.rect.mouseHover)
    rabbit.setDefaultForeground(TCODColor::lightGreen);
  else
    rabbit.setDefaultForeground(TCODColor::white);
  button.render(&rabbit);
  TCODConsole::blit(&rabbit, 0, 0, rect.w, rect.h, TCODConsole::root, rect.x, rect.y, 1.0f, 0.5f);
}

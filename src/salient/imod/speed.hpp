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
#include "widget/widget.hpp"

namespace imod {
class ModSpeed : public widget::Widget {
  friend class engine::Engine;

 public:
  ModSpeed();
  /**
   * Gathers data about time useage.
   * @return <code>true</code> if Speedo is to continue active, <code>false</code> it it should be deactivated
   */
  bool update() override;
  /**
   * Displays the Speedo widget.
   */
  void render() override;
  /**
   * Parses mouse input.
   */
  void mouse(TCOD_mouse_t&) override{};
  void onEvent(const SDL_Event&) override;
  /**
   * Sets the minimised state of the widget.
   * @param val <code>true</code> for minimised, <code>false</code> for maximised
   */
  inline void setMinimised(bool val) { isMinimized = val; }

 private:
  float cumulatedElapsed{0.0f};
  float updateTime{0.0f};
  float renderTime{0.0f};
  int updatePer{0}, renderPer{0}, sysPer{0};
  TCODImage* timeBar{};
  TCODConsole* speed{};
  int fps{};
  bool isMinimized{false};

  /**
   * Removes the frames per second limit in order to attempt to enforce a 100% load on the CPU.
   */
  void onActivate();
  /**
   * Restores the frames per second limit from before Speedo activation.
   */
  void onDeactivate();
  /**
   * Uptades the update and render times.
   * @param updateTime the time spend on <code>update()</code> methods
   * @param renderTime the time spend on <code>render()</code> methods
   */
  void setTimes(long updateTime, long renderTime);  // this is called by engine each frame
};
}  // namespace imod

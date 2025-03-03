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

#include <iostream>

#include "base/rect.hpp"
#include "widget/widget.hpp"

namespace widget {
class Button : public widget::Widget {
 public:
  Button() = default;
  Button(widget::Widget* parent, int x, int y, int w, int h, const char* tag = "");
  Button(widget::Widget* parent, int x, int y, int w, int h, std::string tag = "");
  virtual ~Button() = default;
  /**
   * Sets the basic properties of the button: parent widget, position in the console, size and tag.
   * @param parent a pointer to the widget::Widget containing the button
   * @param x the <code>x</code> coordinate of the top left corner of the button area
   * @param y the <code>y</code> coordinate of the top left corner of the button area
   * @param w the button area's width
   * @param h the button area's height
   * @param tag the tag's text
   */
  void set(widget::Widget* parent, int x, int y, int w, int h, const char* tag = "");
  /**
   * Sets the basic properties of the button: parent widget, position in the console, size and tag.
   * @param new_parent a pointer to the widget::Widget containing the button
   * @param x the <code>x</code> coordinate of the top left corner of the button area
   * @param y the <code>y</code> coordinate of the top left corner of the button area
   * @param w the button area's width
   * @param h the button area's height
   * @param new_tag the tag's text
   */
  inline void set(widget::Widget* new_parent, int x, int y, int w, int h, std::string new_tag = "") {
    set(new_parent, x, y, w, h, new_tag.c_str());
  }
  /**
   * Renders the button.
   */
  virtual void render(TCODConsole* con);
  bool visible{true};  // visibility (can be toggled)
  std::string tag{""};  // the descriptive tag
};
}  // namespace widget

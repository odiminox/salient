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
#include "widget/stylesheet.hpp"

#include <string.h>

#include "salient.hpp"

namespace widget {
StyleSheetSet::StyleSheetSet() {
  colour.set(TCODColor::white);
  backgroundColour.set(TCODColor::darkerRed);
  borderColour.set(TCODColor::blue);
}

StyleSheet::StyleSheet() {
  // placeholder --- hardcoded values
  active.colour(TCODColor::yellow);
  hover.backgroundColour(TCODColor::darkRed);
  active.backgroundColour(TCODColor::red);
  hover.borderColour(TCODColor::lightBlue);
  active.borderColour(TCODColor::lighterBlue);
}

StyleSheet& StyleSheet::colour(TCODColor val) {
  normal.colour = hover.colour = active.colour = val;
  return *this;
}

StyleSheet& StyleSheet::backgroundColour(TCODColor val) {
  normal.backgroundColour = hover.backgroundColour = active.backgroundColour = val;
  return *this;
}

StyleSheet& StyleSheet::borderColour(TCODColor val) {
  normal.borderColour = hover.borderColour = active.borderColour = val;
  return *this;
}
}  // namespace widget

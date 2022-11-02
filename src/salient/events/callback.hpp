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
#include "base/key.hpp"
#include "engine/engine.hpp"

namespace events {
class Callback {
  friend class engine::Engine;

 public:
  virtual ~Callback() = default;

 protected:
  base::Key key{};
  /**
   * Checks whether the collected keyboard event corresponds to the one the callback is listening to.
   * @param k the keyboard event collected by the engine
   * @return <code>true</code> if the keyboard events match, <code>false</code> otherwise
   */
  virtual inline bool evaluate(base::Key k) {
    if (k == key)
      return true;
    else
      return false;
  }
  /**
   * A pure virtual method (ie. it is mandatory to override it) containing custom code executed whenever the callback
   * encounters a key event it is listening to.
   */
  virtual void action() = 0;
  /**
   * Fetches a pointer to the engine.
   * @return a pointer to the engine
   */
  inline engine::Engine* getEngine() { return engine::Engine::getInstance(); }
};

// Internal callbacks:

// quit game
class CallbackQuit : public Callback {
 public:
  CallbackQuit();

 private:
  base::Key key2;
  /**
   * Checks whether the collected keyboard event corresponds to one of the Quit key combinations.
   * @param k the keyboard event collected by the engine
   * @return <code>true</code> if the keyboard events match, <code>false</code> otherwise
   */
  inline bool evaluate(base::Key k) {
    if (k == key || k == key2)
      return true;
    else
      return false;
  }
  /**
   * Quits the application.
   */
  void action();
};

// switch fullscreen
class CallbackFullscreen : public Callback {
 public:
  CallbackFullscreen();

 private:
  /**
   * Toggles fullscreen mode on/off.
   */
  void action();
};

// save screenshot
class CallbackScreenshot : public Callback {
 public:
  CallbackScreenshot();

 private:
  /**
   * Saves a screenshot.
   */
  void action();
};

// font up
class CallbackFontUp : public Callback {
 public:
  CallbackFontUp();

 private:
  /**
   * Switches the font to the next one in the registered fonts list.
   */
  void action();
};

// switch font down
class CallbackFontDown : public Callback {
 public:
  CallbackFontDown();

 private:
  /**
   * Switches the font to the previous one in the registered fonts list.
   */
  void action();
};

// pause the program
class CallbackPause : public Callback {
 public:
  CallbackPause();

 private:
  /**
   * Toggles the engine's paused state.
   */
  void action();
};

// pause the program
class CallbackSpeedometer : public Callback {
 public:
  CallbackSpeedometer();

 private:
  /**
   * Turns the Speedometer on and off.
   */
  void action();
};
}  // namespace events

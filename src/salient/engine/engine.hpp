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
#ifndef UMBRA_ENGINE_HPP
#define UMBRA_ENGINE_HPP

#include <SDL_events.h>
#include <fmt/printf.h>
#include <libtcod/console_types.h>

#include <iostream>
#include <libtcod/list.hpp>
#include <string>

#include "base/key.hpp"
#include "config/config.hpp"
#include "events/callback_fwd.hpp"
#include "module/factory.hpp"
#include "module/module.hpp"

namespace engine {
/**
 * The keyboard modes available in Salient. They correspond to the ones used in libtcod.
 */
enum KeyboardMode {
  KEYBOARD_WAIT,
  KEYBOARD_WAIT_NOFLUSH,
  KEYBOARD_RELEASED,
  KEYBOARD_PRESSED,
  KEYBOARD_PRESSED_RELEASED,
  KEYBOARD_SDL,  // Use SDL for events.  Libtcod events are processed immediately and there will be no input lag.
};

/**
 * IDs used to reference internal modules.
 */
enum InternalModuleID { INTERNAL_SPEEDOMETER, INTERNAL_BSOD, INTERNAL_CREDITS, INTERNAL_MAX };

/**
 * Flags used to register callbacks. They can be combined using bitwise <code>OR</code>.
 */
enum RegisterCallbackFlag {
  REGISTER_NONE = 0x00000000,
  REGISTER_DEFAULT = 0x00000001,
  REGISTER_ADDITIONAL = 0x00000002,
  REGISTER_ALL = 0xFFFFFFFF
};

struct CustomCharMap {
  int x;
  int y;
  int code;
};

// the main engine
class Engine {
 public:
  /**
   * The constructor.
   * @param fileName the filename from which to read Umbra's configuration.
   * @param flag the callback registering flags, used to choose which callbacks are to be registered in the application.
   */
  Engine(const char* fileName = "data/cfg/umbra.txt", RegisterCallbackFlag flag = REGISTER_DEFAULT);
  /**
   * The constructor.
   * @param flag the callback registering flags, used to choose which callbacks are to be registered in the application.
   */
  Engine(RegisterCallbackFlag flag) : Engine{"data/cfg/umbra.txt", flag} {}

  ~Engine();

  /**
   * Add custom character mapping. Mappings are put on queue for Engine::initialise()/reinitialise() to call them.
   * @param x position of character in font file
   * @param y position of character in font file
   * @param ascii code associated with the character
   */
  void addCustomCharacter(int x, int y, int code);

  /**
   * Checks whether a module name has already been used in order to prevent registering modules with the same name.
   * @param name the module name to be checked
   * @return <code>true</code> if the name is free and available for use, <code>false</code> otherwise
   */
  bool isNameFree(const char* name);
  /**
   * Registers a module for usage in the application. Unregistered modules cannot be activated. Registering is done only
   * once per application run.<br><i>Note: this method only registers the module, but doesn't activate it. Activation is
   * performed using the Engine::activateModule(*) methods!</i>
   * @param module a pointer to the module to be registered. Creating the module using the <code>new</code> keyword is
   * strongly encouraged, eg. <code>registerModule(new myModule());</code>
   * @param name the module's name
   * @return the module's unique ID number (0 for the first registered module, 1 for the second, etc.)
   */
  int registerModule(module::Module* module, const char* name = NULL);  // add a module to the modules list. returns id
  /**
   * Registers a font for usage in the application.<br><i>Note: you are encouraged to let the engine register fonts
   * automatically. Please refer to the documentation regarding font autodetection.</i>
   * @param columns number of character columns in the font image file
   * @param rows number of character rows in the font image file
   * @param filename the filename of the font image file
   * @param flags font layout flags. Please refer to <b>libtcod</b> documentation for more information on font layout
   * flags.
   */
  void registerFont(int columns, int rows, const char* filename, int flags = TCOD_FONT_LAYOUT_TCOD);
  /**
   * Read module configuration from the given filename, or the filename defined as moduleConfig in umbra.txt.<br>If
   * there's no filename or the file cannot be read, return false.
   * @param filename name of the module configuration file
   * @param factory a module factory
   * @param chainName (optional) the name of the module chain to load. Leave at default to load the chain specified in
   * <code>umbra.txt</code> or, in case it's not specified, the first module chain that's encountered in the module
   * configuration file.
   * @return <code>true</code> if module configuration has been loaded successfully, <code>false</code> otherwise
   */
  bool loadModuleConfiguration(const char* filename, module::ModuleFactory* factory, const char* chainName = NULL);
  /**
   * Read module configuration from the given filename, or the filename defined as moduleConfig in umbra.txt.<br>If
   * there's no filename or the file cannot be read, return false.
   * @param filename name of the module configuration file
   * @param chainName (optional) the name of the module chain to load. Leave at default to load the chain specified in
   * <code>umbra.txt</code> or, in case it's not specified, the first module chain that's encountered in the module
   * configuration file.
   * @return <code>true</code> if module configuration has been loaded successfully, <code>false</code> otherwise
   */
  bool loadModuleConfiguration(const char* filename, const char* chainName = NULL);
  /**
   * Initialises the engine.
   * @param renderer the renderer to be used (defaults to SDL)
   * @return <code>true</code> if initialisation is successful, <code>false</code> if errors were encountered
   */
  bool initialise(TCOD_renderer_t renderer = TCOD_RENDERER_SDL2);
  /**
   * Reinitialises an already initialised engine. Used mainly when the console font or the window title change.
   * @param renderer the renderer to be used (defaults to SDL)
   */
  void reinitialise(TCOD_renderer_t renderer = TCOD_RENDERER_SDL2);
  /**
   * Runs the engine.
   * @return the result of running the application: <i>0</i> if no errors have occurred, different value otherwise.
   */
  int run();
  /**
   * Sets the window title.<br><i>Note: this method is usually called before initialising the engine. Should it be
   * called after the engine has been initialised, the title won't be changed util the engine is reinitialised.</i>
   * @param title the main program window's new title
   * @param ... optional printf-like formatting of the title
   */
  template <typename S, typename... T>
  void setWindowTitle(const S& title, const T&... args) {
    setWindowTitle(fmt::sprintf(title, args...));
  }
  /**
   * Sets the window title.<br><i>Note: this method is usually called before initialising the engine. Should it be
   * called after the engine has been initialised, the title won't be changed util the engine is reinitialised.</i>
   * @param title the main program window's new title
   */
  void setWindowTitle(std::string title);
  /**
   * Sets the keyboard mode.
   * @param mode keyboard mode, as defined in the <code>KeyboardMode</code> enum.
   */
  inline void setKeyboardMode(KeyboardMode mode) { keyboardMode = mode; }
  /**
   * Pauses or unpauses the engine.
   * @param pause <code>true</code> if the engine is to be paused, <code>false</code> otherwise
   */
  inline void setPause(bool pause) { paused = pause; }
  /**
   * Registers a new keyboard callback.
   * @param cbk a pointer to the keyboard callback. You're encouraged to create the callback using the <code>new</code>
   * keyword here: <code>registerCallback(new MyCallback());</code>
   */
  inline void registerCallback(events::Callback* cbk) { callbacks.push_back(cbk); }
  /**
   * Activates a module.
   * @param moduleId the identification number of the module to be activated
   */
  void activateModule(int moduleId);
  /**
   * Activates a module.
   * @param mod a pointer to the module object to be activated
   */
  void activateModule(module::Module* mod);
  /**
   * Activates an internal module.
   * @param id the identification number of the internal module to be activated
   */
  void activateModule(InternalModuleID id);
  /**
   * Activates a module.
   * @param moduleName the name of the module to be activated
   */
  void activateModule(const char* name);
  /**
   * Deactivates a module.
   * @param moduleId the identification number of the module to be deactivated
   */
  void deactivateModule(int moduleId);
  /**
   * Deactivates a module.
   * @param mod a pointer to the module object to be deactivated
   */
  void deactivateModule(module::Module* mod);
  /**
   * Deactivates an internal module.
   * @param id the identification number of the internal module to be deactivated
   */
  void deactivateModule(InternalModuleID id);
  /**
   * Deactivates a module.
   * @param moduleName the name of the module be deactivated
   */
  void deactivateModule(const char* name);
  /**
   * Deactivates all active modules, either letting their respective fallbacks to get activated, or ignoring fallbacks
   * completely.
   * @param ignoreFallbacks <code>true</code> if the fallbacks should be ignored, <code>false</code> or default value if
   * the fallbacks should be allowed to get activated.
   */
  void deactivateAll(bool ignoreFallbacks = false);
  /**
   * Gets the message log level. All messages with a lower severity level will be discarded when creating the message
   * log.
   * @return the log level, as defined in the configuration file
   */
  inline config::LogLevel getLogLevel() { return config::Config::logLevel; }
  /**
   * Retrieves the paused state of the engine.
   * @return <code>true</code> if the engine is currently paused, <code>false</code> otherwise
   */
  inline bool getPause() { return paused; }
  /**
   * Retrieves the keyboard mode used by the engine.
   * @return the currently used keyboard mode
   */
  inline KeyboardMode getKeyboardMode() { return keyboardMode; }
  /**
   * Fetches a pointer to a module.
   * @param moduleId the identification number of the module to which a pointer is to be fetched
   * @return a pointer to the requested module
   */
  inline module::Module* getModule(int moduleId) {
    return (moduleId < 0 || moduleId >= modules.size() ? NULL : modules.at(moduleId));
  }
  /**
   * Fetches a pointer to a module.
   * @param moduleName the name of the module to which a pointer is to be fetched
   * @return a pointer to the requested module
   */
  module::Module* getModule(const char* name);
  /**
   * Retrieve the module id from its name
   * @param mod pointer to the module
   * @return the module's id
   */
  int getModuleId(const char* name);
  /**
   * Retrieve the module id from its reference
   * @param mod pointer to the module
   * @return the module's id
   */
  int getModuleId(module::Module* mod);
  /**
   * Fetches a pointer to an internal module.
   * @param id the identification number of the internal module to which a pointer is to be fetched
   * @return a pointer to the requested internal module
   */
  inline module::Module* getModule(InternalModuleID id) {
    return (id < 0 || id >= INTERNAL_MAX ? NULL : internalModules[id]);
  }
  /**
   * Fetches a pointer to the engine instance.
   * @return a pointer to the engine
   */
  inline static Engine* getInstance() {
    if (engineInstance == NULL) engineInstance = new Engine();
    return engineInstance;
  }
  /**
   * Displays the last logged error using the inbuilt BSOD module.
   */
  void displayError();
  /**
   * Activates a different font.
   * @param shift a number indicating whether to activate the next or the previous font in the registered fonts
   * list.<br>This can be -1 (switch down) or 1 (switch up). All other numbers will be clamped to these values.<br>A
   * value of 0 results in doing nothing.
   * @return <code>true</code> if the font has been successfully changed, <code>false</code> otherwise
   */
  inline bool activateFont(int shift = 0) { return config::Config::activateFont(shift); }
  /**
   * Retrieves the width of the console in cells.
   * @return the console's width
   */
  inline int getRootWidth() { return config::Config::rootWidth; }
  /**
   * Retrieves the height of the console in cells.
   * @return the console's height
   */
  inline int getRootHeight() { return config::Config::rootHeight; }
  /**
   * Retrieves the ID number of the currently used font.
   * @return current font's ID
   */
  inline int getFontID() { return config::Config::fontID; }
  /**
   * Retrieves the total number of registered fonts.
   * @return number of registered fonts
   */
  int getNbFonts() { return static_cast<int>(config::Config::fonts.size()); }
  /**
   * Retrieves the font directory.
   * @return the font direcory
   */
  std::filesystem::path getFontDir() { return config::Config::fontDir; }
  /**
   * Sets the root console's dimensions in cells.
   * @param w the root console's width
   * @param h the root console's height
   */
  static void setRootDimensions(int w, int h) {
    config::Config::rootWidth = w;
    config::Config::rootHeight = h;
    getInstance()->reinitialise(renderer);
  }
  /**
   * Displays a credits line.
   * @param x the <code>x</code> coordinate of the credits line placement on the root console
   * @param y the <code>y</code> coordinate of the credits line placement on the root console
   * @param duration the total duration of the display (half of which is the fade out)
   */
  void printCredits(int x, int y, uint32_t duration = 10000);

 private:
  /**
   * Register custom characters
   */
  void registerCustomCharacters();

  static Engine* engineInstance;
  static TCOD_renderer_t renderer;
  std::string windowTitle{""};
  bool paused{false};
  std::vector<CustomCharMap> customChars{};  // List of custom chars to add
  std::vector<module::Module*> modules{};  // list of all registered modules
  std::vector<module::Module*> activeModules{};  // currently active modules
  std::vector<module::Module*> toActivate{};  // modules to activate next frame
  std::vector<module::Module*> toDeactivate{};  // modules to deactivate next frame
  module::Module* internalModules[INTERNAL_MAX]{};
  KeyboardMode keyboardMode{KEYBOARD_RELEASED};
  std::vector<events::Callback*> callbacks{};  // the keybinding callbacks
  /**
   * Parses the keyboard input and passes it to the registered callbacks.
   * @param key a reference to the keyboard event object
   */
  void keyboard(TCOD_key_t& key);
  /**
   * Puts the newly activated module in the active modules list.
   * @param mod a pointer to the module that's being activated
   */
  void doActivateModule(module::Module* mod);
  /**
   * Performs font autodetection and registered any found fonts.
   * @return <code>true</code> if at least one font has been registered, <code>false</code> otherwise
   */
  bool registerFonts();
  /**
   * Registers an internal module for usage in the application.
   * @param id the ID number of an internal module
   * @param module a pointer to the internal module to be registered.
   */
  void registerInternalModule(InternalModuleID id, module::Module* module);
  /// @brief SDL event watcher.
  static int onSDLEvent(void* userdata, SDL_Event* event);
};
}  // namespace engine
#endif

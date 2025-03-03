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
#include "engine.hpp"

#include <SDL_events.h>
#include <SDL_timer.h>
#include <fmt/core.h>
#include <stdarg.h>
#include <stdio.h>

#include <cassert>
#include <filesystem>
#include <iostream>
#include <libtcod/libtcod.hpp>
#include <vector>

#include "base/font.hpp"
#include "events/callback.hpp"
#include "imod/bsod.hpp"
#include "imod/credits.hpp"
#include "imod/speed.hpp"
#include "logger/log.hpp"
#include "version.hpp"

namespace engine {
TCOD_renderer_t Engine::renderer = TCOD_RENDERER_SDL2;
Engine* Engine::engineInstance = NULL;

// specific parser for module.txt file
class UmbraModuleConfigParser : public ITCODParserListener {
 private:
  // name of the chain to parse. If NULL, use the first one in the file
  const char* chainName;
  // current module being parsed
  module::Module* module;
  // shall we skip the current moduleChain ?
  bool skip;
  // whether the active chain has already been parsed (skip other chains)
  bool chainDone;
  // factory that creates a module from its name
  module::ModuleFactory* factory;
  // module to activate
  std::vector<module::Module*> toActivate;
  // custom parameters defined at the module chain level
  std::vector<module::Module::ModuleParameter> chain_parameters_;
  // all the modules in the chain
  std::vector<module::Module*> chainModules;

 public:
  UmbraModuleConfigParser(module::ModuleFactory* factory, const char* chainName)
      : chainName(chainName), module(NULL), skip(false), chainDone(false), factory(factory) {}
  bool parserNewStruct(TCODParser*, const TCODParserStruct* str, const char* name) {
    if (strcmp(str->getName(), "moduleChain") == 0) {
      chainModules.clear();
      // parse a new module chain
      if (chainDone || (chainName && strcmp(name, chainName) != 0)) {
        // this is not the chain we're looking for. skip it
        skip = true;
      }
    } else if (strcmp(str->getName(), "module") == 0 && !skip) {
      // parse a module for current chain
      module = Engine::getInstance()->getModule(name);
      if (!module) {
        // module doesn't exist yet. try to get it from the factory
        if (factory) {
          module = factory->createModule(name);
          if (module) Engine::getInstance()->registerModule(module, name);
        }
        if (!module) {
          // could not get the module. abort
          error(fmt::format("Unknown module '{}'", name).c_str());
          return false;
        }
      }
      assert(module != NULL);
      chainModules.emplace_back(module);
    }
    return true;
  }
  bool parserFlag(TCODParser*, const char* name) {
    if (skip) return true;
    if (strcmp(name, "active") == 0) {
      // deferred activation (cannot activate before all params have been parsed)
      toActivate.emplace_back(module);
    }
    return true;
  }
  bool parserProperty(TCODParser*, const char* name, TCOD_value_type_t, TCOD_value_t value) {
    if (skip) return true;
    if (strcmp(name, "timeout") == 0) {
      module->setTimeout(value.i);
    } else if (strcmp(name, "priority") == 0) {
      module->setPriority(value.i);
    } else if (strcmp(name, "fallback") == 0) {
      module::Module* fallback = Engine::getInstance()->getModule(value.s);
      if (!fallback) {
        // fallback references a module that doesn't exist yet. get it from the factory
        if (factory) {
          fallback = factory->createModule(value.s);
          if (fallback) {
            Engine::getInstance()->registerModule(fallback, value.s);
          }
        }
        if (!fallback) {
          error(fmt::format("Unknown module '{}'", value.s).c_str());
          return false;
        }
      }
      module->setFallback(value.s);
    } else {
      // dynamically declared property.
      if (module) {
        // at module level
        module->setParameter(name, value);
      } else {
        // at module chain level
        chain_parameters_.emplace_back(module::Module::ModuleParameter{std::string{name}, value});
      }
    }
    return true;
  }
  bool parserEndStruct(TCODParser*, const TCODParserStruct* str, [[maybe_unused]] const char* name) {
    if (strcmp(str->getName(), "moduleChain") == 0) {
      if (skip) {
        skip = false;
      } else {
        // finished parsing requested chain. skip other chains
        //  copy module chain parameters into modules
        for (module::Module* mod : chainModules) {
          // inherits all chain parameters
          // those parameters can be overloaded in the module declaration
          for (const auto& chainParam : chain_parameters_) {
            module::Module::ModuleParameter moduleParam = mod->getParameter(chainParam.name);
            if (moduleParam.name.empty()) {
              mod->setParameter(chainParam.name, chainParam.value);
            }  // else parameter overloaded by the module.
          }
        }
        // activate active modules
        for (auto* it : toActivate) {
          Engine::getInstance()->activateModule(it);
        }
        chainDone = true;
      }
    } else if (strcmp(str->getName(), "module") == 0) {
      module = NULL;
    }
    return true;
  }
  void error(const char* msg) override { logger::Log::error(fmt::format("UmbraModuleConfigParser | {}", msg)); }
};

int Engine::onSDLEvent(void* userdata, SDL_Event* event) {
  auto self = static_cast<Engine*>(userdata);
  for (auto& module : self->activeModules) module->onEvent(*event);
  if (event->type == SDL_QUIT) self->deactivateAll();
  return 0;
};

Engine::Engine(const char* fileName, RegisterCallbackFlag flag) {
  logger::Log::openBlock("Engine::Engine | Instantiating the engine object.");
  // load configuration variables
  config::Config::load(fileName);
  setWindowTitle("%s ver. %s (%s)", SALIENT_TITLE, SALIENT_VERSION, SALIENT_STATUS);
  engineInstance = this;
  // register internal modules
  registerInternalModule(INTERNAL_SPEEDOMETER, new imod::ModSpeed());
  registerInternalModule(INTERNAL_BSOD, new imod::ModBSOD());
  registerInternalModule(INTERNAL_CREDITS, new imod::ModCredits());
  // register default callbacks
  if (flag & REGISTER_DEFAULT) {
    registerCallback(new events::CallbackQuit());
    registerCallback(new events::CallbackFullscreen());
    registerCallback(new events::CallbackScreenshot());
    registerCallback(new events::CallbackFontUp());
    registerCallback(new events::CallbackFontDown());
    registerCallback(new events::CallbackPause());
  }
  if (flag & REGISTER_ADDITIONAL) {
    registerCallback(new events::CallbackSpeedometer());
  }
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
  SDL_AddEventWatch(onSDLEvent, this);
}

Engine::~Engine() { SDL_DelEventWatch(onSDLEvent, this); }

void Engine::setWindowTitle(std::string title) { windowTitle = title; }

// check whether there already exists a module with a given name
bool Engine::isNameFree(const char* name) {
  for (auto& it : modules)
    if (it->name_ == name) return false;
  return true;
}

// add a module to the modules list
int Engine::registerModule(module::Module* module, const char* name) {
  if (name != NULL) {
    logger::Log::info("Engine::registerModule | Registering a module named \"%s\".", name);
  } else {
    logger::Log::info("Engine::registerModule | Registering a module.");
  }
  module->id_ = static_cast<decltype(module->id_)>(modules.size());
  if (name) module->name_ = name;
  if (module->name_ == "") module->name_ = std::string("module") + std::to_string(module->id_);
  modules.push_back(module);
  return module->id_;
}

// fetch a pointer to a module from its name
module::Module* Engine::getModule(const char* name) {
  for (auto& it : modules) {
    if (it->name_ == name) return it;
  }
  logger::Log::error(
      "Engine::getModule | Tried to retrieve a module named \"%s\", but such a module was not found.", name);
  return NULL;
}

// get module id from its reference
int Engine::getModuleId(module::Module* mod) {
  if (!mod || mod == NULL) {
    logger::Log::error(
        "Engine::getModuleId | Tried to retrieve the module's ID, but the pointer to a module given either is "
        "NULL or points to an invalid module.");
    displayError();
    return -1;
  } else
    return mod->getID();
}

// get module ID from the name
int Engine::getModuleId(const char* name) {
  module::Module* mod = getModule(name);
  if (!mod || mod == NULL) {
    logger::Log::error(
        "Engine::getModuleId | Tried to retrieve the module's ID, but could not get a module pointer based on the "
        "specified name \"%s\".",
        name);
    displayError();
    return -1;
  } else
    return mod->getID();
}

// register a font
void Engine::registerFont(int columns, int rows, const char* filename, int flags) {
  logger::Log::openBlock("Engine::registerFont | Registering font \"%s\"", filename);
  if (!TCODSystem::fileExists(filename)) {
    logger::Log::error("Engine::registerFont | Tried to register a font file that does not exist: %s.", filename);
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return;
  }
  config::Config::registerFont(base::Font{columns, rows, filename, flags});
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
}

// temporary internal font storage structure
struct TmpFontData {
  int rows;
  int columns;
  std::string name;
  int flags;
  int size;
};

bool Engine::registerFonts() {
  logger::Log::openBlock("Engine::registerFonts | Attempting to automatically register fonts.");
  // if fonts registered by the user, do nothing
  if (getNbFonts() > 0) return true;
  // look for font*png in font directory
  std::vector<std::string> font_files{};
  for (std::filesystem::path file : std::filesystem::directory_iterator{getFontDir()}) {
    if (std::filesystem::is_directory(file)) continue;
    if (file.extension() != ".png") continue;
    if (file.filename().string().substr(0, 4) != "font") continue;  // Starts with
    font_files.emplace_back(file.filename().string());
  }
  if (font_files.size() == 0) {
    logger::Log::fatalError(
        "Engine::registerFonts | No fonts registered. The font directory \"%s\" is empty.",
        getFontDir().string().c_str());
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return false;
  }
  std::vector<TmpFontData> fontDataList;
  for (const auto& fontName : font_files) {
    int charWidth = 0;
    int charHeight = 0;
    char layout[32] = "";
    if (sscanf(fontName.c_str(), "font%dx%d%31s", &charWidth, &charHeight, layout) < 2) continue;
    if (charWidth <= 0 || charHeight <= 0) continue;
    int fontFlag = TCOD_FONT_TYPE_GREYSCALE;
    if (layout[0] == '_') {
      // parse font layout
      if (TCOD_strncasecmp(layout, "_TCOD.", 6) == 0)
        fontFlag |= TCOD_FONT_LAYOUT_TCOD;
      else if (TCOD_strncasecmp(layout, "_INCOL.", 7) == 0)
        fontFlag |= TCOD_FONT_LAYOUT_ASCII_INCOL;
      else if (TCOD_strncasecmp(layout, "_INROW.", 7) == 0)
        fontFlag |= TCOD_FONT_LAYOUT_ASCII_INROW;
    } else {
      // default is TCOD |GREYSCALE
      fontFlag |= TCOD_FONT_LAYOUT_TCOD;
    }
    // compute font grid size from image size & char size
    fontDataList.emplace_back();
    fontDataList.back().name = (getFontDir() / fontName).string();
    TCODImage tmp(fontDataList.back().name.c_str());
    int w = 0;
    int h = 0;
    tmp.getSize(&w, &h);
    fontDataList.back().size = charWidth * charHeight;
    fontDataList.back().rows = h / charHeight;
    fontDataList.back().columns = w / charWidth;
    fontDataList.back().flags = fontFlag;
  }
  // sort fonts by size
  std::sort(fontDataList.begin(), fontDataList.end(), [](auto& a, auto& b) { return a.size < b.size; });
  for (const auto& dat : fontDataList) {
    // register the fonts from smallest to biggest
    registerFont(dat.columns, dat.rows, dat.name.c_str(), dat.flags);
  }

  if (getNbFonts() == 0) {
    logger::Log::fatalError(
        "Engine::registerFonts | No fonts registered. Autodetection found no fonts matching the naming pattern "
        "font<WIDTH>x<HEIGHT>[_<LAYOUT>].png in the specified directory \"%s\".",
        getFontDir().string().c_str());
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return false;
  }
  logger::Log::info("Engine::registerFonts | Successfully registered %d fonts.", getNbFonts());
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
  return true;
}

// load external module configuration
bool Engine::loadModuleConfiguration(const char* filename, module::ModuleFactory* factory, const char* chainName) {
  if (chainName == NULL && filename == NULL)
    logger::Log::openBlock("Engine::loadModuleConfiguration | Attempting to load module configuration.");
  else if (chainName == NULL && filename != NULL)
    logger::Log::openBlock(
        "Engine::loadModuleConfiguration | Attempting to load a module configuration from file \"%s\".", filename);
  else if (chainName != NULL && filename == NULL)
    logger::Log::openBlock(
        "Engine::loadModuleConfiguration | Attempting to load \"%s\" module configuration.", chainName);
  else
    logger::Log::openBlock(
        "Engine::loadModuleConfiguration | Attempting to load \"%s\" module configuration from file \"%s\".",
        chainName,
        filename);
  // TODO: if file name == NULL, retrieve from config::Config
  if (!filename) {
    logger::Log::fatalError("Engine::loadModuleConfiguration | specified an empty filename.");
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return false;  // no configuration file is defined
  }
  if (!TCODSystem::fileExists(filename)) {
    logger::Log::fatalError(
        "Engine::loadModuleConfiguration | there exists no file with the specified filename \"%s\".", filename);
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return false;  // file doesn't exist
  }
  TCODParser parser;
  TCODParserStruct* moduleChain = parser.newStructure("moduleChain");
  TCODParserStruct* module = parser.newStructure("module");
  moduleChain->addStructure(module);
  module->addProperty("timeout", TCOD_TYPE_INT, false);
  module->addProperty("priority", TCOD_TYPE_INT, false);
  module->addProperty("fallback", TCOD_TYPE_STRING, false);
  module->addFlag("active");
  if (chainName == NULL && config::Config::moduleChain != NULL) {
    parser.run(filename, new UmbraModuleConfigParser(factory, config::Config::moduleChain));
  } else
    parser.run(filename, new UmbraModuleConfigParser(factory, chainName));
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
  return true;
}

// load external module configuration
bool Engine::loadModuleConfiguration(const char* filename, const char* chainName) {
  return loadModuleConfiguration(filename, NULL, chainName);
}

// public function registering the module for activation next frame, by id
void Engine::activateModule(int moduleId) {
  if (moduleId < 0 || moduleId >= modules.size()) {
    logger::Log::warning("Engine::activateModule | Tried to activate an invalid module: ID %d.", moduleId);
    displayError();
    return;
  }
  module::Module* module = modules.at(moduleId);
  activateModule(module);
}

// public function registering an internal module for activation next frame, by id
void Engine::activateModule(InternalModuleID id) {
  if (id < 0 || id >= INTERNAL_MAX) {
    logger::Log::warning("Engine::activateModule | Tried to activate an invalid internal module: ID %d.", (int)id);
    displayError();
    return;
  }
  activateModule(internalModules[id]);
}

// public function registering the module for activation next frame, by reference
void Engine::activateModule(module::Module* module) {
  if (module != NULL && !module->getActive()) {
    toActivate.push_back(module);
    logger::Log::info("Engine::activateModule | Activated module \"%s\" (ID: %d).", module->getName(), module->getID());
  }
}

void Engine::activateModule(const char* name) {
  module::Module* mod = getModule(name);
  if (mod)
    activateModule(mod);
  else {
    logger::Log::warning(
        "Engine::activateModule | Tried to activate a module named \"%s\", but such a module was not found.", name);
    displayError();
  }
}

// the internal function actually putting a module in active list
void Engine::doActivateModule(module::Module* mod) {
  if (!mod->getActive()) {
    mod->setActive(true);
    mod->initialiseTimeout();
    // insert the module at the right pos, sorted by priority
    int idx = 0;
    while (idx < activeModules.size() && activeModules.at(idx)->getPriority() < mod->getPriority()) idx++;
    if (idx < activeModules.size())
      activeModules.insert(activeModules.begin() + idx, mod);
    else
      activeModules.push_back(mod);
  }
}

// register the module for deactivation by id
void Engine::deactivateModule(int moduleId) {
  if (moduleId < 0 || moduleId >= modules.size()) {
    logger::Log::warning("Engine::deactivateModule | Tried to deactivate an invalid module: ID %d.", moduleId);
    displayError();
  }
  module::Module* module = modules.at(moduleId);
  if (!module->getActive()) {
    logger::Log::notice(
        "Engine::deactivateModule | Tried to deactivate the module with an ID %d, but it's already inactive.",
        moduleId);
    displayError();
  } else
    deactivateModule(module);
}

void Engine::deactivateModule(InternalModuleID id) {
  if (id < 0 || id >= INTERNAL_MAX) {
    logger::Log::warning("Engine::deactivateModule | Tried to deactivate an invalid internal module: ID %d.", (int)id);
    displayError();
  } else if (!internalModules[id]->getActive()) {
    logger::Log::notice(
        "Engine::deactivateModule | Tried to deactivate an internal module with an ID %d, but it's already "
        "inactive.",
        (int)id);
    displayError();
  } else
    deactivateModule(internalModules[id]);
}

// register the module for deactivation by reference
void Engine::deactivateModule(module::Module* module) {
  if (module != NULL && module->getActive()) {
    toDeactivate.push_back(module);
    module->setActive(false);
    logger::Log::info(
        "Engine::deactivateModule | Deactivated \"%s\" module (ID: %d).", module->getName(), module->getID());
  } else if (module != NULL && !module->getActive()) {
    logger::Log::notice("Engine::deactivateModule | Tried to deactivate a module, but it's already inactive.");
    displayError();
  } else if (module == NULL) {
    logger::Log::warning(
        "Engine::deactivateModule | Tried to deactivate a module, but the module pointer given is null.");
    displayError();
  } else {
    logger::Log::warning(
        "Engine::deactivateModule | Tried to deactivate a module, but the module pointer given does not point to "
        "a module on the modules list.");
    displayError();
  }
}

void Engine::deactivateModule(const char* name) {
  module::Module* mod = getModule(name);
  if (mod) {
    if (mod->getActive())
      deactivateModule(mod);
    else {
      logger::Log::notice(
          "Engine::deactivateModule | Tried to deactivate a module named \"%s\", but it's already inactive.", name);
      displayError();
    }
  } else {
    logger::Log::warning(
        "Engine::deactivateModule | Tried to deactivate a module, but the module name \"%s\" was not found.", name);
    displayError();
  }
}

void Engine::deactivateAll(bool ignoreFallbacks) {
  logger::Log::openBlock(
      "Engine::deactivateAll | Deactivating all modules%s.", ignoreFallbacks ? ", ignoring fallbacks" : "");
  for (auto& mod : activeModules) {
    if (ignoreFallbacks) mod->setFallback(-1);
    deactivateModule(mod);
  }
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
}

bool Engine::initialise(TCOD_renderer_t new_renderer) {
  // autodetect fonts if needed
  bool retVal;
  logger::Log::openBlock("Engine::initialise | Initialising the root console.");
  retVal = registerFonts();
  // activate the base font
  if (retVal) {
    Engine::renderer = new_renderer;
    config::Config::activateFont();
    // initialise console
    TCODConsole::setCustomFont(
        config::Config::font->filename(),
        config::Config::font->flags(),
        config::Config::font->columns(),
        config::Config::font->rows());
    TCODConsole::initRoot(
        getRootWidth(), getRootHeight(), windowTitle.c_str(), config::Config::fullScreen, new_renderer);

    registerCustomCharacters();
    TCODSystem::setFps(25);
    TCODMouse::showCursor(true);
    if (TCODConsole::root != NULL)
      logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
    else
      logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
  } else
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
  return retVal;
}

int Engine::run() {
  TCOD_key_t key{};
  TCOD_mouse_t mouse{};

  logger::Log::openBlock("Engine::run | Running the engine.");

  if (modules.size() == 0) {
    logger::Log::fatalError("Engine::run | No modules have been registered!");
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return 1;
  }

  while (!TCODConsole::isWindowClosed()) {
    // execute only when paused
    if (paused) {
      if (keyboardMode >= KEYBOARD_SDL) {
        // Flush all SDL events via checkForEvent.
        while (TCODSystem::checkForEvent(TCOD_EVENT_KEY_RELEASE | TCOD_EVENT_MOUSE_PRESS, &key, &mouse)) {
          keyboard(key);
        }
      } else {
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_RELEASE | TCOD_EVENT_MOUSE_PRESS, &key, &mouse);
        keyboard(key);
      }
      TCODConsole::root->flush();
      continue;  // don't update or render anything anew
    }

    // deactivate modules
    for (auto& mod : toDeactivate) {
      mod->setActive(false);
      auto found = std::find(activeModules.begin(), activeModules.end(), mod);
      if (found != activeModules.end()) {
        activeModules.erase(found);
      } else {
        logger::Log::notice("Tried to deactive non active module: %s", mod->getName());
      }
    }
    toDeactivate.clear();

    // activate new modules
    while (toActivate.size()) {
      auto* mod = toActivate.back();
      toActivate.pop_back();
      doActivateModule(mod);
    }

    if (activeModules.size() == 0) break;  // exit game

    // update all active modules
    switch (keyboardMode) {
      case KEYBOARD_WAIT:
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse, true);
        break;
      case KEYBOARD_WAIT_NOFLUSH:
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse, false);
        break;
      case KEYBOARD_PRESSED:
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse);
        break;
      case KEYBOARD_PRESSED_RELEASED:
        TCODSystem::checkForEvent(TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &key, &mouse);
        break;
      case KEYBOARD_RELEASED:
      default:
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_RELEASE | TCOD_EVENT_MOUSE, &key, &mouse);
        break;
      case KEYBOARD_SDL:
        while (TCOD_event_t event_type = TCODSystem::checkForEvent(TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &key, &mouse)) {
          for (auto& module : activeModules) {
            if (module->getPause()) continue;
            if (event_type & TCOD_EVENT_KEY) module->keyboard(key);
            if (event_type & TCOD_EVENT_MOUSE) module->mouse(mouse);
          }
        }
        break;
    }
    keyboard(key);
    uint32_t startTime = SDL_GetTicks();
    // update all active modules by priority order
    activeModules.erase(
        std::remove_if(
            activeModules.begin(),
            activeModules.end(),
            [&](module::Module* tmpMod) {
              bool remove_this = false;
              if (!tmpMod->getPause()) {
                // handle input
                if (keyboardMode < KEYBOARD_SDL) {  // Old-style handling.
                  tmpMod->keyboard(key);
                  tmpMod->mouse(mouse);
                }
                if (tmpMod->isTimedOut(startTime) || !tmpMod->update() || !tmpMod->getActive()) {
                  module::Module* module = tmpMod;
                  int fallback = module->getFallback();
                  // deactivate module
                  module->setActive(false);
                  remove_this = true;
                  if (fallback != -1) {
                    // register fallback for activation
                    module::Module* fallbackModule = modules.at(fallback);
                    if (fallbackModule != NULL && !fallbackModule->getActive()) toActivate.push_back(fallbackModule);
                  }
                }
              }
              return remove_this;
            }),
        activeModules.end());
    uint32_t updateTime = SDL_GetTicks() - startTime;
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    TCODConsole::root->clear();
    // render active modules by inverted priority order
    for (auto module = activeModules.rbegin(); module != activeModules.rend(); ++module) {
      (*module)->render();
    }
    uint32_t renderTime = SDL_GetTicks() - startTime - updateTime;
    if (internalModules[INTERNAL_SPEEDOMETER]->getActive()) {
      ((imod::ModSpeed*)internalModules[INTERNAL_SPEEDOMETER])->setTimes(updateTime, renderTime);
    }
    // flush the screen
    TCODConsole::root->flush();
  }
  config::Config::save();
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
  logger::Log::save();
  return 0;
}

void Engine::keyboard(TCOD_key_t& key) {
  if (key.vk == TCODK_NONE || (keyboardMode != KEYBOARD_PRESSED && key.pressed)) return;

  base::Key k(key.vk, key.c, key.ralt | key.lalt, key.rctrl | key.lctrl, key.shift);

  bool val = false;

  for (auto& cbk : callbacks) {
    if (cbk->evaluate(k)) {
      cbk->action();
      val = true;
      break;
    }
  }

  if (val) {
    key = TCOD_key_t{};  // "erase" key event
  }
}

void Engine::reinitialise(TCOD_renderer_t new_renderer) {
  logger::Log::openBlock("Engine::reinitialise | Reinitialising the root console.");
  TCOD_console_delete(nullptr);
  TCODConsole::setCustomFont(
      config::Config::font->filename(),
      config::Config::font->flags(),
      config::Config::font->columns(),
      config::Config::font->rows());
  Engine::renderer = new_renderer;
  TCODConsole::initRoot(
      getRootWidth(), getRootHeight(), windowTitle.c_str(), config::Config::fullScreen, this->renderer);
  registerCustomCharacters();
  if (TCODConsole::root != NULL)
    logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
  else
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
}

void Engine::registerInternalModule(InternalModuleID id, module::Module* module) {
  logger::Log::info("Engine::registerInternalModule | Registering an internal module.");
  internalModules[id] = module;
}

void Engine::displayError() {
  if (TCODConsole::root != NULL) {
    if (internalModules[INTERNAL_BSOD]->getActive()) toDeactivate.push_back(internalModules[INTERNAL_BSOD]);
    toActivate.push_back(internalModules[INTERNAL_BSOD]);
  }
}

void Engine::printCredits(int x, int y, uint32_t duration) {
  ((imod::ModCredits*)internalModules[INTERNAL_CREDITS])->set(x, y, duration);
  activateModule(internalModules[INTERNAL_CREDITS]);
}

void Engine::addCustomCharacter(int x, int y, int code) { this->customChars.push_back(CustomCharMap{x, y, code}); }

void Engine::registerCustomCharacters() {
  for (size_t i = 0; i < this->customChars.size(); ++i) {
    CustomCharMap& tmp = this->customChars.at(i);
    TCODConsole::root->mapAsciiCodeToFont(tmp.code, tmp.x, tmp.y);
  }

  logger::Log::info("Engine::registerCustomCharacters | Custom character mappings registered.");
}
}  // namespace engine

#include "imod/credits.hpp"

#include <SDL_timer.h>

#include <libtcod/libtcod.hpp>

#include "version.hpp"

namespace imod {
ModCredits::ModCredits() {
  coords.set(0, 0);
  con = new TCODConsole(40, 1);
}

void ModCredits::onActivate() { startTime = SDL_GetTicks(); }

bool ModCredits::update() {
  alpha = 2.0f - (float)(SDL_GetTicks() - startTime) / (float)duration;
  alpha = MIN(1.0f, alpha);
  if (alpha >= 0.0f)
    return true;
  else
    return false;
}

void ModCredits::render() {
  static const char* str = "Powered by " SALIENT_TITLE " " SALIENT_VERSION " " SALIENT_STATUS;
  con->setDefaultForeground(TCODColor(250, 250, 220));
  con->printEx(0, 0, TCOD_BKGND_NONE, TCOD_LEFT, str);
  TCODConsole::blit(con, 0, 0, static_cast<int>(strlen(str)), 1, TCODConsole::root, coords.x, coords.y, alpha, 0.0f);
}

void ModCredits::set(int x, int y, uint32_t d) {
  duration = d / 2;
  coords.set(x, y);
}
}  // namespace imod

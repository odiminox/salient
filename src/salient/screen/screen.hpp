#pragma once
#include <libtcod/libtcod.hpp>

namespace screen {
class Screen : public Module {
 public:
  Screen(float fadeLvl) : Module{}, fadeLvl{fadeLvl} {}
  Screen(const char* name, float fadeLvl) : UmbraModule{name}, fadeLvl{fadeLvl} {}
  void render() override = 0;
  virtual bool update(float elapsed, TCOD_key_t k, TCOD_mouse_t mouse) = 0;
  void onEvent(const SDL_Event&) override;
  // void keyboard(TCOD_key_t& key) override { key_ = key; }
  //  void mouse(TCOD_mouse_t& ms) override { ms_ = ms; }
  bool update(void) override;

  void setFadeIn(int lengthInMilli, TCODColor col = TCODColor::black);  // set fade lengths in milliseconds
  void setFadeOut(int lengthInMilli, TCODColor col = TCODColor::black);  // set fade lengths in milliseconds

 protected:
  float timefix{};  // remove activate execution time for smooth animation
  float fadeLvl{};
  int fadeInLength{};
  TCODColor fadeInColor{};
  int fadeOutLength{};
  TCODColor fadeOutColor{};

  enum { FADE_UP, FADE_DOWN, FADE_OFF, FADE_NONE } fade{FADE_UP};
  TCOD_key_t key_{};
  TCOD_mouse_t ms_{};
  TCODImage* loadChapterPicture(bool big = false);
  void onActivate() override {
    fadeLvl = 0;
    fade = FADE_UP;
  }
};
}  // namespace screen

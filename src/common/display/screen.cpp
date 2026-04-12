#include "common/display/screen.h"
#include <ncurses.h>

void Screen::print(bool flushNow) const {
  for (size_t i = 0; i < scr.size(); ++i) {
    for (size_t j = 0; j < scr[i].size(); ++j) {
      move(static_cast<int>(i), static_cast<int>(j));
      scr[i][j].print();
    }
  }
  if (flushNow)
    refresh();
}

void Screen::set(const ColorChar &value) {
  for (auto &rol : scr) {
    std::fill(rol.begin(), rol.end(), value);
  }
}
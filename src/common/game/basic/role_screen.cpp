#include "common/game/basic/role_screen.h"
#include "common/display/color_char.h"
#include <ncurses.h>

RoleScreen::RoleScreen(size_t n, size_t m, const Element &init) : scr(n, vE(m, init)) {}

size_t RoleScreen::size() const { return x_size() * y_size(); }

RoleScreen::vvE &RoleScreen::data() { return scr; }

const RoleScreen::vvE &RoleScreen::data() const { return scr; }

size_t RoleScreen::x_size() const { return scr.size(); }

size_t RoleScreen::y_size() const {
  if (scr.empty())
    return 0;
  return scr[0].size();
}

RoleScreen::vE &RoleScreen::operator[](int idx) { return scr[idx]; }

const RoleScreen::vE &RoleScreen::operator[](int idx) const { return scr[idx]; }

RoleScreen::vE &RoleScreen::at(int idx) { return scr.at(idx); }

const RoleScreen::vE &RoleScreen::at(int idx) const { return scr.at(idx); }

RoleScreen::Element &RoleScreen::at(int x, int y) { return scr.at(x).at(y); }

const RoleScreen::Element &RoleScreen::at(int x, int y) const { return scr.at(x).at(y); }

RoleScreen::Element &RoleScreen::at(const std::pair<int, int> &xy) { return scr.at(xy.first).at(xy.second); }

const RoleScreen::Element &RoleScreen::at(const std::pair<int, int> &xy) const {
  return scr.at(xy.first).at(xy.second);
}

void RoleScreen::set(const Element &value) {
  for (auto &row : scr) {
    for (auto &elem : row) {
      elem = value;
    }
  }
}

void RoleScreen::clear() {
  for (auto &row : scr) {
    for (auto &elem : row) {
      elem.first = ColorChar();
      elem.second = nullptr;
    }
  }
}

void RoleScreen::print(bool flushNow) const {
  for (size_t i = 0; i < scr.size(); ++i) {
    for (size_t j = 0; j < scr[i].size(); ++j) {
      move(static_cast<int>(i), static_cast<int>(j));
      scr[i][j].first.print();
    }
  }
  if (flushNow) {
    refresh();
  }
}
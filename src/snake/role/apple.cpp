#include "snake/role/apple.h"
#include "common/display/color_char.h"
#include "common/display/pixel.h"
#include "common/maths/random.hpp"
#include <cstdlib>
#include <ncurses.h>
Apple::Apple(RoleScreen &scr_) : Role(scr_) { moveRandom(); }

void Apple::moveRandom() {
  try {
    if (scr.at(now).second == this) {
      scr.at(now).second = nullptr;
      scr.at(now).first.setBg(Color::WHITE);
    }
  } catch (...) {
    mvprintw(LINES - 1, 0, "未知的错误，转移苹果失败");
    refresh();
    std::exit(0);
  }
  size_t cnt = 0;
  do {
    now = {random(static_cast<size_t>(0), scr.x_size() - 1), random(static_cast<size_t>(0), scr.y_size() - 1)};
  } while (++cnt <= scr.size() && scr.at(now).first.getBg() != Color::WHITE);
  if (cnt > scr.size()) {
    mvprintw(LINES - 1, 0, "没有多余的空间转移苹果，游戏结束");
    refresh();
    throw std::runtime_error("End-no space");
  }
  scr.at(now).first.setBg(Color::RED);
  scr.at(now).second = this;
}

const Apple::xy &Apple::where() const { return now; }

void Apple::kill() { moveRandom(); }
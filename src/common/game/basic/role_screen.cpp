#include "common/game/basic/role_screen.h"
#include "common/display/color_char.h"
#include <ncurses.h>

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

/**
 * @brief 打印整个角色屏幕到ncurses
 *
 * 遍历屏幕上的所有元素并渲染ColorChar部分。
 *
 * @param flushNow 是否立即刷新显示，默认为true
 */
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

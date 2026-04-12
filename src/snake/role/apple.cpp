#include "snake/role/apple.h"
#include "common/display/color_char.h"
#include "common/display/pixel.h"
#include "common/maths/random.hpp"
#include <cstdlib>
#include <ncurses.h>

/**
 * @brief Apple构造函数
 *
 * 创建苹果并立即随机放置到屏幕上的空白位置。
 *
 * @param scr_ 绑定的角色屏幕
 */
Apple::Apple(RoleScreen &scr_) : Role(scr_) { moveRandom(); }

/**
 * @brief 随机移动到新位置
 *
 * 在屏幕上寻找一个空白位置（背景色为WHITE的格子）并移动过去。
 * 清除旧位置的标记，在新位置设置红色背景和角色指针。
 * 如果没有可用空间，抛出std::runtime_error异常。
 */
void Apple::moveRandom() {
  try {
    // 清除旧位置
    if (scr.at(now).second == this) {
      scr.at(now).second = nullptr;
      scr.at(now).first.setBg(Color::WHITE);
    }
  } catch (...) {
    mvprintw(LINES - 1, 0, "未知的错误，转移苹果失败");
    refresh();
    std::exit(0);
  }

  // 寻找新的空白位置
  size_t cnt = 0;
  do {
    now = {random(static_cast<size_t>(0), scr.x_size() - 1), random(static_cast<size_t>(0), scr.y_size() - 1)};
  } while (++cnt <= scr.size() && scr.at(now).first.getBg() != Color::WHITE);

  if (cnt > scr.size()) {
    mvprintw(LINES - 1, 0, "没有多余的空间转移苹果，游戏结束");
    refresh();
    throw std::runtime_error("End-no space");
  }

  // 设置新位置
  scr.at(now).first.setBg(Color::RED);
  scr.at(now).second = this;
}

/**
 * @brief 返回苹果当前位置
 *
 * @return const xy& 苹果的坐标引用
 */
const Apple::xy &Apple::where() const { return now; }

/**
 * @brief 使苹果死亡（重新随机位置）
 *
 * 调用moveRandom()将苹果移动到新的随机位置。
 */
void Apple::kill() { moveRandom(); }

/**
 * @brief 获取角色类型
 *
 * @return std::string 返回 "Apple"
 */
std::string Apple::type() { return "Apple"; }

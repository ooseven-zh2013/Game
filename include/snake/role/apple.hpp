/**
 * @file apple.hpp
 * @brief 苹果角色类
 *
 * Apple是游戏中的食物元素，蛇吃到后会增长。
 * 苹果会在屏幕上随机生成在空白位置。
 */

#ifndef APPLE_HPP
#define APPLE_HPP
#pragma once

#include "common/display/color_char.hpp"
#include "common/display/pixel.hpp"
#include "common/game/basic/role.hpp"
#include "common/maths/random.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <string>

/**
 * @brief 苹果角色类
 *
 * Apple继承自Role，代表游戏中的食物元素。
 * 当蛇吃到苹果时，苹果会重新随机出现在屏幕的空白位置。
 * 苹果显示为红色背景的空格字符。
 */
class Apple : public Role {
public:
  using xy = std::pair<int, int>;

  /**
   * @brief 创建一个苹果
   *
   * @param scr_ 屏幕
   */
  inline Apple(RoleScreen &scr_);

  /**
   * @brief 返回当前位置
   *
   * @return const xy& 位置
   */
  const xy &where() const;

  /**
   * @brief 使苹果死亡（重新随机位置）
   *
   * 调用moveRandom()将苹果移动到新的随机位置。
   */
  void kill() override;

  /**
   * @brief 获取角色类型
   *
   * @return std::string 返回 "Apple"
   */
  std::string type() override;

private:
  xy now; ///< 苹果当前位置

  /**
   * @brief 随机移动到新位置
   *
   * 在屏幕上寻找一个空白位置（背景色为WHITE的格子）并移动过去。
   * 如果没有可用空间，抛出std::runtime_error异常。
   */
  void moveRandom();
};

// Implementation from apple.cpp
inline Apple::Apple(RoleScreen &scr_) : Role(scr_) { moveRandom(); }

inline const Apple::xy &Apple::where() const { return now; }

inline void Apple::kill() { moveRandom(); }

inline std::string Apple::type() { return "Apple"; }

inline void Apple::moveRandom() {
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

#endif // APPLE_HPP

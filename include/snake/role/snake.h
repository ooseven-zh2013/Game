#ifndef SNAKE_H
#define SNAKE_H
#pragma once

#include "common/game/basic/role.h"
#include <deque>

class Snake : Role {
public:
  using xy = std::pair<int, int>;

  /**
   * @brief 创建一条蛇
   *
   * @param scr_ 屏幕
   */
  Snake(RoleScreen &scr_, bool isPlayer_ = false);

  /**
   * @brief 让蛇死亡
   *
   */
  void kill() override;

  /**
   * @brief 更新蛇的状态
   *
   */
  void update();

private:
  bool isPlayer;
  bool isDead;
  std::deque<xy> body;
  unsigned char dir;
  static constexpr xy dirXy[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
};

#endif // SNAKE_H
#ifndef APPLE_H
#define APPLE_H
#pragma once

#include "common/game/basic/role.h"

class Apple : public Role {
public:
  using xy = std::pair<int, int>;

  /**
   * @brief 创建一个苹果
   *
   * @param scr_ 屏幕
   */
  Apple(RoleScreen &scr_);

  /**
   * @brief 返回当前位置
   *
   * @return const xy& 位置
   */
  const xy &where() const;

private:
  xy now;

  /**
   * @brief 随机移动
   *
   */
  void moveRandom();
};

#endif // APPLE_H
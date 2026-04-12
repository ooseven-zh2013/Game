#ifndef APPLE_H
#define APPLE_H
#pragma once

#include "common/game/basic/role.h"
#include <string>

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

#endif // APPLE_H
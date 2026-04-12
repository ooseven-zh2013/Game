#ifndef ROLE_H
#define ROLE_H
#pragma once

#include "common/game/basic/role_screen.h"
#include <string>

/**
 * @brief 游戏角色基类
 *
 * Role是所有游戏角色的抽象基类，定义了角色的基本接口。
 * 每个角色都绑定到一个RoleScreen网格上，并可以实现自己的行为和渲染逻辑。
 */
class Role {
public:
  virtual ~Role() = default;

  /**
   * @brief 使角色死亡
   *
   * 纯虚函数，派生类必须实现此方法来定义角色的死亡行为。
   * 通常用于清理资源、更新屏幕显示等。
   */
  virtual void kill() = 0;

  /**
   * @brief 获取角色类型字符串
   *
   * 纯虚函数，派生类必须实现此方法来返回角色的类型标识。
   *
   * @return std::string 角色类型的字符串表示（如 "Apple", "Snake"）
   */
  virtual std::string type() = 0;

protected:
  RoleScreen &scr; ///< 绑定的角色屏幕引用

  /**
   * @brief 创建一个绑定了网格的角色
   *
   * @param scr_ 绑定的角色屏幕引用
   */
  Role(RoleScreen &scr_);

private:
};

#endif // ROLE_H
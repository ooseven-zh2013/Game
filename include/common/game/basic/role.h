#ifndef ROLE_H
#define ROLE_H
#pragma once

#include "common/game/basic/role_screen.h"

class Role {
public:
  virtual void kill() = 0;

protected:
  RoleScreen &scr;

  /**
   * @brief 创建一个绑定了网格的角色
   *
   * @param scr_ 绑定的网格
   */
  Role(RoleScreen &scr_);

private:
};

#endif // ROLE_H
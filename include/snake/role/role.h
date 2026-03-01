#ifndef ROLE_H
#define ROLE_H
#pragma once

#include "common/display/screen.h"
class Role {
public:
protected:
  Screen &scr;

  /**
   * @brief 创建一个绑定了网格的角色
   * 
   * @param scr_ 绑定的网格
   */
  Role(Screen &scr_);

private:
};

#endif //ROLE_H
#include "snake/role/snake.h"
#include "common/maths/random.hpp"
Snake::Snake(RoleScreen &scr_, bool isPlayer_) : Role(scr_), isPlayer(isPlayer_), isDead(false) {
  // 调用random函数随机生成蛇的位置和方向
  int x, y;
  x = random(0, static_cast<int>(scr.x_size()) - 1);
  y = random(0, static_cast<int>(scr.y_size()) - 1);
  dir = random(0, 3);
  while (scr[x][y].first.getBg() != Color::WHITE) {
    x = random(0, static_cast<int>(scr.x_size()) - 1);
    y = random(0, static_cast<int>(scr.y_size()) - 1);
  }
  body.emplace_back(x, y);
  scr[x][y].first.setBg(isPlayer_ ? Color::BLUE : Color::GREEN);
}

void Snake::kill() {
  isDead = true;
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(Color::BRIGHT_BLACK);
  }
}

void Snake::update() {
  if (isDead) {
    return;
  }

  // TODO: 处理蛇的移动逻辑
  // 区分 AI 移动和玩家移动（根据 isPlayer 变量）
  // if (isPlayer) {
  //   // 玩家控制移动
  // } else {
  //   // AI 控制移动
  // }

  // 更新蛇在屏幕上的显示
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(isPlayer ? Color::BLUE : Color::GREEN);
  }
}
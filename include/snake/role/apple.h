#ifndef APPLE_H
#define APPLE_H
#pragma once

#include "snake/role/role.h"

class Apple : Role {
public:
  using xy = std::pair<size_t, size_t>;
  Apple(Screen &scr_);
  const xy &where() const;

private:
  xy now;
  void moveRandom();
};

#endif // APPLE_H
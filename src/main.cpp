#include "snake/role/apple.h"
#include <cstdlib>
#include <iostream>
signed main() {
  RoleScreen scr(20, 20, std::make_pair(ColorChar(' ', Color::BLUE, Color::WHITE), (Role *)(nullptr)));
  Apple a(scr);
  for (int i = 1; i <= 20; i++) {
    system("clear");
    a.kill();
    scr.print();
    std::cin.get();
  }
}
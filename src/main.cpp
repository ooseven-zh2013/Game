#include "snake/role/apple.h"
signed main() {
  RoleScreen scr(20, 20, std::make_pair(ColorChar(' ', Color::BLUE, Color::WHITE), (Role *)(nullptr)));
  Apple a(scr);
}
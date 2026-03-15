#include "snake/role/apple.h"
#include "common/display/color_char.h"
#include "common/display/pixel.h"
#include "common/maths/random.hpp"
Apple::Apple(RoleScreen &scr_) : Role(scr_) { moveRandom(); }

void Apple::moveRandom() {
  do {
    now = {random(static_cast<size_t>(0), scr.x_size() - 1), random(static_cast<size_t>(0), scr.y_size()) - 1};
  } while (scr.at(now).first.getBg() != Color::WHITE);
}

const Apple::xy &Apple::where() const { return now; }
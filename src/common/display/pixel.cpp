#include "common/display/pixel.h"
#include <ncurses.h>

void Pixel::print() const {
  attron(COLOR_PAIR(static_cast<int>(m_background) + 1));
  addch(' ');
  attroff(COLOR_PAIR(static_cast<int>(m_background) + 1));
}
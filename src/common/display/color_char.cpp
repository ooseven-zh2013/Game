#include "common/display/color_char.h"
#include <ncurses.h>

void ColorChar::print() const {
  attron(COLOR_PAIR(static_cast<int>(m_background) + 1));
  addch(static_cast<char>(m_character));
  attroff(COLOR_PAIR(static_cast<int>(m_background) + 1));
}
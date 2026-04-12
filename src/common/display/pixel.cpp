#include "common/display/pixel.h"
#include <ncurses.h>

Pixel::Pixel(Color bg) : m_background(bg) {}
Color Pixel::getBg() const { return m_background; }

void Pixel::setBg(Color bg) { m_background = bg; }

void Pixel::print() const {
  attron(COLOR_PAIR(static_cast<int>(m_background) + 1));
  addch(' ');
  attroff(COLOR_PAIR(static_cast<int>(m_background) + 1));
}
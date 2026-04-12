#include "common/display/color_char.h"
#include <ncurses.h>

ColorChar::ColorChar(wchar_t ch, Color fg, Color bg) : Pixel(bg), m_character(ch), m_foreground(fg) {}
Color ColorChar::getFg() const { return m_foreground; }

void ColorChar::setFg(Color foreground) { m_foreground = foreground; }

wchar_t ColorChar::getCh() const { return m_character; }

void ColorChar::setCh(wchar_t ch) { m_character = ch; }

void ColorChar::print() const {
  attron(COLOR_PAIR(static_cast<int>(m_background) + 1));
  addch(static_cast<char>(m_character));
  attroff(COLOR_PAIR(static_cast<int>(m_background) + 1));
}
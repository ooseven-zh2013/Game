#include "common/display/color_char.h"

using namespace display;

ColorChar::ColorChar(char ch, Color fg, Color bg) : Pixel(bg), m_character(ch), m_foreground(fg) {}
Color ColorChar::getFg() const { return m_foreground; }

void ColorChar::setFg(Color foreground) { m_foreground = foreground; }

char ColorChar::getCh() const { return m_character; }

void ColorChar::setCh(char ch) { m_character = ch; }
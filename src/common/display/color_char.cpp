#include "common/display/color_char.h"
#include <iostream>

ColorChar::ColorChar(wchar_t ch, Color fg, Color bg) : Pixel(bg), m_character(ch), m_foreground(fg) {}
Color ColorChar::getFg() const { return m_foreground; }

void ColorChar::setFg(Color foreground) { m_foreground = foreground; }

wchar_t ColorChar::getCh() const { return m_character; }

void ColorChar::setCh(wchar_t ch) { m_character = ch; }

void ColorChar::print() const {
  std::wcout << L"\033[" << (static_cast<int>(m_foreground) + 30) << L';' // 前景色码 (30-37, 39 for default)
             << (static_cast<int>(m_background) + 40) << L'm'             // 背景色码 (40-47, 49 for default)
             << m_character                                               // 打印字符
             << L"\033[0m";                                               // 恢复默认颜色
}
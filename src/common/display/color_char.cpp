#include "common/display/color_char.h"
#include <iostream>

// 注意：此处简化了颜色映射，仅适用于标准ANSI颜色。
// 实际应用中可能需要处理 DEFAULT (9) 映射到 39/49 的情况。

ColorChar::ColorChar(char32_t c, ColorChar::Color fg, ColorChar::Color bg)
    : m_char(c), m_fg_color(fg), m_bg_color(bg) {}

char32_t &ColorChar::getChar() { return m_char; }
const char32_t &ColorChar::getChar() const { return m_char; }

ColorChar::ColorChar::Color &ColorChar::getFColor() { return m_fg_color; }
const ColorChar::Color &ColorChar::getFColor() const { return m_fg_color; }

ColorChar::ColorChar::Color &ColorChar::getBColor() { return m_bg_color; }
const ColorChar::Color &ColorChar::getBColor() const { return m_bg_color; }

void ColorChar::setChar(char32_t c) { m_char = c; }
void ColorChar::setFColor(ColorChar::Color color) { m_fg_color = color; }
void ColorChar::setBColor(ColorChar::Color color) { m_bg_color = color; }

bool ColorChar::operator==(const ColorChar &other) const {
  return m_char == other.m_char && m_fg_color == other.m_fg_color && m_bg_color == other.m_bg_color;
}

void ColorChar::print() const {
  std::wcout << "\033[" << (static_cast<int>(m_fg_color) + 30) << ';' // 前景色码 (30-37, 39 for default)
             << (static_cast<int>(m_bg_color) + 40) << 'm'            // 背景色码 (40-47, 49 for default)
             << static_cast<wchar_t>(m_char)                          // 打印字符
             << "\033[0m";                                            // 重置颜色
}
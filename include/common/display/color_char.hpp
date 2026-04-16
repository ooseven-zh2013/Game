/**
 * @file color_char.hpp
 * @brief 带颜色的字符类
 *
 * ColorChar继承自Pixel，扩展了前景色和字符内容，
 * 是游戏中最基本的可显示单元。
 */

#ifndef COLOR_CHAR_HPP
#define COLOR_CHAR_HPP
#pragma once

#include "pixel.hpp"
#include <ncurses.h>

/**
 * @brief 表示带颜色的字符类
 *
 * ColorChar类表示一个具有前景色、背景色和字符的单元格，
 * 它继承自Pixel类并扩展了字符和前景色功能。
 * 用于在屏幕上渲染单个字符及其颜色属性。
 */
class ColorChar : public Pixel {
public:
  /**
   * @brief 带字符的构造函数
   *
   * @param ch 字符，默认为空格
   * @param fg 前景色，默认为Color::DEFAULT
   * @param bg 背景色，默认为Color::DEFAULT
   */
  inline ColorChar(wchar_t ch = ' ', Color fg = Color::DEFAULT, Color bg = Color::DEFAULT);

  /**
   * @brief 获取前景色
   *
   * @return Color 当前前景色
   */
  inline Color getFg() const;

  /**
   * @brief 设置前景色
   *
   * @param fg 新的前景色
   */
  inline void setFg(Color fg);

  /**
   * @brief 获取字符
   *
   * @return wchar_t 当前字符
   */
  inline wchar_t getCh() const;

  /**
   * @brief 设置字符
   *
   * @param ch 新的字符
   */
  inline void setCh(wchar_t ch);

  /**
   * @brief 打印字符到ncurses屏幕
   *
   * 使用前景色和背景色渲染字符。
   */
  void print() const override;

private:
  wchar_t m_character; ///< 字符
  Color m_foreground;  ///< 前景色
};

// Inline implementations
inline ColorChar::ColorChar(wchar_t ch, Color fg, Color bg) : Pixel(bg), m_character(ch), m_foreground(fg) {}
inline Color ColorChar::getFg() const { return m_foreground; }
inline void ColorChar::setFg(Color foreground) { m_foreground = foreground; }
inline wchar_t ColorChar::getCh() const { return m_character; }
inline void ColorChar::setCh(wchar_t ch) { m_character = ch; }

// Implementation from color_char.cpp
inline void ColorChar::print() const {
  attron(COLOR_PAIR(static_cast<int>(m_background) + 1));
  addch(static_cast<char>(m_character));
  attroff(COLOR_PAIR(static_cast<int>(m_background) + 1));
}

#endif // COLOR_CHAR_HPP

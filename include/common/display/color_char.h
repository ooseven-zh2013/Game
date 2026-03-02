#ifndef COLOR_CHAR_H
#define COLOR_CHAR_H
#pragma once

#include "pixel.h"

/**
 * @brief 表示带颜色的字符类
 *
 * ColorChar类表示一个具有前景色、背景色和字符的单元格，
 * 它继承自Pixel类并扩展了字符和前景色功能。
 */
class ColorChar : public Pixel {
public:
  /**
   * @brief 带字符的构造函数
   *
   * @param ch 字符
   * @param fg 前景色，默认为Color::DEFAULT
   * @param bg 背景色，默认为Color::DEFAULT
   */
  ColorChar(wchar_t ch = ' ', Color fg = Color::DEFAULT, Color bg = Color::DEFAULT);

  /**
   * @brief 获取前景色
   *
   * @return Color 当前前景色
   */
  Color getFg() const;

  /**
   * @brief 设置前景色
   *
   * @param fg 新的前景色
   */
  void setFg(Color fg);

  /**
   * @brief 获取字符
   *
   * @return wchar_t 当前字符
   */
  wchar_t getCh() const;

  /**
   * @brief 设置字符
   *
   * @param ch 新的字符
   */
  void setCh(wchar_t ch);

  /**
   * @brief 打印字符
   *
   */
  void print() const override;

private:
  wchar_t m_character; ///< 字符
  Color m_foreground;  ///< 前景色
};

#endif // COLOR_CHAR_H
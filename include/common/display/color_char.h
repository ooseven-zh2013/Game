#ifndef COLOR_CHAR_H
#define COLOR_CHAR_H
#pragma once

/**
 * @file ColorChar.h
 * @brief 定义 ColorChar 类，用于处理带有颜色属性的单个宽字符。
 */

#include <cstdint>

/**
 * @brief 实现彩色宽字符的输出，并且有且仅有这个字符是此色调，后恢复正常颜色（如果没有遇到别的对象）。
 */
class ColorChar {
public:
  /**
   * @brief 颜色枚举，前景背景通用，大写字母。
   */
  enum class Color : std::uint8_t {
    BLACK = 0,   ///< 黑色
    RED = 1,     ///< 红色
    GREEN = 2,   ///< 绿色
    YELLOW = 3,  ///< 黄色
    BLUE = 4,    ///< 蓝色
    MAGENTA = 5, ///< 品红
    CYAN = 6,    ///< 青色
    WHITE = 7,   ///< 白色
    DEFAULT = 9  ///< 默认颜色
  };

private:
  char32_t m_char;  ///< 宽字符
  Color m_fg_color; ///< 前景色
  Color m_bg_color; ///< 背景色

public:
  /**
   * @brief 构造函数。
   * @param c 要显示的字符，默认为空格。
   * @param fg 前景色，默认为 DEFAULT。
   * @param bg 背景色，默认为 DEFAULT。
   */
  explicit ColorChar(char32_t c = U' ', Color fg = Color::DEFAULT, Color bg = Color::DEFAULT);

  /**
   * @brief 获取字符的非 const 引用。
   * @return 返回字符的引用。
   */
  char32_t &getChar();

  /**
   * @brief 获取字符的 const 引用。
   * @return 返回字符的 const 引用。
   */
  const char32_t &getChar() const;

  /**
   * @brief 获取前景色的非 const 引用。
   * @return 返回前景色的引用。
   */
  Color &getFColor();

  /**
   * @brief 获取前景色的 const 引用。
   * @return 返回前景色的 const 引用。
   */
  const Color &getFColor() const;

  /**
   * @brief 获取背景色的非 const 引用。
   * @return 返回背景色的引用。
   */
  Color &getBColor();

  /**
   * @brief 获取背景色的 const 引用。
   * @return 返回背景色的 const 引用。
   */
  const Color &getBColor() const;

  /**
   * @brief 设置字符。
   * @param c 新的字符。
   */
  void setChar(char32_t c);

  /**
   * @brief 设置前景色。
   * @param color 新的前景色。
   */
  void setFColor(Color color);

  /**
   * @brief 设置背景色。
   * @param color 新的背景色。
   */
  void setBColor(Color color);

  /**
   * @brief 比较操作符。
   * @param other 要比较的对象。
   * @return 如果字符、前景色和背景色都相等，则返回 true；否则返回 false。
   */
  bool operator==(const ColorChar &other) const;

  /**
   * @brief 输出字符的函数。
   *        该函数会打印带颜色的字符，并在之后重置为默认颜色。
   */
  void print() const;
};

#endif // COLOR_CHAR_H
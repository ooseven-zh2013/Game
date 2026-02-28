#ifndef COLOR_CHAR_HPP
#define COLOR_CHAR_HPP
#pragma once

/**
 * @file ColorChar.hpp
 * @brief 定义 ColorChar 类，用于处理带有颜色属性的单个宽字符。
 */

#include <iostream>

/**
 * @brief 实现彩色宽字符的输出，并且有且仅有这个字符是此色调，后恢复正常颜色（如果没有遇到别的对象）。
 */
class ColorChar {
public:
  /**
   * @brief 颜色枚举，前景背景通用，大写字母。
   */
  enum class Color : int {
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
  explicit ColorChar(char32_t c = U' ', Color fg = Color::DEFAULT, Color bg = Color::DEFAULT)
      : m_char(c), m_fg_color(fg), m_bg_color(bg) {}

  /**
   * @brief 获取字符的非 const 引用。
   * @return 返回字符的引用。
   */
  char32_t &getChar() { return m_char; }

  /**
   * @brief 获取字符的 const 引用。
   * @return 返回字符的 const 引用。
   */
  const char32_t &getChar() const { return m_char; }

  /**
   * @brief 获取前景色的非 const 引用。
   * @return 返回前景色的引用。
   */
  Color &getFColor() { return m_fg_color; }

  /**
   * @brief 获取前景色的 const 引用。
   * @return 返回前景色的 const 引用。
   */
  const Color &getFColor() const { return m_fg_color; }

  /**
   * @brief 获取背景色的非 const 引用。
   * @return 返回背景色的引用。
   */
  Color &getBColor() { return m_bg_color; }

  /**
   * @brief 获取背景色的 const 引用。
   * @return 返回背景色的 const 引用。
   */
  const Color &getBColor() const { return m_bg_color; }

  /**
   * @brief 设置字符。
   * @param c 新的字符。
   */
  void setChar(char32_t c) { m_char = c; }

  /**
   * @brief 设置前景色。
   * @param color 新的前景色。
   */
  void setFColor(Color color) { m_fg_color = color; }

  /**
   * @brief 设置背景色。
   * @param color 新的背景色。
   */
  void setBColor(Color color) { m_bg_color = color; }

  /**
   * @brief 比较操作符。
   * @param other 要比较的对象。
   * @return 如果字符、前景色和背景色都相等，则返回 true；否则返回 false。
   */
  bool operator==(const ColorChar &other) const {
    return m_char == other.m_char && m_fg_color == other.m_fg_color && m_bg_color == other.m_bg_color;
  }

  /**
   * @brief 输出字符的函数。
   *        该函数会打印带颜色的字符，并在之后重置为默认颜色。
   */
  void print() const {
    // 注意：此处简化了颜色映射，仅适用于标准ANSI颜色。
    // 实际应用中可能需要处理 DEFAULT (9) 映射到 39/49 的情况。
    std::cout << "\033[" << (static_cast<int>(m_fg_color) + 30) << ';' // 前景色码 (30-37, 39 for default)
              << (static_cast<int>(m_bg_color) + 40) << 'm'            // 背景色码 (40-47, 49 for default)
              << static_cast<char>(m_char)                             // 打印字符
              << "\033[0m";                                            // 重置颜色
  }
};

#endif // COLOR_CHAR_HPP
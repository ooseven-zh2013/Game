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
   * @brief 拷贝赋值操作符。
   * @param other 要拷贝的对象。
   * @return 返回自身的引用。
   */
  ColorChar &operator=(const ColorChar &other);

  /**
   * @brief 移动赋值操作符。
   * @param other 要移动的对象。
   * @return 返回自身的引用。
   */
  ColorChar &operator=(ColorChar &&other) noexcept;

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

// --- 成员函数定义 ---

inline ColorChar::ColorChar(char32_t c, Color fg, Color bg) : m_char(c), m_fg_color(fg), m_bg_color(bg) {}

inline char32_t &ColorChar::getChar() { return m_char; }
inline const char32_t &ColorChar::getChar() const { return m_char; }

inline ColorChar::Color &ColorChar::getFColor() { return m_fg_color; }
inline const ColorChar::Color &ColorChar::getFColor() const { return m_fg_color; }

inline ColorChar::Color &ColorChar::getBColor() { return m_bg_color; }
inline const ColorChar::Color &ColorChar::getBColor() const { return m_bg_color; }

inline void ColorChar::setChar(char32_t c) { m_char = c; }
inline void ColorChar::setFColor(Color color) { m_fg_color = color; }
inline void ColorChar::setBColor(Color color) { m_bg_color = color; }

inline ColorChar &ColorChar::operator=(const ColorChar &other) {
  if (this != &other) {
    m_char = other.m_char;
    m_fg_color = other.m_fg_color;
    m_bg_color = other.m_bg_color;
  }
  return *this;
}

inline ColorChar &ColorChar::operator=(ColorChar &&other) noexcept {
  if (this != &other) {
    m_char = other.m_char;
    m_fg_color = other.m_fg_color;
    m_bg_color = other.m_bg_color;
    // 将被移动的对象设置为默认状态
    other.m_char = U' ';
    other.m_fg_color = Color::DEFAULT;
    other.m_bg_color = Color::DEFAULT;
  }
  return *this;
}

inline bool ColorChar::operator==(const ColorChar &other) const {
  return m_char == other.m_char && m_fg_color == other.m_fg_color && m_bg_color == other.m_bg_color;
}

inline void ColorChar::print() const {
  std::cout << "\033[" << (static_cast<int>(m_fg_color) + 30) << ';' // 前景色码 (30-37, 39 for default)
            << (static_cast<int>(m_bg_color) + 40) << 'm'            // 背景色码 (40-47, 49 for default)
            << static_cast<char>(m_char)                             // 打印字符
            << "\033[0m";                                            // 重置颜色
}

#endif // COLOR_CHAR_HPP
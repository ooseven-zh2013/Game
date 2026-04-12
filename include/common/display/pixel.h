#ifndef PIXEL_H
#define PIXEL_H
#pragma once

#include <cstdint>

/**
 * @brief 颜色枚举类
 *
 * 定义了可用的颜色选项，包括默认色、基本色和亮色
 */
enum class Color : uint8_t {
  DEFAULT = 0,    ///< 默认颜色
  BLACK = 0,      ///< 黑色
  RED,            ///< 红色  (1)
  GREEN,          ///< 绿色  (2)
  YELLOW,         ///< 黄色  (3)
  BLUE,           ///< 蓝色  (4)
  MAGENTA,        ///< 品红色 (5)
  CYAN,           ///< 青色  (6)
  WHITE,          ///< 白色  (7)
  BRIGHT_BLACK,   ///< 亮黑色(深灰色) (8)
  BRIGHT_RED,     ///< 亮红色  (9)
  BRIGHT_GREEN,   ///< 亮绿色  (10)
  BRIGHT_YELLOW,  ///< 亮黄色  (11)
  BRIGHT_BLUE,    ///< 亮蓝色  (12)
  BRIGHT_MAGENTA, ///< 亮品红色 (13)
  BRIGHT_CYAN,    ///< 亮青色  (14)
  BRIGHT_WHITE    ///< 亮白色  (15)
};

/**
 * @brief 像素基类
 *
 * Pixel类表示一个具有背景色的基本像素单元，字符固定为空格
 */
class Pixel {
public:
  /**
   * @brief 构造函数，背景色默认为DEFAULT，字符为空格
   *
   * @param background 背景色，默认为Color::DEFAULT
   */
  explicit inline Pixel(Color bg = Color::DEFAULT);

  virtual ~Pixel() = default;

  /**
   * @brief 获取背景色
   *
   * @return Color 当前背景色
   */
  inline Color getBg() const;

  /**
   * @brief 设置背景色
   *
   * @param bg 新的背景色
   */
  inline void setBg(Color bg);

  /**
   * @brief 打印像素到ncurses屏幕
   *
   * 使用当前背景色渲染一个空格字符。
   */
  virtual void print() const;

protected:
  Color m_background;                      ///< 背景色
  static constexpr char m_character = ' '; ///< 像素的字符始终为空格
};

// Inline implementations
inline Pixel::Pixel(Color bg) : m_background(bg) {}
inline Color Pixel::getBg() const { return m_background; }
inline void Pixel::setBg(Color bg) { m_background = bg; }

#endif // PIXEL_H
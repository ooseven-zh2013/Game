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
  BLACK,          ///< 黑色
  RED,            ///< 红色
  GREEN,          ///< 绿色
  YELLOW,         ///< 黄色
  BLUE,           ///< 蓝色
  MAGENTA,        ///< 品红色
  CYAN,           ///< 青色
  WHITE,          ///< 白色
  BRIGHT_BLACK,   ///< 亮黑色(深灰色)
  BRIGHT_RED,     ///< 亮红色
  BRIGHT_GREEN,   ///< 亮绿色
  BRIGHT_YELLOW,  ///< 亮黄色
  BRIGHT_BLUE,    ///< 亮蓝色
  BRIGHT_MAGENTA, ///< 亮品红色
  BRIGHT_CYAN,    ///< 亮青色
  BRIGHT_WHITE    ///< 亮白色
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
  explicit Pixel(Color bg = Color::DEFAULT);

  virtual ~Pixel() = default;

  /**
   * @brief 获取背景色
   *
   * @return Color 当前背景色
   */
  Color getBg() const;

  /**
   * @brief 设置背景色
   *
   * @param bg 新的背景色
   */
  void setBg(Color bg);

  /**
   * @brief 打印像素
   *
   */
  virtual void print() const;

protected:
  Color m_background;                      ///< 背景色
  static constexpr char m_character = ' '; ///< 像素的字符始终为空格
};

#endif // PIXEL_H
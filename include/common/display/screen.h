#ifndef SCREEN_H
#define SCREEN_H
#pragma once

#include "color_char.h"
#include <vector>

/**
 * @brief 实现一个屏幕类
 * @note 所有下标从0开始，描述中所有的快慢皆是相较而言
 */
class Screen {
public:
  using vc = std::vector<ColorChar>; ///< 行向量类型别名
  using vvc = std::vector<vc>;       ///< 屏幕网格类型别名

  ~Screen() = default;

  /**
   * @brief 初始化一个（n * m）的彩色宽字符网格
   * @warning 网格一旦初始化就不能改变长度
   *
   * @param n 行数（高度）
   * @param m 列数（宽度）
   * @param init 初始ColorChar对象，默认为ColorChar()
   */
  inline Screen(size_t n, size_t m, const ColorChar &init = ColorChar());

  /**
   * @brief 网格总大小
   *
   * @return size_t 网格中元素的总数（行数 * 列数）
   */
  inline size_t size() const;

  /**
   * @brief 内部的网格数组引用
   *
   * @return vvc& 对内部网格的引用
   */
  inline vvc &data();

  /**
   * @brief 内部的网格数组常量引用
   *
   * @return const vvc& 对内部网格的常量引用
   */
  inline const vvc &data() const;

  /**
   * @brief 获取网格的长度（行数）
   *
   * @return size_t 网格的行数
   */
  inline size_t x_size() const;

  /**
   * @brief 获取网格的宽度（列数）
   *
   * @return size_t 网格的列数
   */
  inline size_t y_size() const;

  /**
   * @brief 内部的一行网格的引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return vc& 指定行的引用
   */
  inline vc &operator[](int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return const vc& 指定行的常量引用
   */
  inline const vc &operator[](int idx) const;

  /**
   * @brief 内部的一行网格的引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return vc& 指定行的引用
   */
  inline vc &at(int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return const vc& 指定行的常量引用
   */
  inline const vc &at(int idx) const;

  /**
   * @brief 返回网格中单个字符的引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return ColorChar& 指定位置的ColorChar引用
   */
  inline ColorChar &at(int x, int y);

  /**
   * @brief 返回网格中单个字符的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return const ColorChar& 指定位置的ColorChar常量引用
   */
  inline const ColorChar &at(int x, int y) const;

  /**
   * @brief 返回网格中单个字符的引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y}第x行第y列
   * @return ColorChar& 指定位置的ColorChar引用
   */
  inline ColorChar &at(const std::pair<int, int> &xy);

  /**
   * @brief 返回网格中单个字符的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y}第x行第y列
   * @return const ColorChar& 指定位置的ColorChar常量引用
   */
  inline const ColorChar &at(const std::pair<int, int> &xy) const;

  /**
   * @brief 打印整个网格到ncurses屏幕
   * @warning 请确保你的当前行没有任何字符！
   *
   * @param flushNow 是否立刻刷新缓冲区（立即显示），默认为true
   */
  void print(bool flushNow = true) const;

  /**
   * @brief 将屏幕中的所有字符设置为value
   *
   * @param value 设置的字符
   */
  void set(const ColorChar &value);

  /**
   * @brief 将屏幕中的所有字符重置为默认
   */
  inline void clear();

protected:
  vvc scr; ///< 存储屏幕内容的二维向量
};

// Inline implementations
inline Screen::Screen(size_t n, size_t m, const ColorChar &init) : scr(n, vc(m, init)) {}
inline size_t Screen::size() const { return x_size() * y_size(); }
inline Screen::vvc &Screen::data() { return scr; }
inline const Screen::vvc &Screen::data() const { return scr; }
inline size_t Screen::x_size() const { return scr.size(); }
inline size_t Screen::y_size() const { return x_size() ? scr[0].size() : 0; }
inline Screen::vc &Screen::operator[](int idx) { return scr[idx]; }
inline const Screen::vc &Screen::operator[](int idx) const { return scr[idx]; }
inline Screen::vc &Screen::at(int idx) { return scr.at(idx); }
inline const Screen::vc &Screen::at(int idx) const { return scr.at(idx); }
inline ColorChar &Screen::at(int x, int y) { return scr.at(x).at(y); }
inline const ColorChar &Screen::at(int x, int y) const { return scr.at(x).at(y); }
inline ColorChar &Screen::at(const std::pair<int, int> &xy) { return at(xy.first, xy.second); }
inline const ColorChar &Screen::at(const std::pair<int, int> &xy) const { return at(xy.first, xy.second); }
inline void Screen::clear() { set(ColorChar()); }

#endif // SCREEN_H
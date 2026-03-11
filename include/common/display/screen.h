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
   * @param n 长度
   * @param m 宽度
   * @param init 初始ColorChar对象，默认为ColorChar()
   */
  Screen(size_t n, size_t m, const ColorChar &init = ColorChar());

  /**
   * @brief 网格总大小
   *
   * @return size_t 网格中元素的总数
   */
  size_t size() const;

  /**
   * @brief 内部的网格数组引用
   *
   * @return vvc& 对内部网格的引用
   */
  vvc &data();

  /**
   * @brief 内部的网格数组常量引用
   *
   * @return const vvc& 对内部网格的常量引用
   */
  const vvc &data() const;

  /**
   * @brief 获取网格的长度（行数）
   *
   * @return size_t 网格的长度
   */
  size_t x_size() const;

  /**
   * @brief 获取网格的宽度（列数）
   *
   * @return size_t 网格的宽度
   */
  size_t y_size() const;

  /**
   * @brief 内部的一行网格的引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return vc& 指定行的引用
   */
  vc &operator[](int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return const vc& 指定行的常量引用
   */
  const vc &operator[](int idx) const;

  /**
   * @brief 内部的一行网格的引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return vc& 指定行的引用
   */
  vc &at(int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return const vc& 指定行的常量引用
   */
  const vc &at(int idx) const;

  /**
   * @brief 返回网格中单个字符的引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return ColorChar& 指定位置的ColorChar引用
   */
  ColorChar &at(int x, int y);

  /**
   * @brief 返回网格中单个字符的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度较慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return const ColorChar& 指定位置的ColorChar常量引用
   */
  const ColorChar &at(int x, int y) const;

  /**
   * @brief 打印整个网格
   * @warning 请确保你的当前行没有任何字符！
   *
   * @param flushNow 是否立刻刷新缓冲区（立即显示），默认为true
   */
  void print(bool flushNow = true) const;

  void set(const ColorChar &value);

  void clear();

protected:
  vvc scr; ///< 存储屏幕内容的二维向量
};

#endif // SCREEN_H
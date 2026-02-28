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
  using vc = std::vector<ColorChar>;
  using vvc = std::vector<vc>;

  ~Screen() = default;

  /**
   * @brief 初始化一个（n * m）的彩色宽字符网格
   * @warning 网格一旦初始化就不能改变长度
   *
   * @param n 长
   * @param m 宽
   */
  Screen(size_t n, size_t m);

  /**
   * @brief 网格大小
   *
   * @return size_t
   */
  size_t size() const;

  /**
   * @brief 内部的网格数组引用
   *
   * @return vvc&
   */
  vvc &data();

  /**
   * @brief 内部的网格数组常量引用
   *
   * @return const vvc&
   */
  const vvc &data() const;

  /**
   * @brief 长
   *
   * @return size_t
   */
  size_t x_size() const;

  /**
   * @brief 宽
   *
   * @return size_t
   */
  size_t y_size() const;

  /**
   * @brief 内部的一行网格的引用
   * @note 无安全检查，速度快
   *
   * @param idx 下标
   * @return vc&
   */
  vc &operator[](int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 下标
   * @return const vc&
   */
  const vc &operator[](int idx) const;

  /**
   * @brief 内部的一行网格的引用
   * @note 调用vector的at接口进行访问越界检查，速度慢
   *
   * @param idx 下标
   * @return vc&
   */
  vc &at(int idx);

  /**
   * @brief 内部的一行网格的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度慢
   *
   * @param idx 下标
   * @return const vc&
   */
  const vc &at(int idx) const;

  /**
   * @brief 返回网格中的单个字符的引用
   * @note 调用vector的at接口进行访问越界检查，速度慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return ColorChar&
   */
  ColorChar &at(int x, int y);

  /**
   * @brief 返回网格中的单个字符的常量引用
   * @note 调用vector的at接口进行访问越界检查，速度慢
   *
   * @param x 第x行
   * @param y 第y列
   * @return ColorChar&
   */
  const ColorChar &at(int x, int y) const;

  /**
   * @brief 打印整个网格
   * @warning 请确保你的当前行没有任何字符！
   *
   * @param flushNow 是否立刻刷新缓冲区（立即显示）
   */
  void print(bool flushNow = true) const;

protected:
  vvc scr;
};

#endif // SCREEN_H
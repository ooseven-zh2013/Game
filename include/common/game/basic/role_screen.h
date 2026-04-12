#ifndef ROLE_SCREEN_H
#define ROLE_SCREEN_H
#pragma once

#include "common/display/color_char.h"
#include <utility>
#include <vector>

class Role;

/**
 * @brief 角色屏幕管理类
 *
 * RoleScreen管理游戏角色的二维网格，每个格子包含一个ColorChar（用于显示）
 * 和一个Role指针（用于逻辑）。它合并了Screen和RoleList的功能。
 *
 * @note 所有下标从0开始
 */
class RoleScreen {
public:
  using Element = std::pair<ColorChar, Role *>; ///< 元素类型别名（ColorChar和Role指针的配对）
  using vE = std::vector<Element>;              ///< 行向量类型别名
  using vvE = std::vector<vE>;                  ///< 角色屏幕网格类型别名

  ~RoleScreen() = default;

  /**
   * @brief 初始化一个（n * m）的角色屏幕网格
   * @warning 网格一旦初始化就不能改变长度
   *
   * @param n 行数（高度）
   * @param m 列数（宽度）
   * @param init 初始元素，默认为默认构造的 Element
   */
  inline RoleScreen(size_t n, size_t m, const Element &init);

  /**
   * @brief 网格总大小
   *
   * @return size_t 网格中元素的总数（行数 * 列数）
   */
  inline size_t size() const;

  /**
   * @brief 内部的角色屏幕数组引用
   *
   * @return vvE& 对内部角色屏幕数组的引用
   */
  inline vvE &data();

  /**
   * @brief 内部的角色屏幕数组常量引用
   *
   * @return const vvE& 对内部角色屏幕数组的常量引用
   */
  inline const vvE &data() const;

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
   * @brief 内部的一行角色屏幕数组的引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return vE& 指定行的引用
   */
  inline vE &operator[](int idx);

  /**
   * @brief 内部的一行角色屏幕数组的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return const vE& 指定行的常量引用
   */
  inline const vE &operator[](int idx) const;

  /**
   * @brief 内部的一行角色屏幕数组的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return vE& 指定行的引用
   */
  inline vE &at(int idx);

  /**
   * @brief 内部的一行角色屏幕数组的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return const vE& 指定行的常量引用
   */
  inline const vE &at(int idx) const;

  /**
   * @brief 返回指定位置元素的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return Element& 指定位置的元素引用
   */
  inline Element &at(int x, int y);

  /**
   * @brief 返回指定位置元素的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return const Element& 指定位置的元素常量引用
   */
  inline const Element &at(int x, int y) const;

  /**
   * @brief 返回指定位置元素的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return Element& 指定位置的元素引用
   */
  inline Element &at(const std::pair<int, int> &xy);

  /**
   * @brief 返回指定位置元素的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return const Element& 指定位置的元素常量引用
   */
  inline const Element &at(const std::pair<int, int> &xy) const;

  /**
   * @brief 将所有元素设置为 value
   *
   * @param value 设置的元素值
   */
  void set(const Element &value);

  /**
   * @brief 将所有元素重置为默认值
   *
   * ColorChar 重置为默认构造，Role 指针设为 nullptr。
   */
  void clear();

  /**
   * @brief 打印整个角色屏幕到ncurses
   *
   * 遍历屏幕上的所有元素并渲染ColorChar部分。
   *
   * @param flushNow 是否立即刷新显示，默认为true
   */
  void print(bool flushNow = true) const;

protected:
  vvE scr; ///< 存储元素（ColorChar和Role指针配对）的二维向量
};

// Inline implementations
inline RoleScreen::RoleScreen(size_t n, size_t m, const Element &init) : scr(n, vE(m, init)) {}
inline size_t RoleScreen::size() const { return x_size() * y_size(); }
inline RoleScreen::vvE &RoleScreen::data() { return scr; }
inline const RoleScreen::vvE &RoleScreen::data() const { return scr; }
inline size_t RoleScreen::x_size() const { return scr.size(); }
inline size_t RoleScreen::y_size() const { return scr.empty() ? 0 : scr[0].size(); }
inline RoleScreen::vE &RoleScreen::operator[](int idx) { return scr[idx]; }
inline const RoleScreen::vE &RoleScreen::operator[](int idx) const { return scr[idx]; }
inline RoleScreen::vE &RoleScreen::at(int idx) { return scr.at(idx); }
inline const RoleScreen::vE &RoleScreen::at(int idx) const { return scr.at(idx); }
inline RoleScreen::Element &RoleScreen::at(int x, int y) { return scr.at(x).at(y); }
inline const RoleScreen::Element &RoleScreen::at(int x, int y) const { return scr.at(x).at(y); }
inline RoleScreen::Element &RoleScreen::at(const std::pair<int, int> &xy) { return scr.at(xy.first).at(xy.second); }
inline const RoleScreen::Element &RoleScreen::at(const std::pair<int, int> &xy) const {
  return scr.at(xy.first).at(xy.second);
}

#endif // ROLE_SCREEN_H

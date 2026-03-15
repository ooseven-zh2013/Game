#ifndef ROLE_SCREEN_H
#define ROLE_SCREEN_H
#pragma once

#include <utility>
#include <vector>

class ColorChar;
class Role;

/**
 * @brief 角色屏幕管理类
 * @note 合并 Screen 和 RoleList 的功能，每个格子包含 ColorChar 和 Role*
 */
class RoleScreen {
public:
  using Element = std::pair<ColorChar, Role *>; ///< 元素类型别名
  using vE = std::vector<Element>;              ///< 行向量类型别名
  using vvE = std::vector<vE>;                  ///< 角色屏幕网格类型别名

  ~RoleScreen() = default;

  /**
   * @brief 初始化一个（n * m）的角色屏幕网格
   * @warning 网格一旦初始化就不能改变长度
   *
   * @param n 长度（行数）
   * @param m 宽度（列数）
   * @param init 初始元素，默认为默认构造的 Element
   */
  RoleScreen(size_t n, size_t m, const Element &init);

  /**
   * @brief 网格总大小
   *
   * @return size_t 网格中元素的总数
   */
  size_t size() const;

  /**
   * @brief 内部的角色屏幕数组引用
   *
   * @return vvE& 对内部角色屏幕数组的引用
   */
  vvE &data();

  /**
   * @brief 内部的角色屏幕数组常量引用
   *
   * @return const vvE& 对内部角色屏幕数组的常量引用
   */
  const vvE &data() const;

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
   * @brief 内部的一行角色屏幕数组的引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return vE& 指定行的引用
   */
  vE &operator[](int idx);

  /**
   * @brief 内部的一行角色屏幕数组的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return const vE& 指定行的常量引用
   */
  const vE &operator[](int idx) const;

  /**
   * @brief 内部的一行角色屏幕数组的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return vE& 指定行的引用
   */
  vE &at(int idx);

  /**
   * @brief 内部的一行角色屏幕数组的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return const vE& 指定行的常量引用
   */
  const vE &at(int idx) const;

  /**
   * @brief 返回指定位置元素的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return Element& 指定位置的元素引用
   */
  Element &at(int x, int y);

  /**
   * @brief 返回指定位置元素的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return const Element& 指定位置的元素常量引用
   */
  const Element &at(int x, int y) const;

  /**
   * @brief 返回指定位置元素的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return Element& 指定位置的元素引用
   */
  Element &at(const std::pair<int, int> &xy);

  /**
   * @brief 返回指定位置元素的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return const Element& 指定位置的元素常量引用
   */
  const Element &at(const std::pair<int, int> &xy) const;

  /**
   * @brief 将所有元素设置为 value
   *
   * @param value 设置的元素值
   */
  void set(const Element &value);

  /**
   * @brief 将所有元素重置为默认值
   */
  void clear();

protected:
  vvE scr; ///< 存储元素指针的二维向量
};

#endif // ROLE_SCREEN_H
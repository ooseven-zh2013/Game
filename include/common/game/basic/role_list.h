#ifndef ROLE_LIST_H
#define ROLE_LIST_H
#pragma once

#include <vector>

class Role;

/**
 * @brief 角色列表管理类
 * @note 仿照 Screen 类设计，管理二维角色指针数组
 */
class RoleList {
public:
  using vr = std::vector<Role *>; ///< 行向量类型别名
  using vvr = std::vector<vr>;    ///< 角色网格类型别名

  ~RoleList() = default;

  /**
   * @brief 初始化一个（n * m）的角色网格
   * @warning 网格一旦初始化就不能改变长度
   *
   * @param n 长度（行数）
   * @param m 宽度（列数）
   * @param init 初始角色指针，默认为 nullptr
   */
  RoleList(size_t n, size_t m, Role *init = nullptr);

  /**
   * @brief 网格总大小
   *
   * @return size_t 网格中元素的总数
   */
  size_t size() const;

  /**
   * @brief 内部的角色数组引用
   *
   * @return vvr& 对内部角色数组的引用
   */
  vvr &data();

  /**
   * @brief 内部的角色数组常量引用
   *
   * @return const vvr& 对内部角色数组的常量引用
   */
  const vvr &data() const;

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
   * @brief 内部的一行角色数组的引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return vr& 指定行的引用
   */
  vr &operator[](int idx);

  /**
   * @brief 内部的一行角色数组的常量引用
   * @note 无安全检查，速度快
   *
   * @param idx 行索引
   * @return const vr& 指定行的常量引用
   */
  const vr &operator[](int idx) const;

  /**
   * @brief 内部的一行角色数组的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return vr& 指定行的引用
   */
  vr &at(int idx);

  /**
   * @brief 内部的一行角色数组的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param idx 行索引
   * @return const vr& 指定行的常量引用
   */
  const vr &at(int idx) const;

  /**
   * @brief 返回指定位置角色的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return Role*& 指定位置的角色指针引用
   */
  Role *&at(int x, int y);

  /**
   * @brief 返回指定位置角色的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param x 第 x 行
   * @param y 第 y 列
   * @return Role*const& 指定位置的角色指针常量引用
   */
  Role *const &at(int x, int y) const;

  /**
   * @brief 返回指定位置角色的引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return Role*& 指定位置的角色指针引用
   */
  Role *&at(const std::pair<int, int> &xy);

  /**
   * @brief 返回指定位置角色的常量引用
   * @note 调用 vector 的 at 接口进行访问越界检查，速度较慢
   *
   * @param xy {x, y} 第 x 行第 y 列
   * @return Role*const& 指定位置的角色指针常量引用
   */
  Role *const &at(const std::pair<int, int> &xy) const;

  /**
   * @brief 将所有角色指针设置为 value
   *
   * @param value 设置的角色指针
   */
  void set(Role *value);

  /**
   * @brief 将所有角色指针重置为 nullptr
   */
  void clear();

  /**
   * @brief 删除所有角色并重置为 nullptr
   * @warning 这会释放所有角色的内存
   */
  void destroy_all();

protected:
  vvr roles; ///< 存储角色指针的二维向量
};

#endif // ROLE_LIST_H
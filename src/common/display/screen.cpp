#include "common/display/screen.h"
#include <iostream>

/**
 * @brief 构造函数，初始化一个（n * m）的彩色字符网格
 *
 * @param n 长度
 * @param m 宽度
 * @param init 初始ColorChar对象
 */
Screen::Screen(size_t n, size_t m, const ColorChar &init) : scr(n, vc(m, init)) {}

/**
 * @brief 网格总大小
 *
 * @return size_t 网格中元素的总数
 */
size_t Screen::size() const { return x_size() * y_size(); }

/**
 * @brief 内部的网格数组引用
 *
 * @return vvc& 对内部网格的引用
 */
Screen::vvc &Screen::data() { return scr; }

/**
 * @brief 内部的网格数组常量引用
 *
 * @return const vvc& 对内部网格的常量引用
 */
const Screen::vvc &Screen::data() const { return scr; }

/**
 * @brief 获取网格的长度（行数）
 *
 * @return size_t 网格的长度
 */
size_t Screen::x_size() const { return scr.size(); }

/**
 * @brief 获取网格的宽度（列数）
 *
 * @return size_t 网格的宽度
 */
size_t Screen::y_size() const { return x_size() ? scr[0].size() : 0; }

/**
 * @brief 内部的一行网格的引用
 * @note 无安全检查，速度快
 *
 * @param idx 行索引
 * @return vc& 指定行的引用
 */
Screen::vc &Screen::operator[](int idx) { return scr[idx]; }

/**
 * @brief 内部的一行网格的常量引用
 * @note 无安全检查，速度快
 *
 * @param idx 行索引
 * @return const vc& 指定行的常量引用
 */
const Screen::vc &Screen::operator[](int idx) const { return scr[idx]; }

/**
 * @brief 内部的一行网格的引用
 * @note 调用vector的at接口进行访问越界检查，速度较慢
 *
 * @param idx 行索引
 * @return vc& 指定行的引用
 */
Screen::vc &Screen::at(int idx) { return scr.at(idx); }

/**
 * @brief 内部的一行网格的常量引用
 * @note 调用vector的at接口进行访问越界检查，速度较慢
 *
 * @param idx 行索引
 * @return const vc& 指定行的常量引用
 */
const Screen::vc &Screen::at(int idx) const { return scr.at(idx); }

/**
 * @brief 返回网格中单个字符的引用
 * @note 调用vector的at接口进行访问越界检查，速度较慢
 *
 * @param x 第x行
 * @param y 第y列
 * @return ColorChar& 指定位置的ColorChar引用
 */
ColorChar &Screen::at(int x, int y) { return scr.at(x).at(y); }

/**
 * @brief 返回网格中单个字符的常量引用
 * @note 调用vector的at接口进行访问越界检查，速度较慢
 *
 * @param x 第x行
 * @param y 第y列
 * @return const ColorChar& 指定位置的ColorChar常量引用
 */
const ColorChar &Screen::at(int x, int y) const { return scr.at(x).at(y); }

/**
 * @brief 打印整个网格
 * @warning 请确保你的当前行没有任何字符！
 *
 * @param flushNow 是否立刻刷新缓冲区（立即显示），默认为true
 */
void Screen::print(bool flushNow) const {
  if (flushNow)
    std::flush(std::wcout);
  for (const auto &row : scr) {
    for (const auto &col : row) {
      col.print();
    }
    std::wcout.put('\n');
  }
  if (flushNow)
    std::flush(std::wcout);
}

void Screen::set(const ColorChar &value) {
  for (auto &rol : scr) {
    std::fill(rol.begin(), rol.end(), value);
  }
}

void Screen::clear() { set(ColorChar()); }
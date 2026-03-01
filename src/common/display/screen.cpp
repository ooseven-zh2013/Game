#include "common/display/screen.h"
#include <iostream>

// 构造函数实现
Screen::Screen(size_t n, size_t m, const ColorChar &init) : scr(n, vc(m, init)) {}

// 网格总大小
size_t Screen::size() const { return x_size() * y_size(); }

// 返回内部数据引用 (非常量版本)
Screen::vvc &Screen::data() { return scr; }

// 返回内部数据引用 (常量版本)
const Screen::vvc &Screen::data() const { return scr; }

// 返回行数 (长)
size_t Screen::x_size() const { return scr.size(); }

// 返回列数 (宽)
size_t Screen::y_size() const { return x_size() ? scr[0].size() : 0; }

// 重载 [] 操作符 (非常量版本)
Screen::vc &Screen::operator[](int idx) { return scr[idx]; }

// 重载 [] 操作符 (常量版本)
const Screen::vc &Screen::operator[](int idx) const { return scr[idx]; }

// 重载 at() 操作符 (行访问, 非常量版本)
Screen::vc &Screen::at(int idx) { return scr.at(idx); }

// 重载 at() 操作符 (行访问, 常量版本)
const Screen::vc &Screen::at(int idx) const { return scr.at(idx); }

// 重载 at() 操作符 (元素访问, 非常量版本)
ColorChar &Screen::at(int x, int y) { return scr.at(x).at(y); }

// 重载 at() 操作符 (元素访问, 常量版本)
const ColorChar &Screen::at(int x, int y) const { return scr.at(x).at(y); }

// 打印网格内容
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
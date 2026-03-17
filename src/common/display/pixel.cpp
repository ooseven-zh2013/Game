#include "common/display/pixel.h"
#include <iostream>

Pixel::Pixel(Color bg) : m_background(bg) {}
Color Pixel::getBg() const { return m_background; }

void Pixel::setBg(Color bg) { m_background = bg; }

void Pixel::print() const {
  std::wcout << L"\033[" << 39 << L';'                        // 前景色码
             << (static_cast<int>(m_background) + 40) << L'm' // 背景色码
             << L' '                                          // 打印字符
             << L"\033[0m";                                   // 恢复默认颜色
}
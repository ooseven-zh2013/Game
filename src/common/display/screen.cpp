#include "common/display/screen.h"
#include <iostream>

Screen::Screen(size_t n, size_t m, const ColorChar &init) : scr(n, vc(m, init)) {}

size_t Screen::size() const { return x_size() * y_size(); }

Screen::vvc &Screen::data() { return scr; }

const Screen::vvc &Screen::data() const { return scr; }

size_t Screen::x_size() const { return scr.size(); }

size_t Screen::y_size() const { return x_size() ? scr[0].size() : 0; }

Screen::vc &Screen::operator[](int idx) { return scr[idx]; }

const Screen::vc &Screen::operator[](int idx) const { return scr[idx]; }

const ColorChar &Screen::at(int x, int y) const { return scr.at(x).at(y); }

Screen::vc &Screen::at(int idx) { return scr.at(idx); }

const Screen::vc &Screen::at(int idx) const { return scr.at(idx); }

ColorChar &Screen::at(int x, int y) { return scr.at(x).at(y); }

ColorChar &Screen::at(const std::pair<int, int> &xy) { return at(xy.first, xy.second); }

const ColorChar &Screen::at(const std::pair<int, int> &xy) const { return at(xy.first, xy.second); }

void Screen::print(bool flushNow) const {
  for (const auto &row : scr) {
    for (const auto &col : row) {
      col.print();
    }
    std::wcout.put('\n');
  }
  if (flushNow)
    std::wcout.flush();
}

void Screen::set(const ColorChar &value) {
  for (auto &rol : scr) {
    std::fill(rol.begin(), rol.end(), value);
  }
}

void Screen::clear() { set(ColorChar()); }
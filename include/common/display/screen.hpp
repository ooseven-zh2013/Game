#include "color_char.h"
#include <vector>

// TODO 实现屏幕类
class Screen {
public:
  using vc = std::vector<ColorChar>;
  using vvc = std::vector<vc>;
  Screen() = default;
  ~Screen() = default;
  Screen(size_t n_, size_t m_) : scr{n_, vc{m_}} {}
  size_t size() const { return x_size() * y_size(); }
  vvc &data() { return scr; }
  const vvc &data() const { return scr; }
  size_t x_size() const { return scr.size(); }
  size_t y_size() const { return x_size() ? scr[0].size() : 0; }

protected:
  vvc scr;

private:
};
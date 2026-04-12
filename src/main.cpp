#include "common/game/basic/role_screen.h"
#include "common/io/control.hpp"
#include "snake/role/apple.h"
#include "snake/role/snake.h"
#include <memory>
#include <ncurses.h>
#include <unistd.h>
#include <vector>

#ifdef DEBUG
#include <chrono>
#include <iostream>
#endif

/**
 * @brief 贪吃蛇游戏主函数
 *
 * 初始化4条AI蛇和2个苹果，轮流更新蛇的状态直到所有蛇死亡。
 *
 * @return int 程序退出码
 */
int main() {
  // 初始化ncurses环境
  inNcu();

  // 创建角色屏幕（30行 x 60列）
  constexpr size_t ROWS = 30;
  constexpr size_t COLS = 60;
  RoleScreen::Element defaultElem(ColorChar(' ', Color::DEFAULT, Color::WHITE), nullptr);
  RoleScreen scr(ROWS, COLS, defaultElem);

  // 创建4条AI蛇（非玩家控制）
  std::vector<std::unique_ptr<Snake>> snakes;
  snakes.reserve(4);
  for (int i = 0; i < 4; ++i) {
    snakes.push_back(std::make_unique<Snake>(scr, false)); // false表示AI蛇
  }

  // 创建2个苹果
  std::vector<std::unique_ptr<Apple>> apples;
  apples.reserve(2);
  for (int i = 0; i < 2; ++i) {
    apples.push_back(std::make_unique<Apple>(scr));
  }

  // 初始显示
  scr.print(true);

#ifdef DEBUG
  // 性能统计
  int frameCount = 0;
  double totalTimeMs = 0.0;
  double maxTimeMs = 0.0;
#endif

  // 游戏主循环：直到所有蛇都死亡
  bool allDead = false;
  while (!allDead) {
    // 轮流更新4条AI蛇
    for (auto &snake : snakes) {
      snake->update();
    }

#ifdef DEBUG
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    double frameTimeMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();
    // 统计性能
    frameCount++;
    totalTimeMs += frameTimeMs;
    if (frameTimeMs > maxTimeMs) {
      maxTimeMs = frameTimeMs;
    }
#endif

    // 刷新屏幕显示
    scr.print(true);

    // 检查所有蛇的状态
    allDead = true;
    int cnt = 0;
    for (const auto &snake : snakes) {
      if (!snake->isDeadCheck()) {
        allDead = false;
        attron(COLOR_PAIR(static_cast<int>(Color::GREEN) + 1));
        printw("\n%d号蛇:%zu 存活", ++cnt, snake->score());
        attroff(COLOR_PAIR(static_cast<int>(Color::GREEN) + 1));
      } else {
        attron(COLOR_PAIR(static_cast<int>(Color::RED) + 1));
        printw("\n%d号蛇:%zu 死亡", ++cnt, snake->score());
        attroff(COLOR_PAIR(static_cast<int>(Color::RED) + 1));
      }
    }
    refresh();

    // 短暂延迟，控制游戏速度（100毫秒）
    usleep(50000);
  }

  waitKey();

  // 清理ncurses环境
  clNcu();

#ifdef DEBUG
  // 输出性能统计
  std::cout << "\n=== Performance Stats ===" << '\n';
  std::cout << "Total frames: " << frameCount << '\n';
  std::cout << "Average update time: " << (totalTimeMs / frameCount) << " ms" << '\n';
  std::cout << "Max update time: " << maxTimeMs << " ms" << '\n';
  std::cout << "========================\n" << '\n';
  std::cout.flush();
#endif

  return 0;
}

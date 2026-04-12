#ifndef CONTROL_HPP
#define CONTROL_HPP
#pragma once

#include <clocale>
#include <ncurses.h>

/**
 * @brief 初始化 ncurses 环境
 *
 * 配置终端为非规范模式，禁用回显，启用功能键识别，并初始化颜色对。
 * 颜色对编号 1-16 对应背景色：
 *   1=BLACK, 2=RED, 3=GREEN, 4=YELLOW, 5=BLUE, 6=MAGENTA, 7=CYAN, 8=WHITE,
 *   9=BRIGHT_BLACK, 10=BRIGHT_RED, ..., 16=BRIGHT_WHITE
 */
inline void inNcu() {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  use_default_colors();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, FALSE);

  // 初始化16个颜色对，背景色对应 Color 枚举值+1
  for (short i = 0; i < 16; ++i) {
    init_pair(static_cast<short>(i + 1), -1, i); // 前景色-1表示使用默认色
  }
}

/**
 * @brief 清理 ncurses 环境并恢复终端设置
 */
inline void clNcu() {
  curs_set(1);
  endwin();
}

/**
 * @brief 等待用户按键输入
 *
 * @return int 用户按下的键的ASCII码值
 */
inline int waitKey() {
  mvprintw(LINES - 1, 0, "请按任意键继续...");
  refresh();
  return getch();
}

#endif // CONTROL_HPP
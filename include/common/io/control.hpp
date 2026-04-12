#ifndef CONTROL_HPP
#define CONTROL_HPP
#pragma once

#include <ncurses.h>

/**
 * @brief 初始化 ncurses 环境
 *
 * 配置终端为非规范模式，禁用回显，启用功能键识别
 */
inline void inNcu() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, FALSE);
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
#ifndef CONTROL_HPP
#define CONTROL_HPP
#pragma once

#include <clocale>
#include <ncurses.h>
#include <string>

/**
 * @brief ncurses初始化和清理的RAII管理类
 *
 * NcuInit类使用RAII（Resource Acquisition Is Initialization）模式管理ncurses环境。
 * 在构造时自动初始化ncurses，在析构时自动清理并恢复终端设置。
 * 这种设计确保了即使在异常情况下也能正确清理资源。
 */
class NcuInit {
public:
  /**
   * @brief 构造函数，自动初始化ncurses环境
   */
  NcuInit() {
    static bool isInit = false;
    if (isInit)
      return;
    inNcu();
    isInit = true;
  }

  /**
   * @brief 析构函数，自动清理ncurses环境
   */
  ~NcuInit() {
    static bool isInit = true;
    if (!isInit)
      return;
    clNcu();
    isInit = false;
  }

private:
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

    // 初始化颜色对
    for (short i = 0; i < 16; ++i) {
      init_pair(static_cast<short>(i + 1), -1, i);                          // 背景色
      init_pair(static_cast<short>(i + 17), i, -1);                         // 前景色
      init_pair(static_cast<short>(i + 33), i, static_cast<short>(15 - i)); // 反差色
    }
  }

  /**
   * @brief 清理 ncurses 环境并恢复终端设置
   */
  inline void clNcu() {
    curs_set(1);
    endwin();
  }
};

/**
 * @brief 清除指定行的内容
 *
 * 用空格字符填充指定行从指定列开始到行尾的所有位置。
 *
 * @param line_ 要清除的行号
 * @param col_ 起始列号，默认为0（从行首开始）
 */
inline void eraseLine(int line_, int col_ = 0) { mvprintw(line_, col_, "%s", std::string(COLS - col_, ' ').c_str()); }

/**
 * @brief 等待用户按键输入
 *
 * @return int 用户按下的键的ASCII码值
 */
inline int waitKey(const std::string &todo = "继续") {
  eraseLine(LINES - 1, 0);
  mvprintw(LINES - 1, 0, "请按任意键%s...", todo.c_str());
  refresh();
  return getch();
}

#endif // CONTROL_HPP
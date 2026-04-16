/**
 * @file debug.hpp
 * @brief 调试控制台
 *
 * 提供交互式调试界面，支持命令输入、自动补全和实时提示。
 * 仅在DEBUG模式下编译启用。
 */

#ifndef DEBUG_H
#define DEBUG_H
#pragma once

#include "common/io/control.hpp"
#include "snake/command.hpp"

/**
 * @brief 调试函数 - 显示交互式调试控制台
 *
 * 提供一个类似命令行的界面，支持：
 *   - 实时命令输入和退格删除
 *   - Tab键自动补全命令
 *   - 显示匹配命令的描述和参数提示
 *   - 回车确认输入
 *
 * @param col 调试控制台的起始列位置（行固定为0）
 * @return std::pair<std::string, std::vector<std::string>>
 *         第一个元素为命令名，第二个元素为参数列表
 */
std::pair<std::string, std::vector<std::string>> debug(int col) {
  init();
  eraseLine(0, col);
  mvprintw(0, col, "请键入调试指令");
  move(0, col);
  refresh();
  std::string command;
  int temp = getch();
  eraseLine(0, col);
  while (temp != '\n') {
    if (temp == '\b' || temp == 263) {
      if (!command.empty()) {
        command.pop_back();
        mvprintw(0, static_cast<int>(command.size() + col), " ");
        move(0, static_cast<int>(command.size() + col));
      }
    } else if (temp == '\t') {
      auto result = ctr.find(hanCom(command).first);
      if (result.second) {
        command = result.first; // 使用返回的命令字符串
      }
      eraseLine(0, col);
      mvprintw(0, col, "%s", command.c_str());
      refresh();
      move(0, static_cast<int>(command.size() + col));
    } else if (std::isprint(temp)) {
      command += static_cast<char>(temp);
      mvprintw(0, static_cast<int>(command.size() + col - 1), "%c", temp);
      refresh();
    }
    auto result = ctr.find(hanCom(command).first);
    if (result.second) {
      mvprintw(1, col, "%s", result.first.c_str());
      for (const auto &par : result.second->parameters) {
        printw(" [%s]", par.c_str());
      }
      mvprintw(2, col, "%s", result.second->description.c_str());
    } else {
      eraseLine(1, col);
      eraseLine(2, col);
      move(0, static_cast<int>(command.size() + col));
    }
    temp = getch();
  }
  eraseLine(0, col);
  eraseLine(1, col);
  eraseLine(2, col);
  auto result = hanCom(command);
  std::string com = result.first;
  std::vector<std::string> pars = result.second;
  return {com, pars};
}

#endif // DEBUG_H
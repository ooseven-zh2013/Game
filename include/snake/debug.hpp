/**
 * @file debug.hpp
 * @brief 调试命令初始化（贪吃蛇专用）
 *
 * 提供贪吃蛇游戏的调试命令初始化功能。
 */

#ifndef SNAKE_DEBUG_H
#define SNAKE_DEBUG_H
#pragma once

#include "common/debug/debug.hpp"

/**
 * @brief 初始化贪吃蛇调试命令
 *
 * 注册所有可用的调试命令到全局命令树 ctr 中。
 * 此函数是幂等的，多次调用只会初始化一次。
 */
void init() {
  static bool isInit = false;
  if (isInit)
    return;
  isInit = true;
  ctr.insert("help", "显示帮助信息");
  ctr.insert("info", "查询蛇类信息", {"编号"});
  ctr.insert("kill", "强制杀死蛇类", {"编号"});
  ctr.insert("continue", "退出控制台");
  ctr.insert("exit", "结束程序");
  ctr.insert("edit", "编辑蛇的属性", {"编号", "属性(l=长度,d=方向)", "值"});
}

auto snakeDebug(int col) {
  init();
  return debug(col);
}

#endif // SNAKE_DEBUG_H

/**
 * @file debug.hpp
 * @brief 调试命令初始化（贪吃蛇专用）
 *
 * 提供贪吃蛇游戏的调试命令初始化和控制台功能。
 */

#ifndef SNAKE_DEBUG_H
#define SNAKE_DEBUG_H
#pragma once

#ifndef DEBUG
#define DEBUG
#endif
#include "common/debug/debug.hpp"
#include "snake/role/snake.hpp"
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

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
  ctr.insert("respawn", "复活已死亡的蛇", {"编号"});
  ctr.insert("continue", "退出控制台");
  ctr.insert("exit", "结束程序");
  ctr.insert("edit", "编辑蛇的属性", {"编号", "属性(l=长度,d=方向)", "值"});
}

/**
 * @brief 贪吃蛇调试控制台
 *
 * 显示调试控制台界面，处理用户输入的调试命令。
 * 支持的命令：
 *   - info [编号]: 显示蛇的信息（包括待转向方向）
 *   - kill [编号]: 强制杀死指定蛇
 *   - respawn [编号]: 原地复活已死亡的蛇
 *   - edit [编号] [属性] [值]: 编辑蛇的属性（l=长度, d=方向）
 *   - continue: 退出调试控制台
 *   - exit: 结束程序
 *
 * @param col 调试控制台起始列位置
 * @param snakes 蛇对象向量（通过unique_ptr管理）
 * @return bool true表示继续游戏，false表示退出程序
 */
bool snakeDebug(int col, std::vector<std::unique_ptr<Snake>> &snakes) {
  init();

  // 调试模式：显示调试控制台
  bool shouldContinue = true;
  while (true) {
    const auto com = debug(col);
    int infoLine = 0;

    if (com.first == "continue") {
      break;
    } else if (com.first == "exit") {
      shouldContinue = false;
      break;
    } else if (com.first == "info") {
      // 显示蛇的信息
      if (com.second.empty()) {
        // 没有提供编号，显示所有蛇的信息
        for (size_t snakeIndex = 0; snakeIndex < snakes.size(); snakeIndex++) {
          mvprintw(infoLine++, static_cast<int>(col), "编号:%zu", snakeIndex + 1);
          mvprintw(infoLine++, static_cast<int>(col), "长度:%zu", snakes[snakeIndex]->score());
          mvprintw(infoLine++, static_cast<int>(col), "颜色:%s", snakes[snakeIndex]->getColorName());
          mvprintw(infoLine++, static_cast<int>(col), "待增加长度:%d", snakes[snakeIndex]->getEdit());
          mvprintw(infoLine++, static_cast<int>(col), "待转向方向:%s", snakes[snakeIndex]->getForcedDirName());
          infoLine++;
        }
      } else {
        // 显示指定蛇的信息
        const int snakeIndex = std::stoi(com.second[0]) - 1;
        if (snakeIndex >= 0 && snakeIndex < static_cast<int>(snakes.size())) {
          mvprintw(infoLine++, static_cast<int>(col), "长度:%zu", snakes[snakeIndex]->score());
          mvprintw(infoLine++, static_cast<int>(col), "颜色:%s", snakes[snakeIndex]->getColorName());
          mvprintw(infoLine++, static_cast<int>(col), "待增加长度:%d", snakes[snakeIndex]->getEdit());
          mvprintw(infoLine++, static_cast<int>(col), "待转向方向:%s", snakes[snakeIndex]->getForcedDirName());
        } else {
          mvprintw(infoLine++, static_cast<int>(col), "错误: 无效的蛇编号");
        }
      }
    } else if (com.first == "kill") {
      // 强制杀死指定蛇
      if (!com.second.empty()) {
        const int snakeIndex = std::stoi(com.second[0]) - 1;
        if (snakeIndex >= 0 && snakeIndex < static_cast<int>(snakes.size())) {
          snakes[snakeIndex]->kill();
        } else {
          mvprintw(infoLine++, static_cast<int>(col), "错误: 无效的蛇编号");
        }
      } else {
        mvprintw(infoLine++, static_cast<int>(col), "错误: 请提供蛇编号");
      }
    } else if (com.first == "respawn") {
      // 复活已死亡的蛇（原地复活）
      if (!com.second.empty()) {
        try {
          const int snakeIndex = std::stoi(com.second[0]) - 1;
          // 防御性编程：检查编号合法性
          if (snakeIndex < 0 || snakeIndex >= static_cast<int>(snakes.size())) {
            mvprintw(infoLine++, static_cast<int>(col), "错误: 无效的蛇编号 %d", snakeIndex + 1);
          } else if (!snakes[snakeIndex]->isDeadCheck()) {
            mvprintw(infoLine++, static_cast<int>(col), "警告: 蛇%d仍然存活，无需复活", snakeIndex + 1);
          } else {
            // 调用蛇的复活方法（原地复活，如果位置被占用则立即死亡）
            snakes[snakeIndex]->respawn();
            if (snakes[snakeIndex]->isDeadCheck()) {
              mvprintw(infoLine++, static_cast<int>(col), "失败: 蛇%d复活位置被占用，无法复活", snakeIndex + 1);
            } else {
              mvprintw(infoLine++, static_cast<int>(col), "成功: 蛇%d已原地复活", snakeIndex + 1);
            }
          }
        } catch (const std::exception &e) {
          mvprintw(infoLine++, static_cast<int>(col), "错误: 无效的编号格式");
        }
      } else {
        mvprintw(infoLine++, static_cast<int>(col), "错误: 请提供蛇编号");
      }
    } else if (com.first == "edit") {
      if (com.second.size() < 3) {
        mvprintw(infoLine++, col, "错误：参数过少");
      } else {
        try {
          int num = std::stoi(com.second[0]) - 1;
          // 防御性编程：检查蛇编号合法性
          if (num < 0 || num >= static_cast<int>(snakes.size())) {
            mvprintw(infoLine++, col, "错误：无效的蛇编号 %d", num + 1);
          } else {
            std::string mode = com.second[1];
            if (mode == "l") {
              int val = std::stoi(com.second[2]);
              // 计算调整后的预期长度：当前长度 + 待增加长度 + 新调整值
              int expectedLength = static_cast<int>(snakes[num]->score()) + snakes[num]->getEdit() + val;
              if (expectedLength < 1) {
                mvprintw(infoLine++, col, "错误：不允许长度小于1的蛇类出现（当前:%zu, 待调整:%d, 新调整:%d, 预期:%d）",
                         snakes[num]->score(), snakes[num]->getEdit(), val, expectedLength);
              } else {
                // 调整蛇的长度
                snakes[num]->edit(val);
                mvprintw(infoLine++, col, "已设置蛇%d的长度调整为:%d", num + 1, val);
              }
            } else if (mode == "d") {
              // 强制转向：支持 u(上), d(下), l(左), r(右) 或数字 -1 - 3
              std::string dirStr = com.second[2];
              int newDir = 0;
              bool validDir = true;

              if (dirStr == "u" || dirStr == "U") {
                newDir = 3; // 上
              } else if (dirStr == "r" || dirStr == "R") {
                newDir = 2; // 右
              } else if (dirStr == "d" || dirStr == "D") {
                newDir = 1; // 下
              } else if (dirStr == "l" || dirStr == "L") {
                newDir = 0; // 左
              } else {
                // 尝试解析为数字
                try {
                  int dirVal = std::stoi(dirStr);
                  if (dirVal >= -1 && dirVal <= 3) {
                    newDir = dirVal;
                  } else {
                    validDir = false;
                  }
                } catch (...) {
                  validDir = false;
                }
              }

              if (validDir) {
                snakes[num]->turn(newDir);
                const char *dirNames[] = {"无", "左", "下", "右", "上"};
                mvprintw(infoLine++, col, "已设置蛇%d强制转向:%s", num + 1, dirNames[newDir + 1]);
              } else {
                mvprintw(infoLine++, col, "错误：方向值必须为 u/d/l/r 或 -1 - 3");
              }
            } else {
              mvprintw(infoLine++, col, "错误：不支持的属性 (支持: l=长度, d=方向)");
            }
          }
        } catch (const std::exception &e) {
          mvprintw(infoLine++, col, "错误：无效的编号格式");
        }
      }
    }

    mvprintw(infoLine, static_cast<int>(col), "按任意键继续");
    getch();

    for (int i = 0; i <= infoLine; i++) {
      eraseLine(i, col);
    }
  }

  return shouldContinue;
}

#endif // SNAKE_DEBUG_H

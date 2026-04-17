/**
 * @file main.cpp
 * @brief 贪吃蛇游戏主程序入口
 *
 * 本文件实现了基于ncurses的贪吃蛇游戏主循环。
 * 支持4条AI蛇同时游戏，包含调试模式和暂停功能。
 *
 * @author Ooseven
 * @date 2026
 */

#define DEBUG

#include "common/game/basic/role_screen.hpp"
#include "common/io/control.hpp"
#include "snake/role/apple.hpp"
#include "snake/role/snake.hpp"
#include <memory>
#include <ncurses.h>
#include <unistd.h>
#include <vector>

#ifdef DEBUG

#include "snake/debug.hpp"

#endif // DEBUG

/**
 * @brief 贪吃蛇游戏主函数
 *
 * 初始化ncurses环境，创建4条AI蛇和2个苹果，
 * 进入游戏主循环，轮流更新蛇的状态直到所有蛇死亡。
 * 支持暂停、调试和游戏结束提示功能。
 *
 * @return int 程序退出码（始终返回0）
 */
int main() {

  // 初始化ncurses环境（RAII管理，自动清理）
  NcuInit NcuInitSpace;

  // 创建角色屏幕（30行 x 60列）
  constexpr size_t Rows = 30;      ///< 游戏区域行数
  constexpr size_t Cols = 60;      ///< 游戏区域列数
  constexpr size_t SnakeCount = 4; ///< AI蛇的数量
  constexpr size_t AppleCount = 2; ///< 苹果数量
  constexpr int GameDelay = 50;    ///< 游戏刷新延迟（毫秒）

  RoleScreen::Element defaultElem(ColorChar(' ', Color::DEFAULT, Color::WHITE), nullptr);
  RoleScreen scr(Rows, Cols, defaultElem);

  // 创建4条AI蛇（非玩家控制）
  std::vector<std::unique_ptr<Snake>> snakes;
  snakes.reserve(SnakeCount);
  for (size_t i = 0; i < SnakeCount; ++i) {
    snakes.push_back(std::make_unique<Snake>(scr, false)); // false表示AI蛇
  }

  // 创建2个苹果
  std::vector<std::unique_ptr<Apple>> apples;
  apples.reserve(AppleCount);
  for (size_t i = 0; i < AppleCount; ++i) {
    apples.push_back(std::make_unique<Apple>(scr));
  }

  // 初始显示
  scr.print(true);

  // 游戏主循环：直到所有蛇都死亡
  bool allDead = false;
  while (!allDead) {
    // 轮流更新所有AI蛇的状态
    for (auto &snake : snakes) {
      snake->update();
    }

    // 刷新屏幕显示
    scr.print(true);

    // 检查并显示所有蛇的状态
    allDead = true;
    int lineOffset = 0;
    for (const auto &snake : snakes) {
      const bool isAlive = !snake->isDeadCheck();
      if (isAlive) {
        allDead = false;
      }

      // 显示蛇编号和分数
      attron(COLOR_PAIR(static_cast<int>(snake->getColor()) + 33));
      mvprintw(static_cast<int>(Rows + lineOffset), 0, "%d号蛇:%zu ", lineOffset + 1, snake->score());
      attroff(COLOR_PAIR(static_cast<int>(snake->getColor()) + 33));

      // 显示存活/死亡状态
      if (isAlive) {
        attron(COLOR_PAIR(static_cast<int>(Color::GREEN) + 1));
        printw("存活");
        attroff(COLOR_PAIR(static_cast<int>(Color::GREEN) + 1));
      } else {
        attron(COLOR_PAIR(static_cast<int>(Color::RED) + 1));
        printw("死亡");
        attroff(COLOR_PAIR(static_cast<int>(Color::RED) + 1));
      }
      ++lineOffset;
    }
    // 清除旧的状态信息并显示提示
    eraseLine(static_cast<int>(Rows + snakes.size()));
    mvprintw(static_cast<int>(Rows + snakes.size()), 0, "按[Esc]键暂停游戏");
    refresh();

    // 设置输入超时并检测暂停键
    timeout(GameDelay);
    const int key = getch();
    if (key == 27) { // Esc键
      // 进入暂停模式
      timeout(-1);
      eraseLine(static_cast<int>(Rows + snakes.size()));
      mvprintw(static_cast<int>(Rows + snakes.size()), 0, "游戏暂停中,按任意键继续...");
      eraseLine(static_cast<int>(Rows + snakes.size()) + 1);
      mvprintw(static_cast<int>(Rows + snakes.size()) + 1, 0, "按[Esc]键退出游戏");

#ifdef DEBUG

      // 调试模式：显示调试控制台
      bool shouldContinue = true;
      while (true) {
        const auto com = snakeDebug(Cols);
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
              mvprintw(infoLine++, static_cast<int>(Cols), "编号:%zu", snakeIndex + 1);
              mvprintw(infoLine++, static_cast<int>(Cols), "长度:%zu", snakes[snakeIndex]->score());
              mvprintw(infoLine++, static_cast<int>(Cols), "颜色:%s", snakes[snakeIndex]->getColorName());
              mvprintw(infoLine++, static_cast<int>(Cols), "待增加长度:%d", snakes[snakeIndex]->getEdit());
              infoLine++;
            }
          } else {
            // 显示指定蛇的信息
            const int snakeIndex = std::stoi(com.second[0]) - 1;
            if (snakeIndex >= 0 && snakeIndex < static_cast<int>(snakes.size())) {
              mvprintw(infoLine++, static_cast<int>(Cols), "长度:%zu", snakes[snakeIndex]->score());
              mvprintw(infoLine++, static_cast<int>(Cols), "颜色:%s", snakes[snakeIndex]->getColorName());
            } else {
              mvprintw(infoLine++, static_cast<int>(Cols), "错误: 无效的蛇编号");
            }
          }
        } else if (com.first == "kill") {
          // 强制杀死指定蛇
          if (!com.second.empty()) {
            const int snakeIndex = std::stoi(com.second[0]) - 1;
            if (snakeIndex >= 0 && snakeIndex < static_cast<int>(snakes.size())) {
              snakes[snakeIndex]->kill();
            } else {
              mvprintw(infoLine++, static_cast<int>(Cols), "错误: 无效的蛇编号");
            }
          } else {
            mvprintw(infoLine++, static_cast<int>(Cols), "错误: 请提供蛇编号");
          }
        } else if (com.first == "edit") {
          if (com.second.size() < 3) {
            mvprintw(infoLine++, Cols, "错误：参数过少");
          } else {
            int num = std::stoi(com.second[0]) - 1;
            std::string mode = com.second[1];
            if (mode == "l") {
              int val = std::stoi(com.second[2]);
              if (-val >= static_cast<int>(snakes[num]->score())) {
                mvprintw(infoLine++, Cols, "错误：不允许长度小于1的蛇类出现");
              }
              // 调整蛇的长度
              snakes[num]->edit(val);
              mvprintw(infoLine++, Cols, "已设置蛇%d的长度调整为:%d", num + 1, val);
            } else if (mode == "d") {
              // TODO 修复强制转向功能（update函数）
              // TODO 实现转向累加
              // TODO 运行 /code-document-helper
              // 强制转向：支持 u(上), d(下), l(左), r(右) 或数字 0-3
              std::string dirStr = com.second[2];
              Snake::dir_t newDir = 0;
              bool validDir = true;

              if (dirStr == "u" || dirStr == "U") {
                newDir = 0; // 上
              } else if (dirStr == "r" || dirStr == "R") {
                newDir = 1; // 右
              } else if (dirStr == "d" || dirStr == "D") {
                newDir = 2; // 下
              } else if (dirStr == "l" || dirStr == "L") {
                newDir = 3; // 左
              } else {
                // 尝试解析为数字
                try {
                  int dirVal = std::stoi(dirStr);
                  if (dirVal >= 0 && dirVal <= 3) {
                    newDir = static_cast<Snake::dir_t>(dirVal);
                  } else {
                    validDir = false;
                  }
                } catch (...) {
                  validDir = false;
                }
              }

              if (validDir) {
                snakes[num]->turn(newDir);
                const char *dirNames[] = {"上", "右", "下", "左"};
                mvprintw(infoLine++, Cols, "已设置蛇%d强制转向:%s", num + 1, dirNames[newDir]);
              } else {
                mvprintw(infoLine++, Cols, "错误：方向值必须为 u/d/l/r 或 0-3");
              }
            } else {
              mvprintw(infoLine++, Cols, "错误：不支持的属性 (支持: l=长度, d=方向)");
            }
          }
        } // TODO 实现让蛇活过来

        mvprintw(infoLine, static_cast<int>(Cols), "按任意键继续");
        getch();

        for (int i = 0; i <= infoLine; i++) {
          eraseLine(i, Cols);
        }
      }

      if (!shouldContinue) {
        break; // 退出游戏主循环
      }

#else

      // 非调试模式：简单暂停
      if (getch() == 27) {
        break; // 再次按Esc退出游戏
      }

#endif // DEBUG
    }
    // 恢复输入模式并清除额外行
    timeout(-1);
    eraseLine(static_cast<int>(Rows + snakes.size()) + 1);

    // 短暂延迟，控制游戏速度
    usleep(GameDelay * 1000); // 转换为微秒
  }

  // 游戏结束，清理显示
  eraseLine(static_cast<int>(Rows + snakes.size()));
  eraseLine(static_cast<int>(Rows + snakes.size()) + 1);
  eraseLine(LINES - 2);
  mvprintw(LINES - 2, 0, "游戏结束");
  waitKey("退出");

  return 0;
}

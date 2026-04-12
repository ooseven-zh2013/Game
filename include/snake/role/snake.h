#ifndef SNAKE_H
#define SNAKE_H
#pragma once

#include "common/game/basic/role.h"
#include <deque>
#include <string>
#include <vector>

class Snake : Role {
public:
  using xy = std::pair<int, int>;
  using dir_t = unsigned char;

  /**
   * @brief 创建一条蛇
   *
   * @param scr_ 屏幕
   */
  Snake(RoleScreen &scr_, bool isPlayer_ = false);

  /**
   * @brief 让蛇死亡
   *
   * 将蛇标记为死亡状态，并将蛇身所有格子变为灰色（BRIGHT_BLACK）。
   */
  void kill() override;

  /**
   * @brief 更新蛇的状态
   *
   * 如果蛇已死亡则直接返回。否则处理移动逻辑并更新屏幕显示。
   * 区分玩家控制和AI控制两种模式。
   */
  void update();

  /**
   * @brief 让蛇向前移动一步
   *
   * 根据当前方向移动蛇头，移除蛇尾（除非吃到苹果）。
   * 此函数为内联函数以提高性能。
   *
   * @param eatApple 是否吃到苹果（吃到则不移除蛇尾）
   */
  inline void forward(bool eatApple = false);

  /**
   * @brief 获取角色类型
   *
   * @return std::string 返回 "Snake"
   */
  std::string type() override;

  /**
   * @brief 检查蛇是否已死亡
   *
   * @return true 蛇已死亡
   * @return false 蛇仍然存活
   */
  inline bool isDeadCheck() const { return isDead; }

  inline size_t score() const { return body.size(); }

private:
  bool isPlayer;                                                     ///< 是否为玩家控制的蛇
  bool isDead;                                                       ///< 蛇是否已死亡
  std::deque<xy> body;                                               ///< 蛇身位置队列（队首为蛇头）
  dir_t dir;                                                         ///< 当前移动方向（0-3对应dirXy数组索引）
  std::vector<xy> apples;                                            ///< 屏幕上所有苹果的坐标列表（每帧更新）
  Color snakeColor;                                                  ///< 此蛇的颜色
  static constexpr xy dirXy[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; ///< 方向向量数组：上、右、下、左
  static constexpr int COLOR_COUNT = 16;                             ///< 可用颜色数量（Color枚举值的总数）
  static bool colorUsed[COLOR_COUNT];                                ///< 颜色占用标记数组（索引对应Color枚举值）

  /**
   * @brief 改变蛇的移动方向
   *
   * @param newDir 新的方向（0-3）
   */
  inline void changeDir(dir_t newDir);

  /**
   * @brief 寻找吃到苹果的最优路径方向
   *
   * 使用BFS找到到最近苹果的最短路径，并选择第一步的方向。
   * 同时检查移动后是否仍有逃生路径以避免进入死路。
   */
  void findPath();

  /**
   * @brief 使用BFS寻找从起点到目标点的最短路径
   *
   * @param startX 起点X坐标
   * @param startY 起点Y坐标
   * @param targetX 目标X坐标
   * @param targetY 目标Y坐标
   * @return std::vector<dir_t> 路径方向序列（空表示不可达）
   */
  std::vector<dir_t> bfsPath(int startX, int startY, int targetX, int targetY);

  /**
   * @brief 检查从某位置出发是否有足够的可行走空间（逃生路径）
   *
   * 使用Flood Fill算法计算从给定位置出发可以到达的格子数量。
   * 如果可达空间太小，说明可能进入死路。
   *
   * @param startX 起点X坐标
   * @param startY 起点Y坐标
   * @return int 可达的格子数量
   */
  int floodFillCount(int startX, int startY) const;

  /**
   * @brief 检查某个位置是否是可行走的（非障碍物）
   *
   * @param x X坐标
   * @param y Y坐标
   * @return true 位置可行走
   * @return false 位置被阻挡（边界、蛇身等）
   */
  inline bool isWalkable(int x, int y) const;
};

#endif // SNAKE_H
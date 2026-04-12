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
   * @brief 获取蛇的颜色
   *
   * 根据是否为玩家控制返回不同的颜色。
   *
   * @return Color 玩家蛇返回BLUE，AI蛇返回GREEN
   */
  inline Color getColor() const;

  /**
   * @brief 获取角色类型
   *
   * @return std::string 返回 "Snake"
   */
  std::string type() override;

private:
  bool isPlayer;                                                     ///< 是否为玩家控制的蛇
  bool isDead;                                                       ///< 蛇是否已死亡
  std::deque<xy> body;                                               ///< 蛇身位置队列（队首为蛇头）
  dir_t dir;                                                         ///< 当前移动方向（0-3对应dirXy数组索引）
  std::vector<xy> apples;                                            ///< 屏幕上所有苹果的坐标列表（每帧更新）
  static constexpr xy dirXy[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; ///< 方向向量数组：上、右、下、左

  /**
   * @brief 改变蛇的移动方向
   *
   * @param newDir 新的方向（0-3）
   */
  inline void changeDir(dir_t newDir);

  /**
   * @brief 寻找吃到苹果的最优路径方向
   *
   * 遍历三个可行方向（排除掉头），评估每个方向的pathSum值，
   * 选择推荐程度最高的方向。如果是死路则保持原方向。
   */
  void findPath();

  /**
   * @brief 计算沿某个方向移动的推荐程度评分
   *
   * 评分越高表示越不推荐该方向。计算方式为：
   * 从新位置出发，对所有可行子方向，累加到所有苹果的曼哈顿距离之和。
   * 死路返回INT_MAX。
   *
   * @param newDir 要评估的方向
   * @return long long 推荐程度评分（越大越不推荐）
   */
  inline long long pathSum(dir_t newDir);

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
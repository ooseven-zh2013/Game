/**
 * @file snake.hpp
 * @brief 蛇角色类
 *
 * Snake是游戏的核心角色，支持玩家控制和AI自动控制。
 * AI使用BFS寻路算法和Flood Fill安全检查来实现智能移动。
 */

#ifndef SNAKE_HPP
#define SNAKE_HPP
#pragma once

#include "common/game/basic/role.hpp"
#include "common/maths/random.hpp"
#include <climits>
#include <deque>
#include <queue>
#include <set>
#include <string>
#include <vector>

/**
 * @brief 蛇角色类
 *
 * Snake继承自Role，是游戏的主要角色。
 * 支持两种模式：
 *   - 玩家控制模式（isPlayer = true）：等待用户输入
 *   - AI控制模式（isPlayer = false）：自动寻路和避障
 *
 * AI算法特点：
 *   - 使用BFS寻找最短路径到苹果
 *   - 使用Flood Fill检查逃生空间避免死路
 *   - 优先保证生存，其次追求吃苹果
 */
class Snake : Role {
public:
  using xy = std::pair<int, int>;
  using dir_t = unsigned char;

  /**
   * @brief 创建一条蛇
   *
   * @param scr_ 屏幕
   */
  inline Snake(RoleScreen &scr_, bool isPlayer_ = false);

  /**
   * @brief 让蛇死亡
   *
   * 将蛇标记为死亡状态，并将蛇身所有格子变为灰色（BRIGHT_BLACK）。
   */
  void kill() override;

  /**
   * @brief 复活已死亡的蛇
   *
   * 原地复活蛇，保持原有的长度、位置和方向不变。
   * 如果复活位置被其他蛇或障碍物占据，则复活失败，蛇保持死亡状态。
   * 只能在蛇处于死亡状态时调用。
   */
  void respawn();

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

  /**
   * @brief 获取蛇的当前分数（身体长度）
   *
   * @return size_t 蛇的身体长度，即得分
   */
  inline size_t score() const { return body.size(); }

  /**
   * @brief 获取蛇的颜色
   *
   * @return Color 蛇的颜色
   */
  inline Color getColor() const { return snakeColor; }

  /**
   * @brief 获取蛇颜色的字符串表示
   *
   * @return const char* 颜色名称字符串
   */
  inline const char *getColorName() const;

#ifdef DEBUG

  inline void edit(int delta) { add += delta; }

  inline int getEdit() const { return add; }

  /**
   * @brief 获取待转向方向字符串（调试用）
   *
   * @return const char* 方向字符串（←↓→↑None）
   */
  inline const char *getForcedDirName() const {
    if (!forceTurn) {
      return "None";
    }
    static const char *dirNames[] = {"←", "↓", "→", "↑"};
    return dirNames[forcedDir];
  }

  /**
   * @brief 强制设置蛇的移动方向（调试用）
   *
   * 在下次update时，蛇会强制转向到指定的方向。
   * 方向值：0=左, 1=下, 2=右, 3=上
   *
   * @param newDir 新的方向（0-3），-1表示取消强制转向
   */
  inline void turn(int newDir) {
    if (newDir == -1) {
      forceTurn = false;
    }
    if (newDir >= 0 && newDir < 4) {
      forceTurn = true;
      forcedDir = static_cast<dir_t>(newDir);
    }
  }

#endif // DEBUG

private:
#ifdef DEBUG

  int add;         ///< 调试模式下的长度调整值
  bool forceTurn;  ///< 是否强制转向标志
  dir_t forcedDir; ///< 强制转向的目标方向

#endif                                                               // DEBUG
  bool isPlayer;                                                     ///< 是否为玩家控制的蛇
  bool isDead;                                                       ///< 蛇是否已死亡
  std::deque<xy> body;                                               ///< 蛇身位置队列（队首为蛇头）
  dir_t dir;                                                         ///< 当前移动方向（0-3对应dirXy数组索引）
  std::vector<xy> apples;                                            ///< 屏幕上所有苹果的坐标列表（每帧更新）
  Color snakeColor;                                                  ///< 此蛇的颜色
  static constexpr xy dirXy[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; ///< 方向向量数组：索引0=上, 1=右, 2=下, 3=左
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

// Static member definition
bool Snake::colorUsed[Snake::COLOR_COUNT] = {false, true,  false, false, false, false, false, true,
                                             true,  false, false, false, false, false, false, false};

// Implementation from snake.cpp
inline Snake::Snake(RoleScreen &scr_, bool isPlayer_) : Role(scr_), isPlayer(isPlayer_), isDead(false) {
  int x, y;
  x = random(0, static_cast<int>(scr.x_size()) - 1);
  y = random(0, static_cast<int>(scr.y_size()) - 1);
  dir = random(0, 3);
  // 寻找空白位置（背景色为WHITE）
  while (scr[x][y].first.getBg() != Color::WHITE) {
    x = random(0, static_cast<int>(scr.x_size()) - 1);
    y = random(0, static_cast<int>(scr.y_size()) - 1);
  }

  // 随机选择颜色，直到找到未被占用的颜色
  // 颜色索引范围 0-15（对应Color枚举值）
  int colorIdx;
  do {
    colorIdx = random(0, 15);
  } while (colorUsed[colorIdx]);

  colorUsed[colorIdx] = true;
  snakeColor = static_cast<Color>(colorIdx);

  body.emplace_back(x, y);
  scr[x][y].first.setBg(snakeColor);

#ifdef DEBUG

  add = 0;
  forceTurn = false;
  forcedDir = 0;

#endif // DEBUG
}

inline void Snake::kill() {
  isDead = true;
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(Color::BRIGHT_BLACK);
    scr.at(pos.first, pos.second).second = nullptr; // 清除角色指针，释放格子占用
  }
}

inline void Snake::respawn() {
  if (!isDead) {
    return; // 如果蛇还活着，不需要复活
  }

  // 保存原有的蛇身位置和方向
  auto savedBody = body;
  dir_t savedDir = dir;

  // 重置死亡状态
  isDead = false;

  // 恢复蛇身到屏幕上
  for (const auto &pos : savedBody) {
    // 检查位置是否仍然可用（没有被其他蛇或障碍物占据）
    if (pos.first >= 0 && pos.first < static_cast<int>(scr.x_size()) && pos.second >= 0 &&
        pos.second < static_cast<int>(scr.y_size()) && scr[pos.first][pos.second].second == nullptr) {
      scr[pos.first][pos.second].first.setBg(snakeColor);
      scr[pos.first][pos.second].second = this;
    } else {
      // 如果位置被占用，复活失败，蛇立即死亡
      isDead = true;
      // 清除已放置的部分
      for (const auto &placedPos : body) {
        scr.at(placedPos.first, placedPos.second).first.setBg(Color::BRIGHT_BLACK);
        scr.at(placedPos.first, placedPos.second).second = nullptr;
      }
      body.clear();
      return;
    }
  }

  // 恢复蛇身数据
  body = savedBody;
  dir = savedDir;

#ifdef DEBUG
  add = 0;
  forceTurn = false;
  forcedDir = 0;
#endif // DEBUG
}

inline bool Snake::isWalkable(int x, int y) const {
  // 检查边界
  if (x < 0 || x >= static_cast<int>(scr.x_size()) || y < 0 || y >= static_cast<int>(scr.y_size())) {
    return false;
  }
  // 检查是否是死蛇尸体（背景色为BRIGHT_BLACK）
  if (scr[x][y].first.getBg() == Color::BRIGHT_BLACK) {
    return false; // 死蛇尸体不可行走
  }
  // 检查是否有其他蛇的身体阻挡
  if (scr[x][y].second != nullptr && scr[x][y].second->type() == "Snake") {

#ifdef DEBUG

    if (add <= 0) {

#endif // DEBUG

      // 如果是自己蛇的身体，需要检查是否是蛇尾（蛇尾在移动时会移开）
      if (scr[x][y].second == this) {
        // 检查该位置是否是自己的蛇尾
        const auto &tail = body.back();
        if (x == tail.first && y == tail.second) {
          return true; // 蛇尾位置可行走（移动时会移开）
        }
      }

#ifdef DEBUG
    }

#endif // DEBUG

    return false; // 其他蛇或自己身体的其他部分都不可行走
  }
  return true;
}

inline void Snake::changeDir(dir_t newDir) { dir = newDir; }

inline std::vector<Snake::dir_t> Snake::bfsPath(int startX, int startY, int targetX, int targetY) {
  // BFS队列：存储位置和到达该位置的路径
  std::queue<std::pair<xy, std::vector<dir_t>>> q;
  std::set<xy> visited;

  q.push({{startX, startY}, {}});
  visited.insert({startX, startY});

  while (!q.empty()) {
    auto [pos, path] = q.front();
    q.pop();

    // 检查是否到达目标
    if (pos.first == targetX && pos.second == targetY) {
      return path;
    }

    // 尝试四个方向
    for (dir_t d = 0; d < 4; ++d) {
      int nx = pos.first + dirXy[d].first;
      int ny = pos.second + dirXy[d].second;
      xy nextPos = {nx, ny};

      // 检查是否已访问或不可行走
      if (visited.count(nextPos) > 0 || !isWalkable(nx, ny)) {
        continue;
      }

      visited.insert(nextPos);
      std::vector<dir_t> newPath = path;
      newPath.push_back(d);
      q.push({nextPos, newPath});
    }
  }

  // 无法到达目标
  return {};
}

inline int Snake::floodFillCount(int startX, int startY) const {
  std::queue<xy> q;
  std::set<xy> visited;

  q.push({startX, startY});
  visited.insert({startX, startY});

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();

    // 尝试四个方向
    for (dir_t d = 0; d < 4; ++d) {
      int nx = x + dirXy[d].first;
      int ny = y + dirXy[d].second;
      xy nextPos = {nx, ny};

      // 检查是否已访问或不可行走
      if (visited.count(nextPos) > 0 || !isWalkable(nx, ny)) {
        continue;
      }

      visited.insert(nextPos);
      q.push(nextPos);
    }
  }

  return static_cast<int>(visited.size());
}

inline void Snake::findPath() {
  if (apples.empty()) {
    return; // 没有苹果，保持当前方向
  }

  int headX = body.front().first;
  int headY = body.front().second;

  // 找到最近的苹果（使用BFS实际距离而非曼哈顿距离）
  int shortestDist = INT_MAX;
  std::vector<dir_t> bestPath;

  for (size_t i = 0; i < apples.size(); ++i) {
    auto path = bfsPath(headX, headY, apples[i].first, apples[i].second);
    if (!path.empty() && static_cast<int>(path.size()) < shortestDist) {
      shortestDist = static_cast<int>(path.size());
      bestPath = path;
    }
  }

  // 如果找到了到某个苹果的路径
  if (!bestPath.empty()) {
    dir_t firstStep = bestPath[0];

    // 安全检查：模拟移动后的状态，检查是否仍有逃生路径
    int nextX = headX + dirXy[firstStep].first;
    int nextY = headY + dirXy[firstStep].second;

    // 计算需要的最小逃生空间（蛇身长度 + 一些余量）
    int minSafeSpace = static_cast<int>(body.size()) + 5;

    // 临时移动蛇头来检查逃生空间
    int floodCount = floodFillCount(nextX, nextY);

    if (floodCount >= minSafeSpace) {
      // 安全，朝苹果移动
      changeDir(firstStep);
      return;
    }
  }

  // 如果无法安全到达苹果，或者没有路径到苹果
  // 则选择最安全的方向（有最大逃生空间的方向）
  dir_t oppositeDir = (dir + 2) % 4; // 不能掉头
  dir_t safestDir = dir;             // 默认保持当前方向
  int maxSpace = 0;

  for (dir_t d = 0; d < 4; ++d) {
    if (d == oppositeDir)
      continue;

    int nx = headX + dirXy[d].first;
    int ny = headY + dirXy[d].second;

    if (isWalkable(nx, ny)) {
      int space = floodFillCount(nx, ny);
      if (space > maxSpace) {
        maxSpace = space;
        safestDir = d;
      }
    }
  }

  changeDir(safestDir);
}

inline void Snake::update() {
  if (isDead) {
    return;
  }

#ifdef DEBUG
  // 如果有强制转向指令，优先执行
  if (forceTurn) {
    dir = forcedDir;
  } else {
#endif // DEBUG

    // 模拟玩家用眼找苹果的过程：遍历整张屏幕，记录所有苹果的位置
    apples.clear();
    for (int row = 0; row < static_cast<int>(scr.x_size()); ++row) {
      for (int col = 0; col < static_cast<int>(scr.y_size()); ++col) {
        if (scr[row][col].second != nullptr && scr[row][col].second->type() == "Apple") {
          apples.emplace_back(row, col);
        }
      }
    }

    // 处理蛇的移动逻辑：区分玩家控制和AI控制
    if (isPlayer) {
      // TODO 玩家控制移动
    } else {
      // AI控制移动：自动寻找最优路径
      findPath();
    }

#ifdef DEBUG
  }

#endif

  // 计算新蛇头位置
  int nx = body.front().first + dirXy[dir].first;
  int ny = body.front().second + dirXy[dir].second;

  // 先检查是否越界或碰到障碍物
  if (!isWalkable(nx, ny)) {
    kill();
    return;
  }

  // 检测碰撞（此时已确保坐标合法）
  bool eatApple = false;
  if (scr[nx][ny].second != nullptr && scr[nx][ny].second->type() == "Apple") {
    // 吃到苹果
    eatApple = true;
    scr[nx][ny].second->kill();
  }

  // 向前移动
  forward(eatApple);

  // 更新蛇在屏幕上的显示
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(snakeColor);
  }
}

inline void Snake::forward(bool eatApple) {
  // 保存旧蛇头位置用于计算新位置
  int oldHeadX = body.front().first;
  int oldHeadY = body.front().second;

#ifdef DEBUG

  if (add > 0) {
    add -= !eatApple;
  } else {

#endif // DEBUG

    if (!eatApple) {
      const auto &[lx, ly] = body.back();
      scr[lx][ly].first.setBg(Color::WHITE);
      scr[lx][ly].second = nullptr;
      body.pop_back();
    }

#ifdef DEBUG
  }
  if (add < 0) {
    ++add;
  } else {

#endif // DEBUG

    // 添加新蛇头
    int nx = oldHeadX + dirXy[dir].first;
    int ny = oldHeadY + dirXy[dir].second;
    scr[nx][ny].first.setBg(snakeColor);
    scr[nx][ny].second = this;
    body.emplace_front(nx, ny);

#ifdef DEBUG
  }
#endif // DEBUG
}

inline std::string Snake::type() { return "Snake"; }

inline const char *Snake::getColorName() const {
  static const char *colorNames[] = {"DEFAULT",     "BLACK",        "RED",           "GREEN",       "YELLOW",
                                     "BLUE",        "MAGENTA",      "CYAN",          "WHITE",       "BRIGHT_BLACK",
                                     "BRIGHT_RED",  "BRIGHT_GREEN", "BRIGHT_YELLOW", "BRIGHT_BLUE", "BRIGHT_MAGENTA",
                                     "BRIGHT_CYAN", "BRIGHT_WHITE"};
  return colorNames[static_cast<int>(snakeColor) + 1];
}

#endif // SNAKE_HPP

#include "snake/role/snake.h"
#include "common/maths/random.hpp"
#include <climits>
#include <queue>
#include <set>

// 静态成员定义：颜色占用标记数组，初始全部为false
// 白色（7）和红色（1）默认被占用，避免与背景或苹果冲突
// BRIGHT_BLACK（8）默认被占用，因为它是死蛇尸体的颜色
bool Snake::colorUsed[Snake::COLOR_COUNT] = {false, true,  false, false, false, false, false, true,
                                             true,  false, false, false, false, false, false, false};

/**
 * @brief Snake构造函数
 *
 * 在屏幕上随机选择一个空白位置生成蛇的初始身体（单格）。
 * 玩家蛇为蓝色，AI蛇为绿色。
 *
 * @param scr_ 绑定的角色屏幕
 * @param isPlayer_ 是否为玩家控制的蛇
 */
Snake::Snake(RoleScreen &scr_, bool isPlayer_) : Role(scr_), isPlayer(isPlayer_), isDead(false) {
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
}

/**
 * @brief 使蛇死亡
 *
 * 将蛇标记为死亡状态，清除屏幕上的所有占用，并将蛇身格子变为灰色（BRIGHT_BLACK）。
 */
void Snake::kill() {
  isDead = true;
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(Color::BRIGHT_BLACK);
    scr.at(pos.first, pos.second).second = nullptr; // 清除角色指针，释放格子占用
  }
}

/**
 * @brief 检查某个位置是否是可行走的（非障碍物）
 *
 * @param x X坐标
 * @param y Y坐标
 * @return true 位置可行走
 * @return false 位置被阻挡（边界、蛇身、死蛇尸体等）
 */
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
    // 如果是自己蛇的身体，需要检查是否是蛇尾（蛇尾在移动时会移开）
    if (scr[x][y].second == this) {
      // 检查该位置是否是自己的蛇尾
      const auto &tail = body.back();
      if (x == tail.first && y == tail.second) {
        return true; // 蛇尾位置可行走（移动时会移开）
      }
    }
    return false; // 其他蛇或自己身体的其他部分都不可行走
  }
  return true;
}

/**
 * @brief 改变蛇的移动方向
 *
 * @param newDir 新的方向（0-3）
 */
inline void Snake::changeDir(dir_t newDir) { dir = newDir; }

/**
 * @brief 使用BFS寻找从起点到目标点的最短路径
 *
 * @param startX 起点X坐标
 * @param startY 起点Y坐标
 * @param targetX 目标X坐标
 * @param targetY 目标Y坐标
 * @return std::vector<dir_t> 路径方向序列（空表示不可达）
 */
std::vector<Snake::dir_t> Snake::bfsPath(int startX, int startY, int targetX, int targetY) {
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
int Snake::floodFillCount(int startX, int startY) const {
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

/**
 * @brief 寻找吃到苹果的最优路径方向
 *
 * 使用BFS找到到最近苹果的最短路径，并选择第一步的方向。
 * 同时检查移动后是否仍有逃生路径以避免进入死路。
 */
void Snake::findPath() {
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

/**
 * @brief 更新蛇的状态
 *
 * 如果蛇已死亡则直接返回。否则处理移动逻辑并更新屏幕显示。
 * AI蛇会自动寻找苹果，玩家蛇等待外部输入。
 */
void Snake::update() {
  if (isDead) {
    return;
  }

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

/**
 * @brief 让蛇向前移动一步
 *
 * 移除蛇尾，根据当前方向在蛇头前方添加新格子。
 * 如果吃到苹果则不移除蛇尾（蛇身增长）。
 *
 * @param eatApple 是否吃到苹果（吃到则不移除蛇尾）
 */
inline void Snake::forward(bool eatApple) {
  // 保存旧蛇头位置用于计算新位置
  int oldHeadX = body.front().first;
  int oldHeadY = body.front().second;

  if (!eatApple) {
    const auto &[lx, ly] = body.back();
    scr[lx][ly].first.setBg(Color::WHITE);
    scr[lx][ly].second = nullptr;
    body.pop_back();
  }

  // 添加新蛇头
  int nx = oldHeadX + dirXy[dir].first;
  int ny = oldHeadY + dirXy[dir].second;
  scr[nx][ny].first.setBg(snakeColor);
  scr[nx][ny].second = this;
  body.emplace_front(nx, ny);
}

/**
 * @brief 获取角色类型
 *
 * @return std::string 返回 "Snake"
 */
std::string Snake::type() { return "Snake"; }

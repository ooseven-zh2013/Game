#include "snake/role/snake.h"
#include "common/maths/random.hpp"
#include <climits>

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
  body.emplace_back(x, y);
  scr[x][y].first.setBg(isPlayer_ ? Color::BLUE : Color::GREEN);
}

/**
 * @brief 使蛇死亡
 *
 * 将蛇标记为死亡状态，并将蛇身所有格子变为灰色（BRIGHT_BLACK）。
 */
void Snake::kill() {
  isDead = true;
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(Color::BRIGHT_BLACK);
  }
}

/**
 * @brief 检查某个位置是否是可行走的（非障碍物）
 *
 * @param x X坐标
 * @param y Y坐标
 * @return true 位置可行走
 * @return false 位置被阻挡（边界、蛇身等）
 */
inline bool Snake::isWalkable(int x, int y) const {
  // 检查边界
  if (x < 0 || x >= static_cast<int>(scr.x_size()) || y < 0 || y >= static_cast<int>(scr.y_size())) {
    return false;
  }
  // 检查是否有蛇身阻挡（排除自己，因为移动后蛇尾会移开）
  if (scr[x][y].second != nullptr && scr[x][y].second->type() == "Snake") {
    return false;
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
 * @brief 计算沿某个方向移动的推荐程度评分
 *
 * 评分越高表示越不推荐该方向。计算方式为：
 * 从新位置出发，对所有可行子方向，累加到所有苹果的曼哈顿距离之和。
 * 死路返回INT_MAX。
 *
 * @param newDir 要评估的方向
 * @return long long 推荐程度评分（越大越不推荐）
 */
inline long long Snake::pathSum(dir_t newDir) {
  // 计算沿newDir方向前进后的新蛇头坐标
  int nx = body.front().first + dirXy[newDir].first;
  int ny = body.front().second + dirXy[newDir].second;

  // 收集从新位置出发的三个可行方向（排除掉头方向）
  std::vector<dir_t> validDirs;
  dir_t oppositeDir = (newDir + 2) % 4; // 相反方向（掉头）
  for (dir_t d = 0; d < 4; ++d) {
    if (d == oppositeDir)
      continue; // 不能掉头
    int subNx = nx + dirXy[d].first;
    int subNy = ny + dirXy[d].second;
    if (isWalkable(subNx, subNy)) {
      validDirs.push_back(d);
    }
  }

  // 如果处理队列为空（死路），返回INT_MAX表示非常不推荐
  if (validDirs.empty()) {
    return INT_MAX;
  }

  // 对于每个可行子方向，计算到所有苹果的曼哈顿距离之和
  // 然后将所有方向的值再相加
  long long totalSum = 0;
  for (dir_t d : validDirs) {
    // 计算沿子方向移动后的位置
    int lx = nx + dirXy[d].first;
    int ly = ny + dirXy[d].second;

    // 累加该位置到所有苹果的曼哈顿距离
    // 使用long long避免int溢出风险
    long long distSum = 0;
    for (const auto &applePos : apples) {
      // 曼哈顿距离：|x1-x2| + |y1-y2|
      distSum += std::abs(lx - applePos.first) + std::abs(ly - applePos.second);
    }
    totalSum += distSum;
  }

  return totalSum;
}

/**
 * @brief 寻找吃到苹果的最优路径方向
 *
 * 遍历三个可行方向（排除掉头），评估每个方向的pathSum值，
 * 选择推荐程度最高的方向。如果是死路则保持原方向。
 */
void Snake::findPath() {
  // 收集三个可行方向（排除掉头方向）
  std::vector<dir_t> validDirs;
  dir_t oppositeDir = (dir + 2) % 4; // 相反方向（掉头）
  for (dir_t d = 0; d < 4; ++d) {
    if (d == oppositeDir)
      continue; // 不能掉头
    int nx = body.front().first + dirXy[d].first;
    int ny = body.front().second + dirXy[d].second;
    if (isWalkable(nx, ny)) {
      validDirs.push_back(d);
    }
  }

  // 如果处理队列为空（死路），不改变方向
  if (validDirs.empty()) {
    return;
  }

  // 如果只有一个可行方向（唯一路径），直接取那个方向
  if (validDirs.size() == 1) {
    changeDir(validDirs[0]);
    return;
  }

  // 对于每个方向，检查沿着这个方向前进一次是否可以吃到苹果
  for (dir_t d : validDirs) {
    int nx = body.front().first + dirXy[d].first;
    int ny = body.front().second + dirXy[d].second;
    if (scr[nx][ny].second != nullptr && scr[nx][ny].second->type() == "Apple") {
      // 可以直接吃到苹果，这是最优解
      changeDir(d);
      return;
    }
  }

  // 否则，计算每个方向的pathSum，选择最小的
  dir_t bestDir = validDirs[0];
  long long bestSum = pathSum(validDirs[0]);
  for (size_t i = 1; i < validDirs.size(); ++i) {
    long long currentSum = pathSum(validDirs[i]);
    if (currentSum < bestSum) {
      bestSum = currentSum;
      bestDir = validDirs[i];
    }
  }
  changeDir(bestDir);
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

  if (nx < 0 || nx >= static_cast<int>(scr.x_size()) || ny < 0 || ny >= static_cast<int>(scr.y_size())) {
    kill();
    return;
  }

  // 检测碰撞
  bool eatApple = false;
  if (scr[nx][ny].second != nullptr) {
    if (scr[nx][ny].second->type() == "Apple") {
      // 吃到苹果
      eatApple = true;
      scr[nx][ny].second->kill();
    } else if (scr[nx][ny].second->type() == "Snake") {
      // 撞到其他蛇，死亡
      kill();
      return;
    }
  }

  // 向前移动
  forward(eatApple);

  // 更新蛇在屏幕上的显示
  for (const auto &pos : body) {
    scr.at(pos.first, pos.second).first.setBg(getColor());
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
  if (!eatApple) {
    // 清除蛇尾（没吃到苹果时蛇身长度不变）
    const auto &[lx, ly] = body.back();
    scr[lx][ly].first.setBg(Color::WHITE);
    scr[lx][ly].second = nullptr;
    body.pop_back();
  }

  // 添加新蛇头
  int nx = body.front().first + dirXy[dir].first;
  int ny = body.front().second + dirXy[dir].second;
  scr[nx][ny].first.setBg(getColor());
  scr[nx][ny].second = this;
  body.emplace_front(nx, ny);
}

/**
 * @brief 获取蛇的颜色
 *
 * @return Color 玩家蛇返回BLUE，AI蛇返回GREEN
 */
inline Color Snake::getColor() const { return isPlayer ? Color::BLUE : Color::GREEN; }

/**
 * @brief 获取角色类型
 *
 * @return std::string 返回 "Snake"
 */
std::string Snake::type() { return "Snake"; }

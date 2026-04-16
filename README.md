# Game

## 一个新手也可以自定义自己的游戏的项目，内置了一些基础小游戏（基础小游戏还在开发中）

本项目采用 Header-only 设计模式，所有头文件都包含了完整的实现代码，无需单独的.cpp文件（除了main.cpp）。这种设计简化了编译流程，提高了代码的可维护性。

项目遵循 Doxygen 文档标准，所有类和函数都有完整的注释，并优化使用了 `inline`、`const`、引用和 `static` 关键字。

## 系统要求与依赖

- **编译器**: 支持 C++23 标准的编译器（如 GCC 13+、Clang 16+）
- **构建工具**: CMake 3.10 或更高版本
- **依赖库**: ncurses（终端图形界面库）

### 安装依赖

在 Ubuntu/Debian 系统上：

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

在 Fedora/RHEL 系统上：

```bash
sudo dnf install ncurses-devel
```

在 Arch Linux 系统上：

```bash
sudo pacman -S ncurses
```

---

## 如何自定义

Tips: 所有路径相对于项目根目录

### 显示模块

路径：`include/common/display`

> Tips: 彩色字符无法在vscode终端显示

#### 1. pixel.h

提供了 [Pixel](include/common/display/pixel.h#L36-L70) 类，用于表示像素

- [Color](include/common/display/pixel.h#L11-L29) 枚举类：定义了可用的颜色（前景色与背景色通用），颜色为其英文单词的全大写
  - `Color::DEFAULT` 为控制台默认颜色
- [Pixel](include/common/display/pixel.h#L36-L70) 类：
  - 背景色默认为 `Color::DEFAULT`
  - 字符固定为空格 `' '`

#### 2. color_char.h

提供了 [ColorChar](include/common/display/color_char.h#L12-L60) 类，用于表示带颜色的字符

- 继承自 [Pixel](include/common/display/pixel.h#L36-L70) 类
- 添加了前景色和可设置字符的功能
- 可以设置前景色、背景色和字符

#### 3. screen.h

提供了 [Screen](include/common/display/screen.h#L11-L160) 类，用于表示屏幕

- 内部使用 `vector<vector<ColorChar>>` 存储屏幕内容
- 支持初始化指定大小的屏幕网格
- 提供访问、修改、打印屏幕等功能

---

### 输入控制模块

路径：`include/common/io`

#### 1. control.hpp

> Tips: 该文件为 `.hpp` 头文件，声明与定义合并

提供了 ncurses 终端控制的辅助函数：

- `initNcurses()`: 初始化 ncurses 环境，配置终端为非规范模式
- `cleanupNcurses()`: 清理 ncurses 环境并恢复终端设置
- `waitKey()`: 等待用户按键输入

---

### 数学模块

路径：`include/common/maths`

#### 1. random.hpp

> Tips: 该文件为 `.hpp` 文件，声明与定义合并，不会出现在 `src/common/maths/random.cpp` 中

提供了 [random](include/common/maths/random.hpp#L50-L59) 函数模板，支持多种类型：

- 浮点数版本：`float`, `double`, `long double`
- 整数版本：任意整型（通过模板）

> 警告：请不要擅自使用不支持随机数的类型（例如 `std::string`，会引发未定义行为）

---

### 游戏基础模块

路径：`include/common/game/basic`

#### 1. role.h

提供了 [Role](include/common/game/basic/role.h#L6-L19) 基类

- 所有游戏角色的基类
- 绑定了 [RoleScreen](include/common/game/basic/role_screen.h#L14-L155) 网格
- 用于派生具体的游戏角色

#### 2. role_screen.h

提供了 [RoleScreen](include/common/game/basic/role_screen.h#L14-L155) 类

- 角色屏幕管理类
- 每个格子包含 [ColorChar](include/common/display/color_char.h#L12-L60) 和 `Role*`
- 支持访问、修改、打印屏幕等功能

---

### 贪吃蛇游戏模块

路径：`include/snake`

#### 1. command.hpp - 命令管理系统

提供了基于 Trie 树的智能命令查找系统：

- **Command类**: 存储命令描述和参数列表
- **CommandTree类**:
  - 使用 Trie 树结构高效存储命令
  - 支持模糊匹配和自动补全
  - 使用 LCS（最长公共子序列）算法进行智能排序
  - 大小写不敏感匹配
- **全局实例 `ctr`**: 预定义的命令树实例
- **辅助函数**:
  - `init()`: 初始化预定义命令
  - `hanCom()`: 解析命令字符串为命令名和参数列表

#### 2. debug.hpp - 调试控制台

提供交互式调试界面（仅在 DEBUG 模式下启用）：

- 实时命令输入和退格删除
- Tab 键自动补全命令
- 显示匹配命令的描述和参数提示
- 支持的调试命令：
  - `help`: 显示帮助信息
  - `info <编号>`: 查询指定蛇的信息
  - `kill <编号>`: 强制杀死指定蛇
  - `continue`: 退出调试控制台
  - `exit`: 结束程序

#### 3. role/apple.hpp - 苹果角色类

提供了 [Apple](include/snake/role/apple.hpp) 类：

- 继承自 [Role](include/common/game/basic/role.hpp) 类
- 表示贪吃蛇游戏中的食物元素
- 被蛇吃到后会自动移动到新的随机位置
- 显示为红色背景

#### 4. role/snake.hpp - 蛇角色类

提供了 [Snake](include/snake/role/snake.hpp) 类：

- 继承自 [Role](include/common/game/basic/role.hpp) 类
- 支持玩家控制和 AI 控制两种模式
- **AI 算法特点**:
  - 使用 BFS（广度优先搜索）寻找最短路径到苹果
  - 使用 Flood Fill 算法检查逃生空间，避免进入死路
  - 优先级策略：生存 > 吃苹果
- **主要成员函数**:
  - `kill()`: 蛇死亡时调用，将蛇身变为灰色
  - `update()`: 更新蛇的状态和屏幕显示
  - `forward()`: 向前移动一步
  - `findPath()`: AI 寻路逻辑
  - `bfsPath()`: BFS 路径搜索
  - `floodFillCount()`: 计算可达空间大小
- 内部使用 `std::deque` 存储蛇身位置
- 每条蛇有独立的颜色，自动分配不重复

---

## 项目结构

```txt

Game
├── include/
│   ├── common/
│   │   ├── display/              # 显示模块
│   │   │   ├── pixel.hpp         # 像素基类和颜色枚举
│   │   │   ├── color_char.hpp    # 彩色字符类
│   │   │   └── screen.hpp        # 屏幕管理类
│   │   ├── io/                   # 输入控制模块
│   │   │   └── control.hpp       # ncurses 控制接口和 RAII 管理
│   │   ├── maths/                # 数学模块
│   │   │   └── random.hpp        # 随机数生成器
│   │   └── game/                 # 游戏基础模块
│   │       └── basic/
│   │           ├── role.hpp      # 角色基类
│   │           └── role_screen.hpp # 角色屏幕类
│   └── snake/                    # 贪吃蛇游戏模块
│       ├── command.hpp           # 命令管理系统（Trie树）
│       ├── debug.hpp             # 调试控制台
│       └── role/
│           ├── apple.hpp         # 苹果类
│           └── snake.hpp         # 蛇类（含AI算法）
├── src/
│   └── main.cpp                  # 主程序入口
├── CMakeLists.txt                # CMake 构建配置
└── README.md                     # 项目文档
```

## 项目架构

本项目采用模块化设计，主要分为以下几个模块：

### 显示模块 (Display Module)

负责终端界面的渲染和显示功能：

- **Pixel类**: 表示基本像素单元，包含背景色信息
- **ColorChar类**: 继承自Pixel，添加了前景色和字符显示功能
- **Screen类**: 管理二维网格屏幕，提供访问、修改和打印功能
- **RoleScreen类**: 扩展Screen类，支持角色绑定和管理

### 输入控制模块 (Input Control Module)

处理用户输入和终端控制：

- **NcuInit类**: RAII模式的ncurses初始化和清理
- **辅助函数**: 提供等待按键、清行等常用操作

### 数学模块 (Mathematics Module)

提供随机数生成等数学工具：

- **random函数**: 支持多种数据类型的随机数生成

### 游戏基础模块 (Game Foundation Module)

提供游戏开发的基础框架：

- **Role基类**: 所有游戏角色的抽象基类
- **RoleScreen类**: 支持角色绑定的屏幕管理类

### 贪吃蛇游戏模块 (Snake Game Module)

具体的游戏实现：

- **Apple类**: 游戏中的苹果角色
- **Snake类**: 游戏中的蛇角色，支持AI和玩家控制

## 代码文档与优化标准

本项目遵循严格的 Doxygen 文档标准和 C++ 最佳实践：

### 文档注释规范

- **文件级注释**: 每个头文件都有 `@file`、`@brief`、`@author`、`@date` 等完整信息
- **类文档**: 所有类都有 `@brief` 和详细描述，说明设计目的和使用场景
- **函数文档**:
  - 公共方法包含 `@brief`、详细描述、`@param`、`@return`、`@throws`（如适用）
  - 私有/保护方法包含必要的实现细节说明
  - 简单的 getter/setter 保持简洁但信息丰富
- **成员变量**: 使用行内注释 (`///<`) 说明用途
- **语言**: 统一使用中文注释

### 关键字优化策略

#### inline 关键字

**应用于：**

- 简单的 getter/setter 方法（1-3行）
- 频繁使用的简单工具函数
- 在头文件中定义的模板函数
- 紧密循环中调用的函数

**不应用于：**

- 虚函数（inline无效）
- 大型函数（>10-15行）
- 具有复杂逻辑的函数

#### const 关键字

**应用于：**

- 不修改成员变量的成员函数
- 不修改参数的引用参数
- 返回值不需要修改的情况

**示例：**

```cpp
inline int getValue() const { return value; }  // ✓ 正确
void setValue(int val) const { value = val; }  // ✗ 错误
```

#### 引用和常量引用

**const & (常量引用):**

- 大型对象作为只读参数（避免拷贝开销）
- STL容器（vector, string, map等）
- 自定义类或结构体

**& (非常量引用):**

- 需要修改的原地参数（输出参数）

**值传递:**

- 基本类型（int, double, bool, char等）
- 小型POD类型

**示例：**

```cpp
void processString(const std::string& text);  // ✓ 大对象用const引用
void setValue(int value);                      // ✓ 基本类型用值传递
void setFlag(const bool& flag);                // ✗ 小类型不应使用const引用
```

#### static 关键字

**静态成员变量：**

- 所有实例共享的数据（计数器、配置等）

**静态成员函数：**

- 不访问任何非静态成员的函数
- 工具函数或工厂方法

**示例：**

```cpp
class Counter {
    static inline int getCount() { return count; }  // 静态函数
    static int count;                                // 静态成员
};
```

## 编译与运行

使用 CMake 构建系统：

```bash
mkdir build
cd build
cmake ..
make
./Game
```

或者使用Ninja构建系统：

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
./Game
```

## 注意事项

1. **终端兼容性**
   - 项目使用 ncurses 库进行终端图形界面渲染
   - 建议使用支持 ncurses 的原生 Linux 终端（如 GNOME Terminal、Konsole、xterm 等）
   - VSCode 集成终端可能无法正确显示 ncurses 界面

2. **ncurses 依赖**
   - 项目依赖 ncurses 库，请确保已安装 `libncurses5-dev` 和 `libncursesw5-dev`
   - 编译时 CMake 会自动查找并链接 ncurses 库

3. **random.hpp 特殊说明**
   - 该文件为 `.hpp` 头文件，声明与定义合并
   - 不需要对应的 `.cpp` 实现文件

4. **数组下标**
   - 所有数组和网格的下标均从 `0` 开始

5. **内存管理**
   - 使用 [Role](include/common/game/basic/role.h#L6-L19)、[RoleScreen](include/common/game/basic/role_screen.h#L14-L155) 相关类时需要注意内存管理
   - 及时释放不再使用的角色对象

6. **随机数使用警告**
   - 请勿对不支持的类型使用 [random](include/common/maths/random.hpp#L50-L59) 函数（例如 `std::string`）
   - 仅支持整型和浮点型，否则会引发未定义行为

7. **代码文档与内联优化**
   - 所有类和公共函数都有完整的 Doxygen 风格文档注释
   - 简单的 getter/setter 函数已标记为 `inline` 以提高性能
   - 虚函数和复杂逻辑保持在 `.cpp` 文件中实现

# Game

## 一个新手也可以自定义自己的游戏的项目（Linux），内置了一些基础小游戏（基础小游戏还在开发中）

所有 include 目录下的.h 文件的实现都在 src 目录下，路径完全相同（random.hpp 除外）

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

#### 2. role_list.h

提供了 [RoleList](include/common/game/basic/role_list.h#L11-L157) 类

- 角色列表管理类
- 管理二维角色指针数组
- 支持添加、删除、遍历角色

#### 3. role_screen.h

提供了 [RoleScreen](include/common/game/basic/role_screen.h#L14-L155) 类

- 角色屏幕管理类
- 合并了 [Screen](include/common/display/screen.h#L11-L160) 和 [RoleList](include/common/game/basic/role_list.h#L11-L157) 的功能
- 每个格子包含 [ColorChar](include/common/display/color_char.h#L12-L60) 和 `Role*`

---

### 贪吃蛇游戏模块

路径：`include/snake/role`

#### 1. apple.h

提供了 [Apple](include/snake/role/apple.h#L6-L32) 类

- 继承自 [Role](include/common/game/basic/role.h#L6-L19) 类
- 表示贪吃蛇游戏中的苹果
- 具有随机移动功能

---

## 项目结构

```txt
Game
├── include/
│ ├── common/
│ │ ├── display/ # 显示模块
│ │ │ ├── pixel.h # 像素基类
│ │ │ ├── color_char.h # 彩色字符类
│ │ │ └── screen.h # 屏幕类
│ │ ├── maths/ # 数学模块
│ │ │ └── random.hpp # 随机数生成器
│ │ └── game/ # 游戏基础模块
│ │ └── basic/
│ │ ├── role.h # 角色基类
│ │ ├── role_list.h # 角色列表类
│ │ └── role_screen.h # 角色屏幕类
│ └── snake/ # 贪吃蛇游戏模块
│ └── role/
│ └── apple.h # 苹果类
├── src/
│ ├── common/
│ │ ├── display/ # 显示模块实现
│ │ │ ├── pixel.cpp
│ │ │ ├── color_char.cpp
│ │ │ └── screen.cpp
│ │ └── game/
│ │ └── basic/ # 游戏基础模块实现
│ │ ├── role.cpp
│ │ ├── role_list.cpp
│ │ └── role_screen.cpp
│ └── snake/
│ └── role/ # 贪吃蛇游戏模块实现
│ └── apple.cpp
└── main.cpp # 主程序入口
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

## 注意事项

1. **终端兼容性**
   - 彩色字符在某些终端（如 VSCode 集成终端）可能无法正常显示
   - 建议使用支持 ANSI 转义码的原生 Linux 终端

2. **random.hpp 特殊说明**
   - 该文件为 `.hpp` 头文件，声明与定义合并
   - 不需要对应的 `.cpp` 实现文件

3. **数组下标**
   - 所有数组和网格的下标均从 `0` 开始

4. **内存管理**
   - 使用 [Role](include/common/game/basic/role.h#L6-L19)、[RoleList](include/common/game/basic/role_list.h#L11-L157)、[RoleScreen](include/common/game/basic/role_screen.h#L14-L155) 相关类时需要注意内存管理
   - 及时释放不再使用的角色对象

5. **随机数使用警告**
   - 请勿对不支持的类型使用 [random](include/common/maths/random.hpp#L50-L59) 函数（例如 `std::string`）
   - 仅支持整型和浮点型，否则会引发未定义行为

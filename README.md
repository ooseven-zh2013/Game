# Game

## 一个新手也可以自定义自己的游戏的项目（Linux），内置了一些基础小游戏（基础小游戏还在开发中）

所有include目录下的.h文件的实现都在src目录下，路径完全相同

## 如何自定义

Tips: 所有路径相当于项目根目录

### 显示

路径: include/common/display
Tips: 彩色字符无法在vscode终端显示

#### 1. pixel.h

##### 1. 提供了`Color`枚举类，用于枚举颜色（前景色与背景色通用），颜色为其英文单词的全大写

Tips: `Color::DEFAULT`为控制台默认颜色

##### 2. 提供了`Pixel`类，用于表示像素

Info: `Pixel`类只有背景色，字符为`L' '`。

#### 2.color_char.h

##### 1.提供了`ColorChar`类，用于表示颜色字符

Info: `ColorChar`继承了`Pixel`类，并添加了前景色与字符的设置。

#### 3.screen.h

#### 1.提供了`Screen`类，用于表示屏幕

Info: `Screen`类是一个`vector<vector<ColorChar>>`，用于表示屏幕。

### 数学

路径: include/common/maths

#### 1.random.hpp

Tips: 该文件为`.hpp`文件，声明与定义合并，不会出现在`src/common/maths/random.cpp`中

Info: 提供了`random`函数，重载了任意整型版本与浮点数版本（请不要擅自使用不支持随机数的类型（例如`std::string`，会引发未定义行为）

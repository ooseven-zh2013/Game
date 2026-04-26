---
name: Bug 报告
about: 创建报告以帮助我们改进
title: '[BUG] '
labels: bug
assignees: ''
---

**问题描述**
清晰简洁地描述这个 bug 是什么。

**Bug 分类**
请选择此 Bug 的类型（可多选）：

- [ ] **编译问题**：代码无法编译通过，存在编译错误
- [ ] **运行问题**：程序可以运行但存在逻辑或显示问题（如蛇类不动、调试控制台无法运行、显示异常等）
- [ ] **异常崩溃**：程序运行时发生异常（如数组越界、段错误、throw 异常信息等）

**复现步骤**
重现该行为的步骤：

1. 进入 '...'
2. 点击 '....'
3. 看到错误

**预期行为**
清晰简洁地描述你期望发生的行为。

**截图**
如果适用，添加截图以帮助解释你的问题。

**环境信息（请填写以下信息）：**

- 操作系统: [必须为 Linux，例如 Kali, Ubuntu, Debian, Arch 等]
- ncurses 库: [请确认已安装，例如 libncurses5-dev 或 libncursesw5-dev]
- 编译器: [例如 GCC, Clang]
- 构建系统: [例如 CMake with Ninja/Make]
- 版本: [例如 v1.0.0]

> **注意**: 本项目使用 ncurses 库，仅支持 Linux 系统。在提交 Bug 前，请确保：
>
> - 你正在使用 Linux 系统
> - 已安装 ncurses 开发库（Ubuntu/Debian: `sudo apt install libncurses5-dev libncursesw5-dev`）

**其他信息**
在此添加有关此问题的任何其他信息。

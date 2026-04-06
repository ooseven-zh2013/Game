#ifndef CONTROL_HPP
#define CONTROL_HPP
#pragma once

#include <cstdio>
#include <cstdlib>
#include <system_error>
#include <termios.h>
#include <unistd.h>

/**
 * @brief 从标准输入读取单个字符（非规范模式）
 *
 * 该函数临时将终端设置为非规范模式，禁用行缓冲和回显功能，
 * 从而可以立即读取用户输入的单个字符而无需等待回车键。
 * 读取完成后会自动恢复终端的原始设置。
 *
 * @return int 读取到的字符的 ASCII 码值
 * @throws std::system_error 当获取或设置终端属性失败时抛出，错误信息包含失败的系统调用名称
 */
int getch() {
  struct termios oldTermios, newt;
  int ch;

  if (tcgetattr(STDIN_FILENO, &oldTermios) != 0) {
    throw std::system_error(errno, std::system_category(), "tcgetattr");
  }

  /**
   * 配置新的终端属性：
   * 禁用 ICANON（规范模式）和 ECHO（字符回显）
   */
  newt = oldTermios;
  newt.c_lflag &= ~(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
    throw std::system_error(errno, std::system_category(), "tcsetattr");
  }

  ch = getchar();

  if (tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios) != 0) {
    throw std::system_error(errno, std::system_category(), "tcsetattr (restore)");
  }

  if (ch == EOF) {
    throw std::system_error(EIO, std::system_category(), "getchar");
  }

  return ch;
}

/**
 * @brief 从标准输入读取一个字符并回显到终端
 *
 * 该函数读取单个字符输入，并自动将字符输出到屏幕。
 * 如果输出失败，将抛出系统错误异常。
 *
 * @return int 读取的字符的 ASCII 码值
 * @throws std::system_error 当 putchar 输出失败时抛出
 */
int getche() {
  int ch = getch();
  if (putchar(ch) == EOF) {
    throw std::system_error(errno, std::system_category(), "putchar");
  }
  return ch;
}

/**
 * @brief 清空终端屏幕
 *
 * 该函数通过执行系统命令 "clear" 来清除终端显示内容，
 * 将光标移动到屏幕左上角。
 */
void clear() { system("clear"); }

/**
 * 等待用户按键输入
 *
 * 该函数会显示提示信息并阻塞等待用户按下任意键，
 * 然后返回按下的键值。
 *
 * @return int 用户按下的键的ASCII码值
 */
int waitKey() {
  puts("请按任意键继续...");
  return getch();
}

#endif // CONTROL_HPP
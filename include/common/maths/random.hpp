/**
 * @file random.hpp
 * @brief 随机数生成工具
 *
 * 提供统一的随机数生成接口，支持整数和浮点数类型。
 * 使用std::mt19937作为随机数引擎，确保高质量的随机性。
 *
 * @author Ooseven
 * @date 2026
 */

#ifndef RANDOM_H
#define RANDOM_H
#pragma once

#include <random>
#include <stdexcept>

/**
 * @brief 在 [l, r) 范围内生成 float 类型的随机数
 *
 * 使用 Mersenne Twister 引擎生成均匀分布的随机浮点数。
 * 随机数引擎在首次调用时初始化，并在程序生命周期内复用。
 *
 * @param l 下限（包含）
 * @param r 上限（不包含）
 * @return float 生成的随机数
 * @throws std::invalid_argument 如果 l >= r
 */
inline float random(float l, float r) {
  if (l >= r) {
    throw std::invalid_argument("下限l必须小于上限r");
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(l, r);
  return dis(gen);
}

/**
 * @brief 在 [l, r) 范围内生成 double 类型的随机数
 *
 * 使用 Mersenne Twister 引擎生成均匀分布的随机浮点数。
 * 随机数引擎在首次调用时初始化，并在程序生命周期内复用。
 *
 * @param l 下限（包含）
 * @param r 上限（不包含）
 * @return double 生成的随机数
 * @throws std::invalid_argument 如果 l >= r
 */
inline double random(double l, double r) {
  if (l >= r) {
    throw std::invalid_argument("下限l必须小于上限r");
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(l, r);
  return dis(gen);
}

/**
 * @brief 在 [l, r) 范围内生成 long double 类型的随机数
 *
 * 使用 Mersenne Twister 引擎生成均匀分布的随机浮点数。
 * 随机数引擎在首次调用时初始化，并在程序生命周期内复用。
 *
 * @param l 下限（包含）
 * @param r 上限（不包含）
 * @return long double 生成的随机数
 * @throws std::invalid_argument 如果 l >= r
 */
inline long double random(long double l, long double r) {
  if (l >= r) {
    throw std::invalid_argument("下限l必须小于上限r");
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<long double> dis(l, r);
  return dis(gen);
}

/**
 * @brief 在 [l, r] 范围内生成整数类型的随机数
 *
 * 使用 Mersenne Twister 引擎生成均匀分布的随机整数。
 * 这是一个模板函数，支持任何整数类型（int, size_t, char等）。
 * 随机数引擎在首次调用时初始化，并在程序生命周期内复用。
 *
 * @tparam T 整数类型
 * @param l 下限（包含）
 * @param r 上限（包含）
 * @return T 生成的随机整数
 * @throws std::invalid_argument 如果 l > r
 */
template <typename T> inline T random(const T &l, const T &r) {
  if (l > r) {
    throw std::invalid_argument("下限l必须小于等于上限r");
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(l, r);
  return dis(gen);
}

#endif // RANDOM_H
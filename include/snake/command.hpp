/**
 * @file command.hpp
 * @brief 命令管理系统
 *
 * 实现了基于Trie树的命令存储和智能查找系统。
 * 支持模糊匹配、自动补全和优先级排序。
 */

#ifndef COMMAND_H
#define COMMAND_H
#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief 命令类，存储命令的描述和参数列表
 *
 * Command对象包含命令的文本描述和预期的参数列表，
 * 用于在调试控制台中提供命令提示和帮助信息。
 */
class Command {
public:
  using csr = const std::string &;
  std::string description;             ///< 命令描述
  std::vector<std::string> parameters; ///< 参数列表

  /**
   * @brief 构造函数
   * @param desc 命令描述
   * @param params 参数列表
   */
  Command(csr desc = "", const std::vector<std::string> &params = {}) : description(desc), parameters(params) {}
};

/**
 * @brief 命令树类，用于存储和查找命令
 *
 * 使用Trie（前缀树）结构存储命令，每个节点代表一个字符。
 * 支持插入、查找、删除命令操作，并提供智能模糊匹配功能。
 * 查找时使用最长公共子序列(LCS)和位置权重进行优先级排序。
 */
class CommandTree {
private:
  using csr = const std::string &;

  /**
   * @brief 树节点结构
   */
  struct TreeNode {
    std::unique_ptr<Command> command;                ///< 如果此节点是命令的结尾，则存储命令信息
    std::vector<std::unique_ptr<TreeNode>> children; ///< 子节点（最多26个，对应a-z）

    TreeNode() : command(nullptr) {}

    /**
     * @brief 获取或创建指定字符的子节点
     * @param ch 字符（'a'-'z'）
     * @return TreeNode* 子节点指针
     */
    TreeNode *getOrCreateChild(char ch) {
      int index = ch - 'a';
      if (index < 0 || index >= 26) {
        return nullptr;
      }

      // 扩展children数组到足够大小
      while (static_cast<int>(children.size()) <= index) {
        children.push_back(nullptr);
      }

      if (!children[index]) {
        children[index] = std::make_unique<TreeNode>();
      }

      return children[index].get();
    }

    /**
     * @brief 获取指定字符的子节点
     * @param ch 字符（'a'-'z'）
     * @return TreeNode* 子节点指针，不存在则返回nullptr
     */
    TreeNode *getChild(char ch) const {
      int index = ch - 'a';
      if (index < 0 || index >= 26 || index >= static_cast<int>(children.size())) {
        return nullptr;
      }
      return children[index].get();
    }
  };

  std::unique_ptr<TreeNode> root; ///< 树的根节点

  /**
   * @brief 递归收集所有匹配的命令
   * @param node 当前节点
   * @param prefix 当前前缀
   * @param results 结果列表
   */
  void collectCommands(TreeNode *node, std::string &prefix,
                       std::vector<std::pair<std::string, Command>> &results) const {
    if (!node)
      return;

    // 如果当前节点有命令，添加到结果中
    if (node->command) {
      results.emplace_back(prefix, *node->command);
    }

    // 递归遍历所有子节点
    for (short i = 0; i < static_cast<short>(node->children.size()); ++i) {
      if (node->children[i]) {
        prefix.push_back(static_cast<char>('a' + i));
        collectCommands(node->children[i].get(), prefix, results);
        prefix.pop_back();
      }
    }
  }

public:
  /**
   * @brief 构造函数，初始化空树
   */
  CommandTree() : root(std::make_unique<TreeNode>()) {}

  /**
   * @brief 插入命令
   * @param commandStr 命令字符串（只包含小写字母）
   * @param description 命令描述
   * @param params 参数列表
   */
  void insert(csr commandStr, csr description = "", const std::vector<std::string> &params = {}) {
    if (commandStr.empty())
      return;

    TreeNode *current = root.get();

    // 遍历命令字符串的每个字符，构建路径
    for (char ch : commandStr) {
      if (ch < 'a' || ch > 'z') {
        return; // 无效字符，直接返回
      }
      current = current->getOrCreateChild(ch);
    }

    // 在最后一个节点存储命令信息
    current->command = std::make_unique<Command>(description, params);
  }

  /**
   * @brief 计算两个字符串的最长公共子序列(LCS)长度
   *
   * 使用动态规划算法计算LCS长度，用于评估搜索字符串与命令的匹配程度。
   * LCS越长，表示两个字符串的相似度越高。
   *
   * @param str1 第一个字符串
   * @param str2 第二个字符串
   * @return int 最长公共子序列的长度
   */
  static int longestCommonSubsequence(csr str1, csr str2) {
    int m = static_cast<int>(str1.size());
    int n = static_cast<int>(str2.size());

    // 创建DP表
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // 填充DP表
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (str1[i - 1] == str2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
        } else {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
      }
    }

    return dp[m][n];
  }

  /**
   * @brief 计算匹配字符在命令中的最早出现位置之和
   *
   * 用于评估匹配的“质量”：匹配字符越靠前，位置总和越小，优先级越高。
   * 如果某个搜索字符未找到，则返回一个极大的值表示不匹配。
   *
   * @param searchStr 搜索字符串
   * @param commandStr 命令字符串
   * @return int 匹配字符在命令中的位置之和（越小越好）
   */
  static int calculateMatchPositionSum(csr searchStr, csr commandStr) {
    int sum = 0;
    int cmdIndex = 0;

    for (char searchChar : searchStr) {
      bool found = false;
      while (cmdIndex < static_cast<int>(commandStr.size())) {
        if (commandStr[cmdIndex] == searchChar) {
          sum += cmdIndex;
          cmdIndex++;
          found = true;
          break;
        }
        cmdIndex++;
      }
      if (!found) {
        // 如果没有找到，加上一个很大的值表示不匹配
        sum += 1e9;
      }
    }

    return sum;
  }

  /**
   * @brief 查找最可能的命令（智能模糊匹配）
   *
   * 根据输入的前缀或模糊字符串，使用多级优先级策略查找最佳匹配：
   *   1. 最长公共子序列(LCS)长度 - 越大越好
   *   2. 匹配字符位置总和 - 越小越好
   *   3. 命令长度 - 越短越好
   *   4. 字典序 - 越小越好
   *
   * 搜索时会自动将输入转换为小写，实现大小写不敏感匹配。
   *
   * @param com 搜索字符串（可以是前缀或模糊匹配）
   * @return std::pair<std::string, std::unique_ptr<Command>>
   *         最可能的命令字符串和命令对象，如果没有匹配则返回空字符串和nullptr
   */
  std::pair<std::string, std::unique_ptr<Command>> find(csr com) const {
    std::vector<std::pair<std::string, Command>> allResults;

    // 将命令转换为小写后查询
    std::string s = com;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s.empty()) {
      // 如果搜索字符串为空，返回所有命令中最短且字典序最小的
      std::string prefix;
      collectCommands(root.get(), prefix, allResults);

      if (allResults.empty()) {
        return {"", nullptr};
      }

      // 按照规则排序：先按长度，再按字典序
      std::sort(allResults.begin(), allResults.end(),
                [](const std::pair<std::string, Command> &a, const std::pair<std::string, Command> &b) {
                  if (a.first.size() != b.first.size()) {
                    return a.first.size() < b.first.size();
                  }
                  return a.first < b.first;
                });

      return {allResults[0].first, std::make_unique<Command>(allResults[0].second)};
    }

    // 导航到前缀对应的节点
    TreeNode *current = root.get();
    for (char ch : s) {
      if (ch < 'a' || ch > 'z') {
        return {"", nullptr}; // 无效字符，返回空结果
      }
      current = current->getChild(ch);
      if (!current) {
        return {"", nullptr}; // 路径不存在，返回空结果
      }
    }

    // 从该节点开始收集所有命令
    std::string prefix(s);
    collectCommands(current, prefix, allResults);

    if (allResults.empty()) {
      return {"", nullptr};
    }

    // 根据优先级规则选择最可能的命令
    // 1. 匹配字符数和字符顺序最多的优先（使用LCS）
    // 2. 如果相同，则命令里匹配字符靠前的优先
    // 3. 如果还是相同，则命令短的优先
    // 4. 如果还是相同，则命令字典序小的优先

    std::string bestCommand = allResults[0].first;
    Command bestCmd = allResults[0].second;
    int bestLCS = longestCommonSubsequence(s, bestCommand);
    int bestPosSum = calculateMatchPositionSum(s, bestCommand);

    for (size_t i = 1; i < allResults.size(); ++i) {
      const std::string &currentCommand = allResults[i].first;
      const Command &currentCmd = allResults[i].second;
      int currentLCS = longestCommonSubsequence(s, currentCommand);
      int currentPosSum = calculateMatchPositionSum(s, currentCommand);

      bool isBetter = false;

      // 比较LCS长度（越大越好）
      if (currentLCS > bestLCS) {
        isBetter = true;
      } else if (currentLCS == bestLCS) {
        // LCS相同，比较匹配位置总和（越小越好）
        if (currentPosSum < bestPosSum) {
          isBetter = true;
        } else if (currentPosSum == bestPosSum) {
          // 位置总和相同，比较命令长度（越短越好）
          if (currentCommand.size() < bestCommand.size()) {
            isBetter = true;
          } else if (currentCommand.size() == bestCommand.size()) {
            // 长度相同，比较字典序（越小越好）
            if (currentCommand < bestCommand) {
              isBetter = true;
            }
          }
        }
      }

      if (isBetter) {
        bestCommand = currentCommand;
        bestCmd = currentCmd;
        bestLCS = currentLCS;
        bestPosSum = currentPosSum;
      }
    }

    return {bestCommand, std::make_unique<Command>(bestCmd)};
  }

  /**
   * @brief 删除命令
   * @param s 要删除的命令字符串
   */
  void erase(csr s) {
    if (s.empty())
      return;

    TreeNode *current = root.get();

    // 导航到命令对应的节点
    for (char ch : s) {
      if (ch < 'a' || ch > 'z') {
        return; // 无效字符，直接返回
      }
      current = current->getChild(ch);
      if (!current) {
        return; // 命令不存在，直接返回
      }
    }

    // 清除该节点的命令信息
    current->command = nullptr;
  }
} ctr;

void init() {
  static bool isInit = false;
  if (isInit)
    return;
  isInit = true;
  ctr.insert("help", "显示帮助信息");
  ctr.insert("continue", "退出控制台");
  ctr.insert("exit", "结束程序");
  ctr.insert("info", "查询蛇类信息", {"编号"});
  ctr.insert("kill", "强制杀死蛇类", {"编号"});
  ctr.insert("edit", "修改蛇类属性", {"编号", "属性", "修改值"});
}

/**
 * @brief 命令处理函数
 * @param command 命令字符串
 * @return std::pair<std::string, std::vector<std::string>>
 * 命令处理结果，包含命令描述和参数列表
 */
std::pair<std::string, std::vector<std::string>> hanCom(const std::string &command) {
  // 通过空格分割命令，格式为：命令 参数1 参数2 ...
  // 分割为命令和{参数1, 参数2, ...}
  size_t pos = command.find(' ');
  std::string cmd = (pos == std::string::npos) ? command : command.substr(0, pos);
  std::string args = (pos == std::string::npos) ? "" : command.substr(pos + 1);

  std::vector<std::string> cmdArgs;
  if (!args.empty()) {
    size_t start = 0;
    size_t end = args.find(' ');
    while (end != std::string::npos) {
      cmdArgs.push_back(args.substr(start, end - start));
      start = end + 1;
      end = args.find(' ', start);
    }
    // 添加最后一个参数
    cmdArgs.push_back(args.substr(start));
  }

  return {cmd, cmdArgs};
}

#endif // COMMAND_H
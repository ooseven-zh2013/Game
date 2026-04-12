#include "common/game/basic/role.h"

/**
 * @brief Role构造函数实现
 *
 * 初始化角色并绑定到指定的角色屏幕。
 *
 * @param scr_ 绑定的角色屏幕引用
 */
Role::Role(RoleScreen &scr_) : scr(scr_) {}
#include "common/game/basic/role_list.h"

RoleList::RoleList(size_t n, size_t m, Role *init) : roles(n, vr(m, init)) {}

size_t RoleList::size() const { return roles.empty() ? 0 : roles.size() * roles[0].size(); }

RoleList::vvr &RoleList::data() { return roles; }

const RoleList::vvr &RoleList::data() const { return roles; }

size_t RoleList::x_size() const { return roles.size(); }

size_t RoleList::y_size() const { return roles.empty() ? 0 : roles[0].size(); }

RoleList::vr &RoleList::operator[](int idx) { return roles[idx]; }

const RoleList::vr &RoleList::operator[](int idx) const { return roles[idx]; }

RoleList::vr &RoleList::at(int idx) { return roles.at(idx); }

const RoleList::vr &RoleList::at(int idx) const { return roles.at(idx); }

Role *&RoleList::at(int x, int y) { return roles.at(x).at(y); }

Role *const &RoleList::at(int x, int y) const { return roles.at(x).at(y); }

Role *&RoleList::at(const std::pair<int, int> &xy) { return roles.at(xy.first).at(xy.second); }

Role *const &RoleList::at(const std::pair<int, int> &xy) const { return roles.at(xy.first).at(xy.second); }

void RoleList::set(Role *value) {
  for (auto &row : roles) {
    for (auto &role : row) {
      role = value;
    }
  }
}

void RoleList::clear() { set(nullptr); }

void RoleList::destroy_all() {
  for (auto &row : roles) {
    for (auto *role : row) {
      delete role;
      role = nullptr;
    }
  }
}
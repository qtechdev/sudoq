#ifndef __GRID_HPP__
#define __GRID_HPP__
#include <array>

namespace sudoq {
  struct cell {
    // std::array<char, 9> notes;
    char value = ' ';
    bool is_given = false;
  };

  struct box {
    box &operator=(const std::array<char, 9> &data);
    std::array<cell, 9> cells;
  };

  struct grid {
    std::array<box, 9> boxes;
    grid &operator=(const std::array<std::array<char, 9>, 9> &data);
    void insert(const int y, const int x, const char c);
  };

  void display(const box &b, const int y, const int x);
  void display(const grid &g);
}


#endif // __GRID_HPP__
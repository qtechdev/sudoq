#ifndef __GRID_HPP__
#define __GRID_HPP__
#include <array>

namespace sudoq {
  struct cell {
    // std::array<char, 9> notes;
    char value = ' ';
    bool is_given = false;
  };

  struct group {
    group &operator=(const std::array<char, 9> &data);
    std::array<cell, 9> cells;
  };

  struct grid {
    std::array<group, 9> rows;
    grid &operator=(const std::array<std::array<char, 9>, 9> &data);
    void insert(const int y, const int x, const char c);
  };

  // display functions
  void display(const group &row, const int y);
  void display(const grid &g);

  // checkers
  bool check(const grid &g);
  bool check(const group &g);

  // conversions
  std::array<group, 9> as_cols(const grid &g);
  std::array<group, 9> as_boxes(const grid &g);

  group get_col(const grid &g, const int n);
  group get_box(const grid &g, const int n);
}


#endif // __GRID_HPP__

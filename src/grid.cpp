#include <ncurses.h>

#include "grid.hpp"

sudoq::box &sudoq::box::operator=(const std::array<char, 9> &data) {
  for (int i = 0; i < 9; ++i) {
    cells[i].value = data[i];
    cells[i].is_given = (data[i] != ' ');
  }

  return *this;
}

sudoq::grid &sudoq::grid::operator=(
  const std::array<std::array<char, 9>, 9> &data
) {
  for (int i = 0; i < 9; ++i) {
    boxes[i] = data[i];
  }

  return *this;
}

void sudoq::grid::insert(const int y, const int x, const char c) {
  int box_ypos = y / 3;
  int box_xpos = x / 3;
  int box_index = (box_ypos * 3) + box_xpos;

  int cell_ypos = y % 3;
  int cell_xpos = x % 3;
  int cell_index = (cell_ypos * 3) + cell_xpos;

  cell &cref = boxes[box_index].cells[cell_index];
  if (!cref.is_given) {
    cref.value = c;
  }
}

void sudoq::display(const box &b, const int y, const int x) {
  attroff(A_BOLD);
  for (int j = 0; j < 3 ; ++j) {
    move(y + j, x);
    for (int i = 0; i < 3 ; ++i) {
      int index = (j * 3) + i;
      attroff(A_BOLD);
      if (b.cells[index].is_given) {
        attron(A_BOLD);
      }

      addch(b.cells[index].value);
    }
  }

  refresh();
}

void sudoq::display(const grid &g) {
  static constexpr int box_width = 3;
  static constexpr int box_height = 3;
  static constexpr int vr_width = 1;
  static constexpr int hr_height = 1;

  move(0, 0);
  printw("   |   |   \n");
  printw("   |   |   \n");
  printw("   |   |   \n");
  printw("---+---+---\n");
  printw("   |   |   \n");
  printw("   |   |   \n");
  printw("   |   |   \n");
  printw("---+---+---\n");
  printw("   |   |   \n");
  printw("   |   |   \n");
  printw("   |   |   \n");

  for (int j = 0; j < 3 ; ++j) {
    for (int i = 0; i < 3 ; ++i) {
      int index = (j * 3) + i;

      int y_pos = j * (box_height + hr_height);
      int x_pos = i * (box_width + vr_width);

      display(g.boxes[index], y_pos, x_pos);
    }
  }

  refresh();
}

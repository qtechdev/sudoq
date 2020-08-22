#include <algorithm> // std::sort, std::transform
#include <ncurses.h>

#include <sstream>

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

std::array<sudoq::box, 9> sudoq::as_rows(const grid &g) {
  std::array<box, 9> boxes;

  for (int i = 0; i < 9; ++i) {
    boxes[i] = get_row(g, i);
  }

  return boxes;
}

std::array<sudoq::box, 9> sudoq::as_cols(const grid &g) {
  std::array<box, 9> boxes;

  for (int i = 0; i < 9; ++i) {
    boxes[i] = get_col(g, i);
  }

  return boxes;
}

sudoq::box sudoq::get_row(const grid &g, const int r) {
  int a = (r / 3) * 3; // box start index
  int b = (r % 3) * 3; // cell start index

  box bx = {
    g.boxes[a    ].cells[b    ], g.boxes[a    ].cells[b + 1], g.boxes[a    ].cells[b + 2],
    g.boxes[a + 1].cells[b    ], g.boxes[a + 1].cells[b + 1], g.boxes[a + 1].cells[b + 2],
    g.boxes[a + 2].cells[b    ], g.boxes[a + 2].cells[b + 1], g.boxes[a + 2].cells[b + 2]
  };

  return bx;
}

sudoq::box sudoq::get_col(const grid &g, const int c) {
  int a = (c / 3); // box start index
  int b = (c % 3); // cell start index

  box bx = {
    g.boxes[a    ].cells[b    ], g.boxes[a    ].cells[b + 3], g.boxes[a    ].cells[b + 6],
    g.boxes[a + 3].cells[b    ], g.boxes[a + 3].cells[b + 3], g.boxes[a + 3].cells[b + 6],
    g.boxes[a + 6].cells[b    ], g.boxes[a + 6].cells[b + 3], g.boxes[a + 6].cells[b + 6]
  };

  return bx;
}


bool sudoq::check(const grid &g) {
  int i;

  i = 0;
  for (const box &b : g.boxes) {
    if (!check(b)) {
      move(14, 0);
      printw("invalid box -> ");
      addch(i + '0');
      refresh();
      return false;
    }
    ++i;
  }

  i = 0;
  for (const box &b : as_rows(g)) {
    if (!check(b)) {
      move(14, 0);
      printw("invalid row -> ");
      addch(i + '0');
      refresh();
      return false;
    }
    ++i;
  }

  i = 0;
  for (const box &b : as_cols(g)) {
    if (!check(b)) {
      move(14, 0);
      printw("invalid col -> ");
      addch(i + '0');
      refresh();
      return false;
    }
    ++i;
  }

  return true;
}

bool sudoq::check(const box &b) {
  std::array<char, 9> cells;
  std::transform(
    b.cells.begin(), b.cells.end(), cells.begin(),
    [](const cell &c){
      return c.value;
    }
  );

  std::sort(cells.begin(), cells.end());

  std::array<char, 9> target = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};


  std::stringstream ss;
  ss << "{ ";
  for (const int &i : cells) {
    ss << i << " ";
  }
  ss << "} -> { ";
  for (const int &i : target) {
    ss << i << " ";
  }
  ss << "}";

  move(15, 0);
  printw(ss.str().c_str());
  refresh();

  return (cells == target);
}

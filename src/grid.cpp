#include <algorithm> // std::sort, std::transform
#include <ncurses.h>

#include <sstream>

#include "grid.hpp"

// struct group
sudoq::group &sudoq::group::operator=(const std::array<char, 9> &data) {
  for (int i = 0; i < 9; ++i) {
    cells[i].value = data[i];
    cells[i].is_given = (data[i] != ' ');
  }

  return *this;
}

// struct grid
sudoq::grid &sudoq::grid::operator=(
  const std::array<std::array<char, 9>, 9> &data
) {
  for (int i = 0; i < 9; ++i) {
    rows[i] = data[i];
  }

  return *this;
}

void sudoq::grid::insert(const int y, const int x, const char c) {
  cell &cref = rows[y].cells[x];
  if (!cref.is_given) {
    cref.value = c;
  }
}

// display functions
void sudoq::display(const group &row, const int y) {
  const int skip_rows = y / 3;

  for (int i = 0; i < 9 ; ++i) {
    const int skip_cols = i / 3;

    row.cells[i].is_given ? attron(A_BOLD) : attroff(A_BOLD);
    mvaddch(y + skip_rows, i + skip_cols, row.cells[i].value);
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

  for (int i = 0; i < 9 ; ++i) {
    display(g.rows[i], i);
  }

  refresh();
}

// checkers
bool sudoq::check(const grid &g) {
  int i;

  i = 0;
  for (const group &b : g.rows) {
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
  for (const group &b : as_cols(g)) {
    if (!check(b)) {
      move(14, 0);
      printw("invalid col -> ");
      addch(i + '0');
      refresh();
      return false;
    }
    ++i;
  }

  i = 0;
  for (const group &b : as_boxes(g)) {
    if (!check(b)) {
      move(14, 0);
      printw("invalid box -> ");
      addch(i + '0');
      refresh();
      return false;
    }
    ++i;
  }

  return true;
}

bool sudoq::check(const group &g) {
  std::array<char, 9> cells;
  std::transform(
    g.cells.begin(), g.cells.end(), cells.begin(),
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

// conversions
std::array<sudoq::group, 9> sudoq::as_cols(const grid &g) {
  std::array<group, 9> cols;

  for (int i = 0; i < 9; ++i) {
    cols[i] = get_col(g, i);
  }

  return cols;
}

std::array<sudoq::group, 9> sudoq::as_boxes(const grid &g) {
  std::array<group, 9> boxes;

  for (int i = 0; i < 9; ++i) {
    boxes[i] = get_box(g, i);
  }

  return boxes;
}

sudoq::group sudoq::get_col(const grid &g, const int c) {
  group b;

  for (int i = 0; i < 9; ++i) {
    b.cells[i] = g.rows[i].cells[c];
  }

  return b;
}

sudoq::group sudoq::get_box(const grid &g, const int n) {
  int a = (n / 3) * 3; // row start index
  int b = (n % 3) * 3; // col start index

  group box = {
    g.rows[a  ].cells[b  ], g.rows[a  ].cells[b+1], g.rows[a  ].cells[b+2],
    g.rows[a+1].cells[b  ], g.rows[a+1].cells[b+1], g.rows[a+1].cells[b+2],
    g.rows[a+2].cells[b  ], g.rows[a+2].cells[b+1], g.rows[a+2].cells[b+2]
  };

  return box;
}

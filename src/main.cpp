#include <algorithm> // std::clamp

#include <ncurses.h>

#include "grid.hpp"

std::array<std::array<char, 9>, 9> data = {
  ' ', ' ', '9',  ' ', '4', ' ',  ' ', ' ', ' ',
  ' ', '3', '4',  ' ', ' ', '5',  ' ', ' ', ' ',
  ' ', ' ', '7',  ' ', ' ', ' ',  '4', '8', '3',

  ' ', ' ', '8',  '1', '7', ' ',  '6', '5', ' ',
  ' ', ' ', '1',  '6', ' ', '8',  '3', ' ', ' ',
  ' ', '4', '6',  ' ', '5', '9',  '7', ' ', ' ',

  '9', '3', '5',  ' ', ' ', ' ',  '8', ' ', ' ',
  ' ', ' ', ' ',  '7', ' ', ' ',  '4', '5', ' ',
  ' ', ' ', ' ',  ' ', '3', ' ',  '9', ' ', ' '
};

enum class keys {
  // unmodified
  UP        = KEY_UP,
  LEFT      = KEY_LEFT,
  DOWN      = KEY_DOWN,
  RIGHT     = KEY_RIGHT,
  // shift
  S_UP      = KEY_SR,
  S_LEFT    = KEY_SLEFT,
  S_DOWN    = KEY_SF,
  S_RIGHT   = KEY_SRIGHT,
  //
  CHECK     = KEY_F(3)
};

struct cursor_info {
  int x_pos = 0;
  int y_pos = 0;
};

int pos_to_scr(const int n) {
  return (n + (n/3)) - ((n == 9) * 2);
};

int main() {
  cursor_info cursor;
  sudoq::grid grid;
  grid = data;

  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, true);

  sudoq::display(grid);
  move(pos_to_scr(cursor.y_pos), pos_to_scr(cursor.x_pos));
  refresh();

  for (int ch = getch(); ch != KEY_F(4); ch = getch()) {
    switch (keys(ch)) {
      case keys::UP: --cursor.y_pos; break;
      case keys::LEFT: --cursor.x_pos; break;
      case keys::DOWN: ++cursor.y_pos; break;
      case keys::RIGHT: ++cursor.x_pos; break;
      case keys::S_UP: cursor.y_pos -= 3; break;
      case keys::S_LEFT: cursor.x_pos -= 3; break;
      case keys::S_DOWN: cursor.y_pos += 3; break;
      case keys::S_RIGHT: cursor.x_pos += 3; break;
      case keys::CHECK: {
        bool x = check(grid);
        move(13, 0);
        if (x) {
          printw("Valid solution!");
          clrtobot();
        } else {
          printw("Invalid solution!");
        }
        refresh();
      };
    }

    cursor.x_pos = std::clamp(cursor.x_pos, 0, 9);
    cursor.y_pos = std::clamp(cursor.y_pos, 0, 9);

    switch (ch) {
      case ' ': grid.insert(cursor.y_pos, cursor.x_pos, ' '); break;
      case '1': grid.insert(cursor.y_pos, cursor.x_pos, '1'); break;
      case '2': grid.insert(cursor.y_pos, cursor.x_pos, '2'); break;
      case '3': grid.insert(cursor.y_pos, cursor.x_pos, '3'); break;
      case '4': grid.insert(cursor.y_pos, cursor.x_pos, '4'); break;
      case '5': grid.insert(cursor.y_pos, cursor.x_pos, '5'); break;
      case '6': grid.insert(cursor.y_pos, cursor.x_pos, '6'); break;
      case '7': grid.insert(cursor.y_pos, cursor.x_pos, '7'); break;
      case '8': grid.insert(cursor.y_pos, cursor.x_pos, '8'); break;
      case '9': grid.insert(cursor.y_pos, cursor.x_pos, '9'); break;
    }

    sudoq::display(grid);
    move(pos_to_scr(cursor.y_pos), pos_to_scr(cursor.x_pos));
    refresh();

  }

  endwin();
}

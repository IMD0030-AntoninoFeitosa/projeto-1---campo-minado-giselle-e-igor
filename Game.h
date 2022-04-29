#ifndef GAME_H
#define GAME_H

#include <vector>

enum class Difficulty { beginner, intermediary, advanced };

struct Cell {
  bool is_hidden {true};
  bool has_bomb {false};
  bool has_flag {false};
  short qnt_bombs;
};

using Map = std::vector<std::vector<Cell>>;

//show map
void show_map(Map & map);

//generate map
void generate_map(Map & map, int n_bombs);

//game over
void end_game();

//action
void action(Map & map, int px, int py);

#endif

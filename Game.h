#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

const std::string BEGINNER_RANKING_FILE = "b_ranking.txt";
const std::string INTERMEDIARY_RANKING_FILE = "i_ranking.txt";
const std::string ADVANCED_RANKING_FILE = "a_ranking.txt";

enum class Difficulty { beginner, intermediary, advanced };

struct Cell {
  bool is_hidden {true};
  bool has_bomb {false};
  bool has_flag {false};
  short qnt_bombs;
};

using Map = std::vector<std::vector<Cell>>;

struct Point{
  short x,y;
};

struct Game{
  short total_bombs;
  Point mapDimensions;
};

//Show map every turn accoding to the states of each cell.
void show_map(Game game,  Map map);

//Stores adaptative game settings based on the selected difficulty.
Game create_game(Difficulty difficulty);

//Generates map when called.
Map create_map(Game game);

//End the game and stores player information to the ranking if they win.
void end_game(bool hasFailed, int seconds);

//Reveal all bombs locations.
void show_bombs(Game game, Map & map);

//Check if the position is valid.
bool is_valid(Game game, Map & map, int x, int y);

//Count mines around cell.
int count_bombs(Game game, Map & map, int x, int y);

//Count flags around cell.
int count_flags(Game game, Map & map, int x, int y);

//Reveals what's around the selected cell.
void reveal_around(Game game, Map &map, int x,int y);

//Clear cells around 0's.
void clear_neighbor(Game game, Map & map, int x, int y);

//Receives player's input
bool player_input(short &x, short &y);

//Checks if the player won the game.
bool check_victory(Game game, Map map);

#endif

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

//MOSTRA O MAPA AO JOGADOR (VERIFICA O ESTADO DAS CÉLULAS)
void show_map(Game game,  Map map);

//Armazena os dados do jogo de acordo com a dificuldade selecionada.
Game create_game(Difficulty difficulty);

//GERA O MAPA, É CHAMADA APENAS APÓS O PRIMEIRO INPUT DO JOGADOR
Map create_map(Game game);

//CHAMADA QND O JOGO ACABA (VITÓRIA OU DERROTA)
void end_game(bool hasFailed, int seconds);

//(AINDA PENSANDO EM COMO ESSA VAI FUNCIONAR/SE É NECESSÁRIA)
void action(Map & map, int px, int py);

void show_bombs(Game game, Map & map);

//Checa se uma posição é válida
bool is_valid(Game game, Map & map, int x, int y);

//Count mines around cell
int count_bombs(Game game, Map & map, int x, int y);

//Count flags around cell
int count_flags(Game game, Map & map, int x, int y);

void reveal_around(Game game, Map &map, int x,int y);

// Limpa as celulas em volta de uma celula com 0
void clear_neighbor(Game game, Map & map, int x, int y);

bool player_input(short &x, short &y);

#endif

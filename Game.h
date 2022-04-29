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

struct Point{
  int x,y;
};

using Map = std::vector<std::vector<Cell>>;

//MOSTRA O MAPA AO JOGADOR (VERIFICA O ESTADO DAS CÉLULAS)
void show_map(Map & map, Difficulty difficulty);

//GERA O MAPA, É CHAMADA APENAS APÓS O PRIMEIRO INPUT DO JOGADOR
void generate_map(Map & map, int n_bombs);

//CHAMADA QND O JOGO ACABA (VITÓRIA OU DERROTA)
void end_game();

//(AINDA PENSANDO EM COMO ESSA VAI FUNCIONAR/SE É NECESSÁRIA)
void action(Map & map, int px, int py);

#endif

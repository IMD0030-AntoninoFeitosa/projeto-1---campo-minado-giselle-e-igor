#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "Game.h"


Game create_game(Difficulty difficulty){
  Game gameSetup;
  
  Point mapDimensions;
  
  if (difficulty == Difficulty::beginner){
    mapDimensions.x = 10;
    mapDimensions.y = 10;
    gameSetup.total_bombs = 10;
  }
  else if (difficulty == Difficulty::intermediary){
    mapDimensions.x = 15;
    mapDimensions.y = 15;
    gameSetup.total_bombs = 40;
  }
  else if (difficulty == Difficulty::advanced){
    mapDimensions.x = 15;
    mapDimensions.y = 30;
    gameSetup.total_bombs = 100;
  }

  gameSetup.mapDimensions = mapDimensions;

  return gameSetup;
}


Map create_map(Game game){
  Map map;
  Cell genericCell;
  for (int i = 0; i < game.mapDimensions.x; i++){
    std::vector<Cell> cellList;
    for (int j = 0; j < game.mapDimensions.y; j++){
      cellList.push_back(genericCell);
    }
    map.push_back(cellList);
  }

  //Placing mines according to the difficulty of the game
  for (int i = 0; i < game.total_bombs;){
    int random = rand() % (game.mapDimensions.x * game.mapDimensions.y);
    int x = random / game.mapDimensions.x;
    int y = random % game.mapDimensions.y;

    // Add the mine if no mine is placed at this pos
    if (map[x][y].has_bomb == false){
        map[x][y].has_bomb = true;
        i++;
    }
  }

  // Count adjacent mines
  for (int i = 0; i < game.mapDimensions.x; i++){
    for (int j = 0; j < game.mapDimensions.y; j++){
      map[i][j].qnt_bombs = count_bombs(game, map, i, j);
    }
  }
  
  return map;
}

void show_map(Game game, Map map){
  for (int i = 0; i < game.mapDimensions.x; i++){
    std::cout << i << " ";
    for (int j = 0; j < game.mapDimensions.y; j++){
      if (map[i][j].is_hidden == true){
        std::cout << "-";
      }
      else {
        if(map[i][j].has_bomb == true){
          std::cout << "X";
        }
        else if(map[i][j].has_flag == true){
          std::cout << "F";
        }
        else{
          std::cout << map[i][j].qnt_bombs;
        }
      }
      std::cout << "  ";
    }
    std::cout << std::endl;
  }
}

void end_game(bool hasFailed){
  if (hasFailed){
    std::cout << "RUIM" << std::endl;
    return;
  }

  std::cout << "CONGRATS" << std::endl;
  std::cout << "ENTER YOUR NAME" << std::endl;
  //(adicionando nome do jogador ao arquivo de ranking)
  std::cout << "SUCCESSFULLY RECORDED" << std::endl;
}

bool is_valid(Game game, int row, int col)
{
    return (row >= 0) && (row < game.mapDimensions.x) &&
           (col >= 0) && (col < game.mapDimensions.y);
}

int count_bombs(Game game, Map & map, int x, int y) {
  int count = 0;
  if (is_valid(game, x - 1, y - 1)) {
    if (map[x-1][y-1].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x - 1, y)) {
    if (map[x-1][y].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x - 1, y + 1)) {
    if (map[x-1][y+1].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x, y - 1)) {
    if (map[x][y-1].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x, y + 1)) {
    if (map[x][y+1].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x + 1, y - 1)) {
    if (map[x+1][y-1].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x + 1, y)) {
    if (map[x+1][y].has_bomb){
      count++;
    }
  }
  if (is_valid(game, x + 1, y + 1)) {
    if (map[x+1][y+1].has_bomb){
      count++;
    }
  }
  return count;
}

void clear_neighbor(Game game, Map & map, int x, int y) {
  if (map[x][y].qnt_bombs == 0) {
    if (is_valid(game, x - 1, y - 1) && map[x-1][y-1].is_hidden) {
      map[x-1][y-1].is_hidden = false;
      clear_neighbor(game, map, x - 1, y - 1);
    }
    if (is_valid(game, x - 1, y) && map[x-1][y].is_hidden) {
      map[x-1][y].is_hidden = false;
      clear_neighbor(game, map, x - 1, y);
    }
    if (is_valid(game, x - 1, y + 1) && map[x-1][y+1].is_hidden) {
      map[x-1][y+1].is_hidden = false;
      clear_neighbor(game, map, x - 1, y + 1);
    }
    if (is_valid(game, x, y - 1) && map[x][y-1].is_hidden) {
      map[x][y-1].is_hidden = false;
      clear_neighbor(game, map, x, y - 1);
    }
    if (is_valid(game, x, y + 1) && map[x][y+1].is_hidden) {
      map[x][y+1].is_hidden = false;
      clear_neighbor(game, map, x, y + 1);
    }
    if (is_valid(game, x + 1, y - 1) && map[x+1][y-1].is_hidden) {
      map[x+1][y-1].is_hidden = false;
      clear_neighbor(game, map, x + 1, y - 1);
    }
    if (is_valid(game, x + 1, y) && map[x+1][y].is_hidden) {
      map[x+1][y].is_hidden = false;
      clear_neighbor(game, map, x + 1, y);
    }
    if (is_valid(game, x + 1, y + 1) && map[x+1][y+1].is_hidden) {
      map[x+1][y+1].is_hidden = false;
      clear_neighbor(game, map, x + 1, y + 1);
    }
  }
}
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

  return map;
}

void show_map(Game game, Map map){
  for (int i = 0; i < game.mapDimensions.x; i++){
    for (int j = 0; j < game.mapDimensions.y; j++){
      if (map[i][j].is_hidden == true){
        std::cout << "º";
      }
      else {
        std::cout << "X";
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

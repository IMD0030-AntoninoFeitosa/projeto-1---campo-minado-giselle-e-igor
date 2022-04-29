#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "Game.h"

void show_map(Map &map, Difficulty difficulty){
  
  Point mapDimensions;
  
  if (difficulty == Difficulty::beginner){
    mapDimensions.x = 10;
    mapDimensions.y = 10;
  }
  else if (difficulty == Difficulty::intermediary){
    mapDimensions.x = 15;
    mapDimensions.y = 15;
  }
  else if (difficulty == Difficulty::advanced){
    mapDimensions.x = 15;
    mapDimensions.y = 30;
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

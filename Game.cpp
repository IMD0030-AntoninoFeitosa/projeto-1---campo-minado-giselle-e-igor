#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "Game.h"

void show_map(Map map, Difficulty difficulty){
  
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

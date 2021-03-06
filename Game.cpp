#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <time.h> 
#include <iomanip>
#include <sstream>
#include <chrono>

#include "Game.h"

bool check_victory(Game game, Map map){
  for (int i = 0; i < game.mapDimensions.y; i++){
    for (int j = 0; j < game.mapDimensions.x; j++){
      if(map[j][i].has_bomb && map[j][i].has_flag)
        continue;
      else if (map[j][i].is_hidden && !map[j][i].has_bomb)
        return false;
    }
  }
  return true;
}

bool player_input(short &x, short &y, Game game, std::chrono::high_resolution_clock::time_point &time, bool &firstMovement){
  char c;
  
  std::cin>> c;

  if (c == 't'){
    int seconds = 0;
    if (!firstMovement){
      auto result = std::chrono::high_resolution_clock::now() - time;
      seconds = std::chrono::duration_cast<std::chrono::seconds>(result).count();
    }
    std::cout << seconds << " seconds have passed, and counting..." << std::endl;
    std::cin.clear();
    std::cin.ignore();
    return player_input(x,y,game,time,firstMovement);
  }
  
  std::cin>> y >> x;

  while (std::cin.fail()){
    std::cin.clear();
    std::cin.ignore();
    std::cout << "INVALID MOVEMENT! Try again:" << std::endl;
    std::cin>> c >> y >> x;
  }

  if (firstMovement){
    time = std::chrono::high_resolution_clock::now();
    firstMovement = false;
  }
  
  if (c == 'f' || c == 'F'){
    if (x >= game.mapDimensions.y || y >= game.mapDimensions.x){
      std::cout << "INVALID COORDINATES! Try again:" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      return player_input(x, y, game, time,firstMovement);
    }
    return true;
  }

  else if (c == 'r' || c == 'R'){
    if (y >= game.mapDimensions.x || x >= game.mapDimensions.y){
      std::cout << "INVALID COORDINATES! Try again:" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      return player_input(x, y, game, time,firstMovement);
    }
  }
  
  else{
    std::cout << "INVALID MOVEMENT! Maybe you meant to type 'r' or 'f' before the coordinates?" << std::endl;
    std::cout << "Try again:" << std::endl;
    return player_input(x, y, game, time,firstMovement);
  }
  
  return false;
}

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
    mapDimensions.x = 30;
    mapDimensions.y = 15;
    gameSetup.total_bombs = 100;
  }

  gameSetup.mapDimensions = mapDimensions;

  return gameSetup;
}

Map create_map(Game game){
  Map map;
  Cell genericCell;

  std::srand(time(NULL));
  
  for (int i = 0; i < game.mapDimensions.y; i++){
    std::vector<Cell> cellList;
    for (int j = 0; j < game.mapDimensions.x; j++){
      cellList.push_back(genericCell);
    }
    map.push_back(cellList);
  }

  //Placing mines according to the difficulty of the game
  for (int i = 0; i < game.total_bombs;){
    int random = rand() % (game.mapDimensions.x * game.mapDimensions.y);
    int x = random / game.mapDimensions.x;
    int y = random % game.mapDimensions.x;

    // Add the mine if no mine is placed at this pos
    if (map[x][y].has_bomb == false){
        map[x][y].has_bomb = true;
        i++;
    }
  }

  // Count adjacent mines
  for (int i = 0; i < game.mapDimensions.y; i++){
    for (int j = 0; j < game.mapDimensions.x; j++){
      map[i][j].qnt_bombs = count_bombs(game, map, i, j);
    }
  }
  
  return map;
}


void show_map(Game game, Map map){
  std::cout << std::endl;
  
  for (int i = 0; i < game.mapDimensions.y; i++){
    std::cout << std::setfill('0') << std::setw(2) << i << "   ";
    for (int j = 0; j < game.mapDimensions.x; j++){
      if (map[i][j].is_hidden == true){
        std::cout << "-";
      }
      else {
        if(map[i][j].has_flag == true){
          std::cout << "F";
        }
        else if(map[i][j].has_bomb == true){
          std::cout << "X";
        }
        else{
          std::cout << map[i][j].qnt_bombs;
        }
      }
      std::cout << "  ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "    ";
  for (int i = 0; i < game.mapDimensions.x; i++){
    std::cout << std::setfill('0') << std::setw(2) << i << " ";
  }
  std::cout << std::endl;
  for (int i = 0; i <= game.mapDimensions.x; i++){
    std::cout << "---";
  }
  std::cout << std::endl;
}

void end_game(bool hasFailed, int seconds){
  std::string name;
  
  if (hasFailed){
    std::cout << "Game Over!! Better luck next time~" << std::endl;
    return;
  }
  
  std::fstream file;
  file.open(BEGINNER_RANKING_FILE, std::fstream::app);
  
  std::cout << "Congratulations! You finished the level in " << seconds << " seconds!!"<< std::endl;
  std::cout << "Please enter your name:" << std::endl;
  std::cin >> name;
  if (file.is_open()){
    file << name << ";" << seconds << "\n";
    std::cout << "Data sucessfully recorded!" << std::endl;
    file.close();
  }
  else{
    std::cout << "ERROR: Failed to register data." << std::endl;
  }
}


void show_bombs(Game game, Map & map) {
  for (int i = 0; i < game.mapDimensions.y; i++){
    for (int j = 0; j < game.mapDimensions.x; j++){
      if (map[i][j].has_bomb)
        map[i][j].is_hidden = false;
    }
  }
}

bool is_valid(Game game, int row, int col)
{
    return (row >= 0) && (row < game.mapDimensions.y) &&
           (col >= 0) && (col < game.mapDimensions.x);
}


int count_flags(Game game, Map & map, int x, int y) {
  int count = 0;
  if (is_valid(game, x - 1, y - 1)) {
    if (map[x-1][y-1].has_flag){
      count++;
    }
  }
  if (is_valid(game, x - 1, y)) {
    if (map[x-1][y].has_flag){
      count++;
    }
  }
  if (is_valid(game, x - 1, y + 1)) {
    if (map[x-1][y+1].has_flag){
      count++;
    }
  }
  if (is_valid(game, x, y - 1)) {
    if (map[x][y-1].has_flag){
      count++;
    }
  }
  if (is_valid(game, x, y + 1)) {
    if (map[x][y+1].has_flag){
      count++;
    }
  }
  if (is_valid(game, x + 1, y - 1)) {
    if (map[x+1][y-1].has_flag){
      count++;
    }
  }
  if (is_valid(game, x + 1, y)) {
    if (map[x+1][y].has_flag){
      count++;
    }
  }
  if (is_valid(game, x + 1, y + 1)) {
    if (map[x+1][y+1].has_flag){
      count++;
    }
  }
  return count;
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

bool reveal_around(Game game, Map &map, int x,int y){
  int count = 0;
  if (is_valid(game, x - 1, y - 1)) {
    if (!map[x-1][y-1].has_flag){
      map[x-1][y-1].is_hidden = false;
      if (map[x-1][y-1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x - 1, y - 1);
    }
  }
  if (is_valid(game, x - 1, y)) {
    if (!map[x-1][y].has_flag){
      map[x-1][y].is_hidden = false;
      if (map[x-1][y].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x - 1, y);
    }
  }
  if (is_valid(game, x - 1, y + 1)) {
    if (!map[x-1][y+1].has_flag){
      map[x-1][y+1].is_hidden = false;
      if (map[x-1][y+1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x - 1, y + 1);
    }
  }
  if (is_valid(game, x, y - 1)) {
    if (!map[x][y-1].has_flag){
      map[x][y-1].is_hidden = false;
      if (map[x][y-1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x, y - 1);
    }
  }
  if (is_valid(game, x, y + 1)) {
    if (!map[x][y+1].has_flag){
      map[x][y+1].is_hidden = false;
      if (map[x][y+1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x, y + 1);
    }
  }
  if (is_valid(game, x + 1, y - 1)) {
    if (!map[x+1][y-1].has_flag){
      map[x+1][y-1].is_hidden = false;
      if (map[x+1][y-1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x + 1, y - 1);
    }
  }
  if (is_valid(game, x + 1, y)) {
    if (!map[x+1][y].has_flag){
      map[x+1][y].is_hidden = false;
      if (map[x+1][y].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x + 1, y);
    }
  }
  if (is_valid(game, x + 1, y + 1)) {
    if (!map[x+1][y+1].has_flag){
      map[x+1][y+1].is_hidden = false;
      if (map[x+1][y+1].has_bomb){
        return game_lost(game, map);
      }
      clear_neighbor(game, map, x + 1, y + 1);
    }
  }
  return false;
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

bool game_lost(Game game, Map map) {
  show_bombs(game, map);
  show_map(game, map);
  return true;
}

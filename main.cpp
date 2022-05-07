#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iomanip>

#include "Game.h"

const std::string CONFIG_FILE = "config.cfg";

void show_usage(void){
  std::cout << "Usage: game [option]" << std::endl;
  std::cout << "Option:" << std::endl;
  std::cout << " -h or --help                  Display this information." << std::endl;
  std::cout << " -d or --difficulty <option>   Change the game difficulty for <option>" << std::endl;
  std::cout << "                               -b or --beginner" << std::endl;
  std::cout << "                               -i or --intermediary" << std::endl;
  std::cout << "                               -a or --advanced" << std::endl;
}

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

bool start_game(Difficulty level){
  
  Game game = create_game(level);
  std::cout << "Welcome to minesweeper! Here are some simple instructions to get started:" << std::endl;
  std::cout << "Type down the x and y coordinates to reveal cells." << std::endl;
  std::cout << "To put flags on the map, type 'f' before the coordinates." << std::endl;
  Map map = create_map(game);
  
  show_map(game, map);

  // Check if the first cell is empty in intermediary
  if (level == Difficulty::intermediary) {
    short x, y;

    bool hasFlag = player_input(x,y);
    
    while (map[x][y].has_bomb || map[x][y].qnt_bombs != 0) {
      map = create_map(game);
    }
    map[x][y].is_hidden = false;
    clear_neighbor(game, map, x, y);
    show_map(game, map);
  }

  // Check if the cell has a number in advanced
  if (level == Difficulty::advanced) {
    short x,y;
    bool hasFlag = player_input(x,y);
    while (map[x][y].has_bomb || map[x][y].qnt_bombs < 1) {
      map = create_map(game);
    }
    map[x][y].is_hidden = false;
    show_map(game, map);
  }

  while (1){
    short x,y;
    bool hasFlag = player_input(x,y);

    if(hasFlag && map[x][y].is_hidden){
      map[x][y].has_flag = true;
      map[x][y].is_hidden = false;
    }
      
    else if(hasFlag && map[x][y].has_flag){
      map[x][y].is_hidden = true;
      map[x][y].has_flag = false;
      show_map(game, map);
      continue;
    }

    else if (map[x][y].is_hidden){
      map[x][y].has_flag = false;
      map[x][y].is_hidden = false;
      if (map[x][y].has_bomb){
        show_bombs(game, map);
        show_map(game, map);
        return true;
      }
      else {
        if (map[x][y].qnt_bombs == count_flags(game,map,x,y) && map[x][y].qnt_bombs != 0)
          reveal_around(game, map, x, y);
        clear_neighbor(game, map, x, y);
      }
      if (check_victory(game, map)){
        show_map(game, map);
        return false;
      }
    }
    show_map(game, map);
  }
}


void store_difficulty(const std::string config_file, Difficulty level){
  std::ofstream file;
  file.open (config_file.c_str(), std::ifstream::out);
  if(file.is_open()){
    switch(level){
      case Difficulty::beginner:
        file << 'b';
        std::cout << "Difficulty set to begginer!" << std::endl;
        break;
      case Difficulty::intermediary:
        file << 'i';
        std::cout << "Difficulty set to intermediary!" << std::endl;
        break;
      case Difficulty::advanced:
        file << 'a';
        std::cout << "Difficulty set to advanced!" << std::endl;
        break;
    }
    file.close();
  }
}


Difficulty load_difficulty(const std::string config_file){
  Difficulty level;
  std::ifstream file;
  file.open (config_file.c_str(), std::ifstream::in);
  if(file.is_open()){
    char c;
    file >> c;
    switch(c){
      case 'b': level = Difficulty::beginner; break;
      case 'i': level = Difficulty::intermediary; break;
      case 'a': level = Difficulty::advanced; break;
    }
    file.close();
  } else {
    store_difficulty(config_file, Difficulty::beginner);
    level = Difficulty::beginner;
  }
  return level;
}


void split(const std::string &s, char c, std::vector<std::string> &v){
  std::string::size_type i = 0;
  std::string::size_type j = s.find(c);
  while (j != std::string::npos){
    v.push_back(s.substr(i, j-i));
    i = ++j;
    j = s.find(c, j);
    if (j == std::string::npos)
      v.push_back(s.substr(i, s.length( )));
  }
}

void sort(std::vector<std::vector<std::string>> &v){
  for (int i = 0; i < v.size()-1; i++){
    for (int j= 0; j < v.size() - i - 1; j++){
      if( std::stoi(v[j][1]) > std::stoi(v[j+1][1]) ){
        std::swap(v[j], v[j+1]);
      }
    }
  }
}

void show_leaderboard(){

  Difficulty level = load_difficulty(CONFIG_FILE);
  std::string levelText;
  std::string line;
  std::ifstream file;
  
  switch(level){
      case Difficulty::beginner:
        levelText = "BEGINNER";
        file.open (BEGINNER_RANKING_FILE.c_str(), std::ifstream::in);
        break;
    
      case Difficulty::intermediary:
        levelText = "INTERMEDIARY";
        file.open (INTERMEDIARY_RANKING_FILE.c_str(), std::ifstream::in);
        break;
    
      case Difficulty::advanced:
        levelText = "ADVANCED";
        file.open (ADVANCED_RANKING_FILE.c_str(), std::ifstream::in);
        break;
    }
  
  std::cout << "LEADERBOARD - - - - - - - - - - - - " << levelText << std::endl;

  std::vector<std::vector<std::string>> users;
  if (file.is_open()){
    while ( getline (file,line) )
    {
      std::vector<std::string> userInfo;
      split(line,';',userInfo);
      users.push_back(userInfo);
    }
    file.close();
    users.shrink_to_fit();
    sort(users);
    for (int i = 0; i < users.size(); i++){
      std::cout << std::setfill('0') << std::setw(2) << i+1 << ": ";
      std::cout << users[i][0] << " - " << users[i][1] << "  seconds."<< std::endl;
    }
  }
    
  else{
    std::cout << "ERROR: no records were found for the " << levelText << " difficulty, please check again later." << std::endl;
  }
}

int main(int argc, char** argv){
  if (argc > 1){
    std::string arg = argv[1];
    if (arg == "-h" || arg == "-help"){
      show_usage();
    }

    else if (arg == "-r" || arg == "--records"){
      show_leaderboard();
    }

    else if (arg == "-d" || arg == "--difficulty"){
      if (argc > 2){
      //PARA FAZER COMPARAÇÕES É NECESSÁRIO CRIAR UMA STRING COM O VALOR DE ARGV[i]
        std::string newlevel = argv[2];
        
        if(newlevel == "-b" || newlevel == "--beginner"){
          store_difficulty(CONFIG_FILE, Difficulty::beginner);
        }
        else if(newlevel == "-i" || newlevel == "--intermediary"){
          store_difficulty(CONFIG_FILE, Difficulty::intermediary);
        }
        else if(newlevel == "-a" || newlevel == "--advanced"){
          store_difficulty(CONFIG_FILE, Difficulty::advanced);
        }
        else {
          std::cout << "Unknown difficulty argument: " << newlevel << std::endl;
          show_usage();
        }
      }
      else {
        std::cout << "It was expected a difficulty for: " << argv[1] << std::endl;
        show_usage();
      }
    }
    else {
      std::cout << "Unknown argument: " << argv[1] << std::endl;
      show_usage();
    }
  }
  else {
    Difficulty level = load_difficulty(CONFIG_FILE);
    auto begin = std::chrono::high_resolution_clock::now();
    
    bool gameResults = start_game(level);
    
    auto result = std::chrono::high_resolution_clock::now() - begin;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(result).count();
    end_game(gameResults, seconds);
  }
  return 0;
}

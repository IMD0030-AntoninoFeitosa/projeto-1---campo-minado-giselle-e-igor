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

bool start_game(Difficulty level, std::chrono::high_resolution_clock::time_point begin){
  
  Game game = create_game(level);
  std::cout << " > > > > > > > > > MINESWEEPER < < < < < < < < <" << std::endl;
  std::cout << std::endl;
  std::cout << "INSTRUCTIONS: Type down the x and y coordinates to reveal hidden cells." << std::endl;
  std::cout << "              To place flags on the map, type 'f' before the coordinates." << std::endl;
  Map map = create_map(game);
  
  show_map(game, map);

  // Check if the first cell is empty in intermediary
  if (level == Difficulty::intermediary) {
    short x, y;

    bool hasFlag = player_input(x,y,game, begin);
    
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
    bool hasFlag = player_input(x,y,game, begin);
    while (map[x][y].has_bomb || map[x][y].qnt_bombs < 1) {
      map = create_map(game);
    }
    map[x][y].is_hidden = false;
    show_map(game, map);
  }

  while (1){
    short x,y;
    bool hasFlag = player_input(x,y,game, begin);

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

    else if (!map[x][y].has_flag){
      map[x][y].has_flag = false;
      if (map[x][y].has_bomb){
        return game_lost(game, map);
      }
      else {
        bool check;
        if (map[x][y].qnt_bombs == count_flags(game,map,x,y) && map[x][y].qnt_bombs != 0 && !map[x][y].is_hidden)
           check = reveal_around(game, map, x, y);
        clear_neighbor(game, map, x, y);
        if (check) {
          return check;
        }  
      }
      
      map[x][y].is_hidden = false;
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

//Support function for splitting strings
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

//Support function for sorting numbers
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
      std::cout << std::setfill('0') << std::setw(2) << std::right << i+1 << ": ";
      std::cout << std::setfill(' ') << std::setw(20) << std::left << users[i][0] << " - " << std::setw(4) << users[i][1] << "  seconds."<< std::endl;
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
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    
    bool gameResults = start_game(level, begin);
    
    auto result = std::chrono::high_resolution_clock::now() - begin;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(result).count();
    end_game(gameResults, seconds);
  }
  return 0;
}

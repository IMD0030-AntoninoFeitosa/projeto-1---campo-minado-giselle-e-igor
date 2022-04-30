#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

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

void show_leaderboard(){
  std::cout << "LEADERBOARD --- BEGINNER" << std::endl;
  for (int i = 0; i < 20; i++){
    std::cout << i+1 << " --- FULANINHOOSSSSS" << std::endl;
  }
}

void start_game(Difficulty level){
  unsigned long seed = 0;
  std::srand(seed);
  
  Game game = create_game(level);
  //MOSTRAR INSTRUÇÕES AO JOGADOR
  Map map = create_map(game);
  
  show_map(game, map);

  for (int i = 0; i < 10; i++){
    short x,y;
    std::cin >> x >> y;
    map[x][y].is_hidden = false;
    show_map(game, map);
    if (map[x][y].has_bomb == true){
      end_game(true);
      break;
    }
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
    start_game(level);
  }
  return 0;
}

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

int main(int argc, char** argv){
    if (argc > 1){
        std::string arg = argv[1];
        if (arg == "-h" || arg == "-help"){
            show_usage();
        }
    }
}

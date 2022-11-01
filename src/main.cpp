#include <boost/algorithm/string.hpp>
#include <gamestate.hpp>
#include <iostream>
#include <vector>

class Settings {
public:
};

class Engine {
public:
  Engine(Settings settings) : settings(settings) {}

private:
  GameState state;
  bool debug = false;
  Settings settings;
};

int main(void) {
  std::cout << "# Welcome to ZuloidPlus." << std::endl;

  GameState game_state;

  for (std::string line; std::getline(std::cin, line);) {
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_space());

    if (tokens.size() == 0) {
      continue;
    } else if (tokens[0] == "uci") {
      std::cout << "id name ZuloidPlus" << std::endl;
      std::cout << "id author Filippo Costa @neysofu" << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (tokens[0] == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (tokens[0] == "quit") {
      break;
    } else if (tokens[0] == "d") {
      auto board = game_state.to_ascii_art();
      std::cout << board << std::endl;
    } else if (tokens[0] == "debug") {
      if (tokens.size() != 2) {
        std::cerr << "Invalid debug command." << std::endl;
        continue;
      } else if (tokens[1] == "on") {
        std::cout << "debug on" << std::endl;
      } else if (tokens[1] == "off") {
        std::cout << "debug off" << std::endl;
      } else {
        std::cout << "debug ?" << std::endl;
      }
    } else if (tokens[0] == "position") {
      if (tokens[1] == "startpos") {
        game_state = GameState();
      } else if (tokens[1] == "fen") {
        // TODO
      }
    }
  }

  return 0;
}

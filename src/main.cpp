#include <boost/algorithm/string.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
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
  std::cout << "# PID: "
            << boost::interprocess::ipcdetail::get_current_process_id()
            << std::endl;

  GameState game_state = GameState();

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
      std::cout << "Farewell, my friend." << std::endl;
      break;
    } else if (tokens[0] == "d") {
      std::cout << game_state.to_ascii_art() << std::endl;
    } else if (tokens[0] == "fen") {
      std::cout << game_state.to_fen() << std::endl;
    } else if (tokens[0] == "debug") {
      if (tokens.size() != 2) {
        std::cerr << "# Invalid debug command." << std::endl;
        continue;
      } else if (tokens[1] == "on") {
        std::cout << "# debug on" << std::endl;
      } else if (tokens[1] == "off") {
        std::cout << "# debug off" << std::endl;
      } else {
        std::cerr << "# Invalid debug value" << std::endl;
      }
    } else if (tokens[0] == "position") {
      if (tokens[1] == "startpos") {
        game_state = GameState();
      } else if (tokens[1] == "fen") {
        // TODO
      }
    } else if (tokens[0] == "go") {
      if (tokens[1] == "perft") {
        auto moves = game_state.legal_moves();
        std::cout << moves.size() << std::endl;
        for (auto move : moves) {
          std::cout << move.to_string() << std::endl;
        }
      }
    } else {
      std::cerr << "# Unknown command: " << tokens[0] << std::endl;
    }
  }

  return 0;
}

#include <boost/algorithm/string.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <chrono>
#include <gamestate.hpp>
#include <iostream>
#include <vector>

class Engine {
public:
  Engine() {
    gs = GameState();
    debug = false;
  }

  GameState game_state() { return gs; }

  void go(std::vector<std::string> tokens) {
    if (tokens.size() >= 2 && tokens[1] == "perft") {
      auto moves = game_state().legal_moves();
      std::cout << moves.size() << std::endl;
      for (auto move : moves) {
        std::cout << move.to_string() << std::endl;
      }
    } else {
      // Empty go command, not implemented yet.
      auto moves = game_state().legal_moves();
      auto best_move = moves[0];
      std::cout << "bestmove " << best_move.to_string() << std::endl;
    }
  }

  GameState gs;
  bool debug;
};

class GoCommandSettings {
public:
  GoCommandSettings(std::string &input) {
    std::vector<std::string> tokens;
    boost::split(tokens, input, boost::is_space());

    for (auto token : tokens) {
      if (token == "wtime") {
        w_time = std::chrono::milliseconds(std::stoi(token));
      } else if (token == "btime") {
        b_time = std::chrono::milliseconds(std::stoi(token));
      } else if (token == "winc") {
        w_increment = std::chrono::milliseconds(std::stoi(token));
      } else if (token == "binc") {
        b_increment = std::chrono::milliseconds(std::stoi(token));
      } else if (token == "movestogo") {
        moves_to_go = std::stoi(token);
      } else if (token == "depth") {
        depth = std::stoi(token);
      } else if (token == "nodes") {
        nodes = std::stoi(token);
      } else if (token == "movetime") {
        move_time = std::chrono::milliseconds(std::stoi(token));
      } else if (token == "infinite") {
        infinite = true;
      }
    }
  }

  std::vector<Move> search_moves;
  bool ponder;
  bool infinite;
  std::chrono::duration<float> w_time;
  std::chrono::duration<float> b_time;
  std::chrono::duration<float> w_increment;
  std::chrono::duration<float> b_increment;
  std::chrono::duration<float> move_time;
  std::optional<uint> moves_to_go;
  std::optional<uint> depth;
  std::optional<uint> nodes;
  std::optional<uint> mate;
};

int main(void) {
  std::cout << "# Welcome to ZuloidPlus." << std::endl;
  std::cout << "# PID: "
            << boost::interprocess::ipcdetail::get_current_process_id()
            << std::endl;

  Engine engine = Engine();

  for (std::string line; std::getline(std::cin, line);) {
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_space());

    if (tokens.size() == 0) {
      continue;
    } else if (tokens[0] == "uci") {
      std::cout << "id name ZuloidPlus\n";
      std::cout << "id author Filippo Costa @neysofu\n";
      std::cout << "uciok" << std::endl;
    } else if (tokens[0] == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (tokens[0] == "quit") {
      std::cout << "Farewell, my friend.\n";
      break;
    } else if (tokens[0] == "ucinewgame") {
      engine.gs = GameState();
    } else if (tokens[0] == "d") {
      std::cout << engine.gs.to_ascii_art() << std::endl;
    } else if (tokens[0] == "fen") {
      std::cout << engine.gs.to_fen() << std::endl;
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
        engine.gs = GameState();
      } else if (tokens[1] == "fen") {
        // TODO
      }
    } else if (tokens[0] == "go") {
      engine.go(tokens);
    } else {
      std::cerr << "# Unknown command: " << tokens[0] << std::endl;
    }
  }

  return 0;
}

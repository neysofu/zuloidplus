#include "engine.hpp"
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <iostream>

GameState Engine::game_state() { return gs; }

Status Engine::evaluate_command(std::string cmd) {
  std::vector<std::string> tokens;
  boost::split(tokens, cmd, boost::is_space());

  if (tokens.size() == 0) {
    return Status::Continue;
  } else if (tokens[0] == "uci") {
    std::cout << "id name ZuloidPlus\n";
    std::cout << "id author Filippo Costa @neysofu\n";
    std::cout << "uciok" << std::endl;
  } else if (tokens[0] == "isready") {
    std::cout << "readyok" << std::endl;
  } else if (tokens[0] == "quit") {
    std::cout << "Farewell, my friend.\n";
    return Status::Quit;
  } else if (tokens[0] == "ucinewgame") {
    gs = GameState();
  } else if (tokens[0] == "d") {
    std::cout << gs.to_ascii_art() << std::endl;
  } else if (tokens[0] == "fen") {
    std::cout << gs.to_fen() << std::endl;
  } else if (tokens[0] == "debug") {
    if (tokens.size() != 2) {
      std::cerr << "# Invalid debug command." << std::endl;
    } else if (tokens[1] == "on") {
      std::cout << "# debug on" << std::endl;
    } else if (tokens[1] == "off") {
      std::cout << "# debug off" << std::endl;
    } else {
      std::cerr << "# Invalid debug value" << std::endl;
    }
  } else if (tokens[0] == "position") {
    if (tokens[1] == "startpos") {
      gs = GameState();
    } else if (tokens[1] == "fen") {
      // TODO
    }
    if (tokens[2] == "moves") {
      for (uint i = 3; i < tokens.size(); i++) {
        // TODO
      }
    }
  } else if (tokens[0] == "go") {
    go(tokens);
  } else {
    std::cerr << "# Unknown command: " << tokens[0] << std::endl;
  }

  return Status::Continue;
}

void Engine::go(std::vector<std::string> tokens) {
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

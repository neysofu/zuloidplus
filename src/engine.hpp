#pragma once

#include "gamestate.hpp"

enum class Status {
  Continue,
  Quit,
};

class Engine {
public:
  Engine() {
    gs = GameState();
    debug = false;
  }

  GameState game_state();

  Status evaluate_command(std::string cmd);

private:
  void go(std::vector<std::string> tokens);

  GameState gs;
  bool debug;
};

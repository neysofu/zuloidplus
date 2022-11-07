#include <cstdint>
#include <gamestate.hpp>
#include <optional>
#include <string>
#include <vector>

GameState::GameState() {
  set_piece_at({0, 0}, {Color::White, PieceKind::Rook});
  set_piece_at({1, 0}, {Color::White, PieceKind::Knight});
  set_piece_at({2, 0}, {Color::White, PieceKind::Bishop});
  set_piece_at({3, 0}, {Color::White, PieceKind::Queen});
  set_piece_at({4, 0}, {Color::White, PieceKind::King});
  set_piece_at({5, 0}, {Color::White, PieceKind::Bishop});
  set_piece_at({6, 0}, {Color::White, PieceKind::Knight});
  set_piece_at({7, 0}, {Color::White, PieceKind::Rook});

  for (int i = 0; i < 8; i++) {
    set_piece_at({i, 1}, {Color::White, PieceKind::Pawn});
  }

  set_piece_at({0, 7}, {Color::Black, PieceKind::Rook});
  set_piece_at({1, 7}, {Color::Black, PieceKind::Knight});
  set_piece_at({2, 7}, {Color::Black, PieceKind::Bishop});
  set_piece_at({3, 7}, {Color::Black, PieceKind::Queen});
  set_piece_at({4, 7}, {Color::Black, PieceKind::King});
  set_piece_at({5, 7}, {Color::Black, PieceKind::Bishop});
  set_piece_at({6, 7}, {Color::Black, PieceKind::Knight});
  set_piece_at({7, 7}, {Color::Black, PieceKind::Rook});

  for (int i = 0; i < 8; i++) {
    set_piece_at({i, 6}, {Color::Black, PieceKind::Pawn});
  }
}

char piece_kind_to_ascii_char(PieceKind kind) {
  switch (kind) {
  case PieceKind::Pawn:
    return 'P';
  case PieceKind::Knight:
    return 'N';
  case PieceKind::Bishop:
    return 'B';
  case PieceKind::Rook:
    return 'R';
  case PieceKind::Queen:
    return 'Q';
  case PieceKind::King:
    return 'K';
  }
  return '\0';
}

char piece_to_ascii_char(Piece pc) {
  char c = piece_kind_to_ascii_char(pc.kind);
  if (pc.color == Color::White) {
    return std::toupper(c);
  } else {
    return std::tolower(c);
  }
}

std::string GameState::to_fen() {
  auto s = std::string();

  for (int y = 7; y >= 0; y--) {
    int empty_columns_count = 0;
    for (int x = 0; x < 8; x++) {
      Coordinate coord = {x, y};
      auto pc = piece_at(coord);
      if (!pc) {
        empty_columns_count++;
        continue;
      }
      if (empty_columns_count > 0) {
        s += std::to_string(empty_columns_count);
        empty_columns_count = 0;
      }
      s += piece_to_ascii_char(pc.value());
    }
    if (empty_columns_count > 0) {
      s += std::to_string(empty_columns_count);
    }

    if (y > 0) {
      s += '/';
    }
  }

  s += " ";
  if (turn == Color::White) {
    s += "w";
  } else {
    s += "b";
  }

  s += " ";

  return s;
}

std::optional<Piece> GameState::piece_at(const Coordinate &c) {
  return pieces[c.i];
}

void GameState::set_piece_at(const Coordinate coord, const Piece pc) {
  pieces[coord.i] = pc;
}

void legal_moves_pawn(GameState gs, Coordinate coord,
                      std::vector<Move> &moves) {}

void legal_moves_knight(GameState gs, Coordinate coord,
                        std::vector<Move> &moves) {
  int offsets[8][2] = {{2, 1},   {1, 2},   {-1, 2}, {-2, 1},
                       {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
  for (auto offset : offsets) {
    Coordinate c = {coord.x() + offset[0], coord.y() + offset[1]};
    if (!c.is_in_bounds()) {
      continue;
    }
    auto pc = gs.piece_at(c);
    if (pc) {
      continue;
    }
    moves.push_back({coord, c});
  }
}

void legal_moves_bishop(GameState gs, Coordinate coord,
                        std::vector<Move> &moves) {
  int offsets[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
  for (auto offset : offsets) {
    for (int i = 1; i < 8; i++) {
      Coordinate c = {coord.x() + offset[0] * i, coord.y() + offset[1] * i};
      if (!c.is_in_bounds()) {
        break;
      }
      auto pc = gs.piece_at(c);
      if (pc) {
        break;
      }
      moves.push_back({coord, c});
    }
  }
}

void legal_moves_rook(GameState gs, Coordinate coord,
                      std::vector<Move> &moves) {
  int offsets[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  for (auto offset : offsets) {
    for (int i = 1; i < 8; i++) {
      Coordinate c = {coord.x() + offset[0] * 1, coord.y() + offset[1] * i};
      if (!c.is_in_bounds()) {
        break;
      }
      auto pc = gs.piece_at(c);
      if (pc) {
        break;
      }
      moves.push_back({coord, c});
    }
  }
}

std::vector<Move> GameState::legal_moves() {
  std::vector<Move> moves;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      auto pc = piece_at({x, y});
      if (!pc || pc.value().color != turn) {
        continue;
      }
      switch (pc.value().kind) {
      case PieceKind::Pawn:
        legal_moves_pawn(*this, {x, y}, moves);
        break;
      case PieceKind::Knight:
        legal_moves_knight(*this, {x, y}, moves);
        break;
      case PieceKind::Bishop:
        legal_moves_bishop(*this, {x, y}, moves);
        break;
      case PieceKind::Rook:
        legal_moves_rook(*this, {x, y}, moves);
        break;
      case PieceKind::Queen:
        break;
      case PieceKind::King:
        break;
      }
    }
  }

  return moves;
}

std::string GameState::to_ascii_art() {
  std::string s = "";
  const std::string number_coordinates = "    a b c d e f g h\n";
  const std::string border = "  +-----------------+\n";

  s += number_coordinates;
  s += border;

  for (int y = 7; y >= 0; y--) {
    s += std::to_string(y + 1);
    s += " | ";
    for (int x = 0; x <= 7; x++) {
      Coordinate coord = {x, y};
      auto piece = piece_at(coord);
      if (piece) {
        s += piece_to_ascii_char(piece.value());
      } else {
        s += ".";
      }
      s += " ";
    }
    s += "| ";
    s += std::to_string(y + 1);
    s += "\n";
  }

  s += border;
  s += number_coordinates;

  return s;
}

std::string Coordinate::to_string() {
  return std::string(1, 'a' + x()) + std::to_string(y() + 1);
}

std::string Move::to_string() { return from.to_string() + to.to_string(); }

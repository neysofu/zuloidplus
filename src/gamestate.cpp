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
  // TODO: pieces

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

std::vector<Move> GameState::legal_moves() {
  std::vector<Move> moves;

  for (int i = 0; i < 64; i++) {
    // TODO: generate moves.
    // Piece pc = piece_at({i / 8, i % 8});
    // switch (pc.kind) {
    // case PieceKind::Rook:
    //  break;
    //}
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

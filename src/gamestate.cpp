#include <cstdint>
#include <gamestate.hpp>
#include <optional>
#include <string>
#include <vector>

GameState::GameState() {
  set_piece_at({0, 0}, {Color::White, PieceKind::Rook});
}

char piece_kind_to_char(PieceKind kind) {
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

std::string GameState::to_fen() {
  auto s = std::string();
  // TODO: pieces

  for (int x = 7; x >= 0; --x) {
    int empty_columns_count = 0;
    for (int y = 0; y < 8; ++y) {
      Coordinate coord = {x, y};
      auto piece = piece_at(coord);
      if (!piece) {
        empty_columns_count++;
        continue;
      }
      if (empty_columns_count > 0) {
        s += std::to_string(empty_columns_count);
        empty_columns_count = 0;
      }
      s += piece_kind_to_char(piece.value().kind);
    }

    if (x > 0) {
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
  return pieces[c.to_index()];
}

void GameState::set_piece_at(const Coordinate coord, const Piece pc) {
  pieces[coord.to_index()] = pc;
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

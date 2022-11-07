#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

typedef uint64_t Bitboard;

/**
 * @brief Provides a mapping between XY coordinates and integers.
 *
 */
class Coordinate {
public:
  uint8_t i;

  Coordinate(int x, int y) { i = (x << 3) | y; }

  Coordinate operator+(Coordinate &c) { return {x() + c.x(), y() + c.y()}; }

  bool is_in_bounds() { return i < 64; }

  std::string to_string();

  uint8_t x() { return i >> 3; }
  u_int8_t y() { return i & 0x7; }
};

class Move {
public:
  Coordinate from;
  Coordinate to;

  std::string to_string();
};

enum class CastlingSide {
  KingSide,
  QueenSide,
};

enum class Color {
  White,
  Black,
};

enum class PieceKind {
  Pawn,
  Knight,
  Bishop,
  Rook,
  Queen,
  King,
};

class Piece {
public:
  Color color;
  PieceKind kind;
};

class GameState {
public:
  GameState();

  std::string to_fen();

  std::string to_ascii_art();

  std::optional<Piece> piece_at(const Coordinate &c);

  void set_piece_at(const Coordinate coord, const Piece pc);

  std::vector<Move> legal_moves();

private:
  Color turn;
  std::optional<Piece> pieces[64];
  CastlingSide castling_rights[2];
};

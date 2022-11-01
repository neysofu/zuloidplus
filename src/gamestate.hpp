#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

typedef uint64_t Bitboard;

class Coordinate {
public:
  uint8_t x;
  uint8_t y;

  Coordinate(int x, int y) : x(x), y(y) {}

  uint8_t to_index() const { return x * 8 + y; }
};

class Move {
public:
  Coordinate from;
  Coordinate to;
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

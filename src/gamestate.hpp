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
  Coordinate(int x, int y) {
    _x = x;
    _y = y;
  }

  Coordinate operator+(Coordinate &c) const {
    return {x() + c.x(), y() + c.y()};
  }

  bool is_in_bounds() const { return x() < 8 && y() < 8; }

  std::string to_string() const;

  uint8_t x() const { return _x; }
  u_int8_t y() const { return _y; }

  uint8_t index() const { return (x() << 3) | y(); }

private:
  uint8_t _x;
  uint8_t _y;
};

class Move {
public:
  Coordinate from;
  Coordinate to;

  std::string to_string();
};

enum class CastlingSide {
  King,
  Queen,
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
  Piece(Color color, PieceKind kind) : color(color), kind(kind) {}

  Color color;
  PieceKind kind;
};

class CastlingRights {
public:
  CastlingRights() : rights{true, true, true, true} {}

  void set(Color color, CastlingSide side, bool value) {
    rights[index(color, side)] = value;
  }

  bool get(Color color, CastlingSide side) {
    return rights[index(color, side)];
  }

private:
  uint index(Color color, CastlingSide side) {
    return static_cast<int>(color) * 2 + static_cast<int>(side);
  }

  bool rights[4];
};

class GameState {
public:
  GameState();

  std::string to_fen();

  std::string to_ascii_art();

  std::optional<Piece> piece_at(const Coordinate &c);

  void set_piece_at(const Coordinate coord, const std::optional<Piece> pc);

  void do_move(Move move);

  std::vector<Move> legal_moves();

  Color whose_turn();

  void set_turn(Color c);

  CastlingRights &get_castling_rights();

private:
  Color turn;
  std::optional<Piece> pieces[64];
  CastlingRights castling_rights;
};

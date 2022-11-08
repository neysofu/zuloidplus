#include <boost/algorithm/string.hpp>
#include <cstdint>
#include <gamestate.hpp>
#include <optional>
#include <string>
#include <vector>

GameState::GameState() {
  turn = Color::White;
  castling_rights = CastlingRights();

  set_piece_at({0, 0}, Piece(Color::White, PieceKind::Rook));
  set_piece_at({1, 0}, Piece(Color::White, PieceKind::Knight));
  set_piece_at({2, 0}, Piece(Color::White, PieceKind::Bishop));
  set_piece_at({3, 0}, Piece(Color::White, PieceKind::Queen));
  set_piece_at({4, 0}, Piece(Color::White, PieceKind::King));
  set_piece_at({5, 0}, Piece(Color::White, PieceKind::Bishop));
  set_piece_at({6, 0}, Piece(Color::White, PieceKind::Knight));
  set_piece_at({7, 0}, Piece(Color::White, PieceKind::Rook));

  for (int i = 0; i < 8; i++) {
    set_piece_at({i, 1}, Piece(Color::White, PieceKind::Pawn));
  }

  set_piece_at({0, 7}, Piece(Color::Black, PieceKind::Rook));
  set_piece_at({1, 7}, Piece(Color::Black, PieceKind::Knight));
  set_piece_at({2, 7}, Piece(Color::Black, PieceKind::Bishop));
  set_piece_at({3, 7}, Piece(Color::Black, PieceKind::Queen));
  set_piece_at({4, 7}, Piece(Color::Black, PieceKind::King));
  set_piece_at({5, 7}, Piece(Color::Black, PieceKind::Bishop));
  set_piece_at({6, 7}, Piece(Color::Black, PieceKind::Knight));
  set_piece_at({7, 7}, Piece(Color::Black, PieceKind::Rook));

  for (int i = 0; i < 8; i++) {
    set_piece_at({i, 6}, Piece(Color::Black, PieceKind::Pawn));
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
  return pieces[c.index()];
}

void GameState::set_piece_at(const Coordinate coord,
                             const std::optional<Piece> pc) {
  pieces[coord.index()] = pc;
}

void legal_moves_pawn(GameState gs, Coordinate coord,
                      std::vector<Move> &moves) {
  // Single steps
  Coordinate offset = {0, 1};
  if (gs.whose_turn() == Color::Black) {
    offset = {0, -1};
  }

  auto target = coord + offset;
  if (!gs.piece_at(target)) {
    moves.push_back({coord, target});
  }

  // Double steps
  if ((gs.whose_turn() == Color::White && coord.y() == 1) ||
      (gs.whose_turn() == Color::Black && coord.y() == 6)) {

    Coordinate offset = {0, 2};
    if (gs.whose_turn() == Color::Black) {
      offset = {0, -2};
    }

    // TODO: check if there is a piece in between
    auto target = coord + offset;
    if (!gs.piece_at(target)) {
      moves.push_back({coord, target});
    }
  }

  // Captures
  Coordinate capture_offset[2] = {{1, 1}, {-1, 1}};
  if (gs.whose_turn() == Color::Black) {
    capture_offset[0] = {1, -1};
    capture_offset[1] = {-1, -1};
  }

  for (auto &offset : capture_offset) {
    auto target = coord + offset;
    if (!target.is_in_bounds()) {
      continue;
    }
    auto pc = gs.piece_at(target);
    if (pc && pc->color != gs.whose_turn()) {
      moves.push_back({coord, target});
    }
  }
}

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
    if (pc && pc.value().color == gs.whose_turn()) {
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
      if (pc && pc.value().color == gs.whose_turn()) {
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
      if (pc && pc.value().color == gs.whose_turn()) {
        break;
      }
      moves.push_back({coord, c});
    }
  }
}

void legal_moves_king(GameState gs, Coordinate coord,
                      std::vector<Move> &moves) {
  int offsets[8][2] = {{0, 1}, {0, -1}, {1, 0},  {-1, 0},
                       {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (auto offset : offsets) {
    Coordinate c = {coord.x() + offset[0], coord.y() + offset[1]};
    if (!c.is_in_bounds()) {
      continue;
    }
    auto pc = gs.piece_at(c);
    if (pc && pc.value().color == gs.whose_turn()) {
      continue;
    }
    moves.push_back({coord, c});
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
        legal_moves_rook(*this, {x, y}, moves);
        legal_moves_bishop(*this, {x, y}, moves);
        break;
      case PieceKind::King:
        legal_moves_king(*this, {x, y}, moves);
        break;
      }
    }
  }

  return moves;
}

Color GameState::whose_turn() { return turn; }

void GameState::set_turn(Color c) { turn = c; }

CastlingRights &GameState::get_castling_rights() { return castling_rights; }

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

std::string Coordinate::to_string() const {
  return std::string(1, 'a' + x()) + std::to_string(y() + 1);
}

std::string Move::to_string() { return from.to_string() + to.to_string(); }

std::optional<Piece> char_to_piece(char c) {
  PieceKind kind;
  switch (c) {
  case 'P':
    kind = PieceKind::Pawn;
    break;
  case 'N':
    kind = PieceKind::Knight;
    break;
  case 'B':
    kind = PieceKind::Bishop;
    break;
  case 'R':
    kind = PieceKind::Rook;
    break;
  case 'Q':
    kind = PieceKind::Queen;
    break;
  case 'K':
    kind = PieceKind::King;
    break;
  case 'p':
    kind = PieceKind::King;
    break;
  default:
    return std::nullopt;
  }
  if (islower(c)) {
    return Piece(Color::Black, kind);
  } else {
    return Piece(Color::White, kind);
  }
}

class ParsedFen {
  ParsedFen(std::string input) {
    GameState gs = GameState();

    std::vector<std::string> tokens;
    boost::split(tokens, input, boost::is_space());

    auto err = std::invalid_argument("Invalid FEN string");

    if (tokens.size() != 6) {
      throw err;
    }

    auto pieces = tokens[0];
    for (int i = 0; i < 8; i++) {
      if (std::isalpha(pieces[i])) {
        auto pc = char_to_piece(pieces[i]);
        gs.set_piece_at({i, 1}, pc.value());
      } else if (std::isdigit(pieces[i])) {
        int n = pieces[i] - '0';
        for (int j = 0; j < n; j++) {
          gs.set_piece_at({i + j, 1}, std::nullopt);
        }
        i += n - 1;
      } else if (pieces[i] == '/') {
        continue;
      } else {
        throw err;
      }
    }

    auto turn_indicator = tokens[1];
    if (turn_indicator == "w") {
      turn = Color::White;
    } else if (turn_indicator == "b") {
      turn = Color::Black;
    } else {
      throw err;
    }

    for (auto c : tokens[2]) {
      if (c == '-') {
        castling_rights.set(Color::White, CastlingSide::King, false);
        castling_rights.set(Color::White, CastlingSide::Queen, false);
        castling_rights.set(Color::Black, CastlingSide::King, false);
        castling_rights.set(Color::Black, CastlingSide::Queen, false);
        break;
      }
      if (c == 'K') {
        castling_rights.set(Color::White, CastlingSide::King, true);
      } else if (c == 'Q') {
        castling_rights.set(Color::White, CastlingSide::Queen, true);
      } else if (c == 'k') {
        castling_rights.set(Color::Black, CastlingSide::King, true);
      } else if (c == 'q') {
        castling_rights.set(Color::Black, CastlingSide::Queen, true);
      } else {
        throw err;
      }
    }

    halfmove_clock = std::stoi(tokens[4]);
    fullmove_count = std::stoi(tokens[5]);
  }

public:
  std::optional<Piece> pieces[8][8];
  Color turn;
  CastlingRights castling_rights;
  std::optional<Coordinate> en_passant_target;
  uint halfmove_clock;
  uint fullmove_count;
};

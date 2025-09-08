#include "Board.h"

#include <iostream>
Board::Board(bool who_starts) 
	:white_king({ 4, who_starts ? 7 : 0 }, TypeAndColor::white)
	,black_king({ 4, who_starts ? 0 : 7 }, TypeAndColor::black) 
{
	init_array();
	init_textures();
}

void Board::init_array() {
	// Initialize the board with empty spaces
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			piece[row][col] = Piece();
		}
	}

	// Set up the white pieces
	piece[0][7] = Piece(white, rook);
	piece[1][7] = Piece(white, knight);
	piece[2][7] = Piece(white, bishop);
	piece[3][7] = Piece(white, queen);
	piece[4][7] = Piece(white, king);
	piece[5][7] = Piece(white, bishop);
	piece[6][7] = Piece(white, knight);
	piece[7][7] = Piece(white, rook);

	for (int i = 0; i < 8; ++i) {
		piece[i][6] = Piece(white, pawn);
	}

	// Set up the black pieces
	piece[0][0] = Piece(black, rook);
	piece[1][0] = Piece(black, knight);
	piece[2][0] = Piece(black, bishop);
	piece[3][0] = Piece(black, queen);
	piece[4][0] = Piece(black, king);
	piece[5][0] = Piece(black, bishop);
	piece[6][0] = Piece(black, knight);
	piece[7][0] = Piece(black, rook);

	for (int i = 0; i < 8; ++i) {
		piece[i][1] = Piece(black, pawn);
	}
}
void Board::init_textures() {
	static const std::string teams[2] = { "white","black" };
	static const std::string types[6] = { "pawn","rook","knight","bishop","queen","king" };
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 6; j++) {
			textures[i][j].loadFromFile("pieces/" + types[j] + "_" + teams[i] + ".png");
		}
}

void Board::render(sf::RenderWindow* window) {
	static sf::Sprite sprite;
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			TypeAndColor::Color team = piece[x][y].get_team();
			TypeAndColor::Type type = piece[x][y].get_type();
			if (team == TypeAndColor::no_team)
				continue;
			sprite.setTexture(textures[team - 1][type - 1]);
			sprite.setPosition(x * Chess::tileSize + Chess::tileSize * 0.15 + Chess::offset,
				y * Chess::tileSize + Chess::tileSize * 0.15 + Chess::offset);
			window->draw(sprite);
		}
	}
}
uint8_t Board::validate_move(const Move& move, bool turn) {
	auto [from, to] = move;
	if ((turn && (piece[from.x][from.y].get_team() == white)) || (!turn && (piece[from.x][from.y].get_team() == black))) {
		if (check_move(move, piece) && simulate_move(move, piece[from.x][from.y].get_team())) {
			if (piece[from.x][from.y].get_type() == pawn && (to.y == 0 || to.y == 7)) {
				return 2;
			}
			return 1;
		}
	}
	return 0;
}

void Board::piece_move(const Move& move) {
	auto [from, to] = move;
	if (piece[from.x][from.y].is_first_move()) {
		if (is_castle(from, to, piece)) {
			castle(from, to);
			return;
		}
		piece[from.x][from.y].moved();
		if (piece[from.x][from.y].get_type() == pawn && abs(from.y - to.y) == 2) {
			piece[from.x][from.y].set_enpass(3);
		}
	}
	if (piece[from.x][from.y].get_type() == king) {
		if (piece[from.x][from.y].get_team() == white) {
			white_king.set_position(to);
		}
		else {
			black_king.set_position(to);
		}
	}
	dec_enpass();
	piece[to.x][to.y] = piece[from.x][from.y];
	piece[from.x][from.y] = Piece();
	if (piece[from.x][from.y].get_type() == pawn && abs(from.x - to.x) == 1 && piece[to.x][to.y].get_team() == no_team) {
		piece[to.x][from.y] = Piece();
}
}
bool Board::check_move(const Move& move, Piece(*piece)[8])const {
	auto [from, to] = move;
	Piece piece_checked = piece[from.x][from.y];
	switch (piece_checked.get_type()) {
	case pawn:
		if (piece[to.x][to.y].get_team() == no_team) { // normal move or en passant
			if (piece_checked.is_legal(move, pawn)) {
				if (piece_checked.get_team() == white && from.y > to.y) {
					return true;
				}
				else if (piece_checked.get_team() == black && from.y < to.y) {
					return true;
				}
			}
			else if (piece[to.x][from.y].get_team() != piece[from.x][from.y].get_team() && abs(from.x - to.x) == 1) {
				if (piece[to.x][from.y].get_enpass() > 0) {
					if (piece_checked.get_team() == white && from.y == to.y + 1)
						return true;
					if (piece_checked.get_team() == black && from.y == to.y - 1)
						return true;
				}
			}
			return false;
		}
		else if (is_pawn_capture(move, piece)) {
			return true;
		}
		return false;
	case rook:
		return is_rook_move_valid(move, piece);
	case knight:
		if (piece_checked.is_legal(move, knight) && piece_checked.get_team() != piece[to.x][to.y].get_team())
			return true;
		return false;
	case bishop:
		return is_bishop_move_valid(move, piece);
	case queen:
		return (is_rook_move_valid(move, piece) || is_bishop_move_valid(move, piece));
	case king:
		if (is_castle(from, to, piece))
			return true;
		if (piece_checked.is_legal(move, king)) {
			if (piece[to.x][to.y].get_team() == piece_checked.get_team())
				return false;
			return true;
		}
		return false;
	}
	return false;
}

bool Board::simulate_move(const Move& move, Color team) {
	auto [from, to] = move;
	Piece simpiece[8][8];
	King tempwhiteKing(white_king);
	King tempblackKing(black_king);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			simpiece[i][j] = (piece[i][j]);
		}
	}
	if (piece[from.x][from.y].get_type() == king) {
		if (piece[from.x][from.y].get_team() == white) {
			tempwhiteKing.set_position(to);
		}
		else {
			tempblackKing.set_position(to);
		}
	}
	simpiece[to.x][to.y] = piece[from.x][from.y];
	simpiece[from.x][from.y] = Piece();

	Position kingPos;
	if (team == white) {
		kingPos = tempwhiteKing.get_position();
		if (!is_checked(kingPos, tempwhiteKing.get_team(), simpiece)) {
			return true;
		}
	}
	if (team == black) {
		kingPos = tempblackKing.get_position() ;
		if (!is_checked(kingPos, tempblackKing.get_team(), simpiece)) {
			return true;
		}
	}
	return false;
}

void Board::castle(const Position& kingPos, const Position& newPos) {
	Position rookPos;
	rookPos.x = kingPos.x > newPos.x ? 0 : 7;
	rookPos.y = kingPos.y;

	piece[kingPos.x][kingPos.y].moved();
	piece[rookPos.x][rookPos.y].moved();
	int dxk = kingPos.x > rookPos.x ? -2 : 2;
	int dxr = kingPos.x > rookPos.x ? -1 : 1;
	if (piece[kingPos.x][kingPos.y].get_team() == white) {
		white_king.set_position({ kingPos.x + dxk, kingPos.y });
	}
	else {
		black_king.set_position({kingPos.x + dxk, kingPos.y	});
	}

	Piece temp;
	temp = (piece[kingPos.x][kingPos.y]);

	piece[kingPos.x + dxr][kingPos.y] = piece[rookPos.x][rookPos.y];
	piece[rookPos.x][rookPos.y] = Piece();
	piece[kingPos.x][kingPos.y] = Piece();
	piece[kingPos.x + dxk][kingPos.y] = temp;
}

void Board::promote(const Position& pos, TypeAndColor::Type type) {
	TypeAndColor::Color team = piece[pos.x][pos.y].get_team();
	piece[pos.x][pos.y] = Piece(team, type);
	}

bool Board::is_pawn_capture(const Move& move, Piece(*piece)[8])const {
	auto [from, to] = move;
	if (abs(from.x - to.x) == 1) {
		if (piece[from.x][from.y].get_team() == white && from.y - to.y == 1) {
			if (piece[to.x][to.y].get_team() == black) {
				return true;
			}
		}
		if (piece[from.x][from.y].get_team() == black && to.y - from.y == 1) {
			if (piece[to.x][to.y].get_team() == white) {
				return true;
			}
		}
	}
	return false;
}
bool Board::is_rook_move_valid(const Move& move, Piece(*piece)[8]) const {
	auto [from, to] = move;
	if (!piece[from.x][from.y].is_legal(move, rook)) {
		return false;
	}
	// Check if the movement is vertical
	if (from.x == to.x) {
		int start = (from.y < to.y ? from.y : to.y) + 1;//start and end of the path
		int end = (from.y > to.y ? from.y : to.y);
		for (int i = start; i < end; ++i) {
			if (piece[from.x][i].get_team() != no_team) {
				return false; // Obstacle detected
			}
		}
	}
	// Check if the movement is horizontal
	else if (from.y == to.y) {
		int start = (from.x < to.x ? from.x : to.x) + 1;
		int end = (from.x > to.x ? from.x : to.x);
		for (int i = start; i < end; ++i) {
			if (piece[i][from.y].get_team() != no_team) {
				return false; // Obstacle detected
			}
		}
	}
	// Check the destination square
	if (piece[to.x][to.y].get_team() == piece[from.x][from.y].get_team()) {
		return false; // Destination square has a piece from the same team
	}
	return true; // Movement is valid
}
bool Board::is_bishop_move_valid(const Move& move, Piece(*piece)[8])const {
	auto [from, to] = move;
	if (piece[from.x][from.y].is_legal(move, bishop)) {
		int dx = to.x > from.x ? 1 : -1;
		int dy = to.y > from.y ? 1 : -1;

		int startx = (from.x > to.x ? from.x : to.x) + dx;
		int endx = (from.x > to.x ? to.x : from.x) + dx;
		int j = from.y;
		int i = from.x;
		for (int k = 0; k < 8; k++) {
			i += dx;
			j += dy;
			if (i < 0 || i>7 || j < 0 || j>7)
				return false;
			if (i == to.x && j == to.y)
				if (piece[from.x][from.y].get_team() != piece[to.x][to.y].get_team())
					return true;
			if (piece[i][j].get_team() != no_team)
				return false;
		}
	}
	return false; // Move is not legal for bishop
}
bool Board::is_checked(const Position& pos, Color team_v, Piece(*piece)[8])const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (piece[i][j].get_team() == no_team)
				continue;
			else if (piece[i][j].get_team() != team_v && check_move(Move{ i, j, pos.x, pos.y }, piece)) {
				return true; // King is in check
			}
		}
	}
	return false; // King is not in check
}
bool Board::is_castle(const Position& kingPos, const Position& newPos, Piece(*piece)[8])const {
	if (newPos.x != 2 && newPos.x != 6) {
		return false; // Incorrect target column for castling
	}

	Position rookPos{};

	// Check if the king is moving horizontally during castling
	if (kingPos.y == newPos.y) {
		// Determine the position of the rook based on the king's movement direction
		if (kingPos.x > newPos.x) {
			rookPos.x = 0;
			rookPos.y = kingPos.y;
		}
		else if (kingPos.x < newPos.x) {
			rookPos.x = 7;
			rookPos.y = kingPos.y;
		}
		else {
			return false; // King and rook are not in the same row
		}
	}
	else {
		return false; // King is not moving horizontally during castling
	}

	if ((piece[kingPos.x][kingPos.y].get_type() == king &&
		piece[rookPos.x][rookPos.y].get_type() == rook) &&
		(piece[kingPos.x][kingPos.y].get_team() == piece[rookPos.x][rookPos.y].get_team()))
	{
		if (piece[kingPos.x][kingPos.y].is_first_move() &&
			piece[rookPos.x][rookPos.y].is_first_move())
		{
			if (kingPos.x < rookPos.x) {
				for (int xAxis = kingPos.x + 1; xAxis < rookPos.x; ++xAxis) {
					if (piece[xAxis][kingPos.y].get_type() != empty) {
						return false;
					}
				}
				if (is_checked({ kingPos.x + 1, kingPos.y }, piece[kingPos.x][rookPos.y].get_team(), piece) ||
					is_checked({ kingPos.x + 2, kingPos.y }, piece[kingPos.x][rookPos.y].get_team(), piece)) {
						return false;
					}
				}
			else if (kingPos.x > rookPos.x) {
				for (int xAxis = rookPos.x + 1; xAxis < kingPos.x; ++xAxis) {
					if (piece[xAxis][rookPos.y].get_type() != empty) {
						return false;
					}
				}
				if (is_checked({ kingPos.x - 1, kingPos.y }, piece[kingPos.x][rookPos.y].get_team(), piece) ||
					is_checked({ kingPos.x - 2, kingPos.y }, piece[kingPos.x][rookPos.y].get_team(), piece)) {
					return false;
				}
			}if (!is_checked(kingPos, piece[kingPos.x][kingPos.y].get_team(), piece)) {
				return true;
			}
		}
	}
	return false;
}
bool Board::have_possible_moves(Color team) {
	for (int fromX = 0; fromX < 8; fromX++) {
		for (int fromY = 0; fromY < 8; fromY++) {
			if (piece[fromX][fromY].get_team() != team)
				continue;
			for (int toX = 0; toX < 8; toX++) {
				for (int toY = 0; toY < 8; toY++) {
					Move move{ {fromX, fromY}, {toX, toY} };
					if (check_move(move, piece) && simulate_move(move, team)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}
void Board::dec_enpass() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (piece[i][j].get_type() == pawn)
				piece[i][j].set_enpass(piece[i][j].get_enpass() - 1);
		}
	}
}

Chess::gameState Board::game_status() {
	bool white_can_move = have_possible_moves(white);
	Position whiteKingPos{ white_king.get_position() };
	bool white_checked = is_checked(whiteKingPos, white, piece);

	bool black_can_move = have_possible_moves(black);
	Position blackKingPos{ black_king.get_position()};
	bool black_checked = is_checked(blackKingPos, black, piece);

	if (!white_can_move && white_checked) {
		return Chess::blackWon;
	}
	else if (!black_can_move && black_checked) {
		return Chess::whiteWon;
	}
	else if (!white_can_move && !white_checked || !black_can_move && !black_checked) {
		return Chess::stalemate;
	}
	return Chess::ongoing;
}
Board::Board(const Board& other) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			piece[i][j] = other.piece[i][j];
		}
	}
	white_king = other.white_king;
	black_king = other.black_king;
}
void Board::highlight_moves(const Position& pos, uint64_t& possible_moves) {
	possible_moves = 0;
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			Position destination{ x, y };
			bool is_move_legal = check_move({ pos, destination }, piece);
			bool king_not_in_danger = simulate_move({ pos, destination }, piece[pos.x][pos.y].get_team());
			if (is_move_legal && king_not_in_danger) {
				int index = convert_to_index(destination);
				uint64_t mask = (uint64_t)1 << index;
				possible_moves |= mask;
			}
		}
	}
}
void Board::reset() {
	init_array();
	white_king.set_position({ 4, 7 });
	black_king.set_position({ 4, 0 });
}


void Board::tab_display()const {
	std::cout << "\n";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!piece[j][i].get_team())
				std::cout << "0 ";
			else
				std::cout << piece[j][i].get_type() << " ";
		}
		std::cout << "\n";
	}
}
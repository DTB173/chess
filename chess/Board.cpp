#include "Board.h"

#include <iostream>
Board::Board(bool who_starts):white_king(4, who_starts ? 7 : 0, 1), black_king(4, who_starts ? 0 : 7, 2) {
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
	piece[0][7] = Piece(white, rook); // rook
	piece[1][7] = Piece(white, knight); // knight
	piece[2][7] = Piece(white, bishop); // bishop
	piece[3][7] = Piece(white, queen); // queen
	piece[4][7] = Piece(white, king); // king
	piece[5][7] = Piece(white, bishop); // bishop
	piece[6][7] = Piece(white, knight); // knight
	piece[7][7] = Piece(white, rook); // rook

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
	std::string teams[2] = { "white","black" };
	std::string types[6] = { "pawn","rook","knight","bishop","queen","king" };
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 6; j++) {
			textures[i][j].loadFromFile("pieces/" + types[j] + "_" + teams[i] + ".png");
		}
}

void Board::render(sf::RenderWindow* window) {
	sf::Texture texture;
	sf::Sprite sprite;
	for (int current_x = 0; current_x < 8; ++current_x) {
		for (int current_y = 0; current_y < 8; ++current_y) {
			Piece current_piece = piece[current_x][current_y]; // Access in [current_x][current_y] order
			if (current_piece.get_team() == 0)
				continue;
			texture = textures[current_piece.get_team() - 1][current_piece.get_type() - 1];
			sprite.setTexture(texture);
			sprite.setPosition(current_x * size + size * 0.15 + offset, current_y * size + size * 0.15 + offset);
			window->draw(sprite);
		}
	}
}
int Board::validate_move(int current_x, int current_y, int new_x, int new_y, bool turn) {
	if ((turn && (piece[current_x][current_y].get_team() == white)) || (!turn && (piece[current_x][current_y].get_team() == black)))
		if (check_move(current_x, current_y, new_x, new_y, piece) && simulate_move(current_x, current_y, new_x, new_y, piece[current_x][current_y].get_team())) {
			if (piece[current_x][current_y].get_type() == pawn && (new_y == 0 || new_y == 7)) {
				return 2;
			}
			return 1;
		}
	return 0;
}

void Board::piece_move(int current_x, int current_y, int new_x, int new_y) {
	if (piece[current_x][current_y].is_first_move()) {
		if (is_castle(current_x, current_y, new_x, new_y, piece)) {
			castle(current_x, current_y, new_x, new_y);
			return ;
		}
		piece[current_x][current_y].moved();
		if (piece[current_x][current_y].get_type() == pawn && abs(current_y - new_y) == 2) {
			piece[current_x][current_y].set_enpass(3);
		}
	}
	if (piece[current_x][current_y].get_type() == king) {
		if (piece[current_x][current_y].get_team() == white) {
			white_king.set_position(new_x, new_y);
		}
		else {
			black_king.set_position(new_x, new_y);
		}
	}
	dec_enpass();
	piece[new_x][new_y].piece_copy(piece[current_x][current_y]);
	piece[current_x][current_y] = Piece();
	if (piece[new_x][current_y].get_enpass() > 0)
		piece[new_x][current_y] = Piece();
	std::cout << "\nwhite king: " << is_checked(white_king.get_x(), white_king.get_y(), white_king.get_team(), piece) << " black king: " << is_checked(black_king.get_x(), black_king.get_y(), black_king.get_team(), piece) << "\n"; ;
	//tab_display(piece);
}
bool Board::check_move(int current_x, int current_y, int new_x, int new_y, Piece(*piece)[8])const {
	Piece piece_checked = piece[current_x][current_y];
	switch (piece_checked.get_type()) {
	case pawn:
		if (piece[new_x][new_y].get_team() == no_team) {//normal move/el passant
			if (piece_checked.is_legal(current_x, current_y, new_x, new_y, pawn)) {
				if (piece_checked.get_team() == white && current_y > new_y) {//ensuring forward mvement
					return true;
				}
				else if (piece_checked.get_team() == black && current_y < new_y) {
					return true;
				}
			}
			else if (piece[new_x][current_y].get_team() !=piece[current_x][current_y].get_team()) {//checking for enpassant
				if (piece[new_x][current_y].get_enpass() > 0) {
					if (piece_checked.get_team() == white && current_y == new_y+1)
						return true;
					if (piece_checked.get_team() == black && current_y == new_y-1)
						return true;
				}
				return false;
			}
		}
		else if (is_pawn_capture(current_x, current_y, new_x, new_y,piece)) {//checking if capture can be performed
			return true;
		}
		return false;
		break;
	case rook:
		return is_rook_move_valid(current_x, current_y, new_x, new_y, piece);
		break;
	case knight:
		if (piece_checked.is_legal(current_x, current_y, new_x, new_y, knight)&&piece_checked.get_team()!=piece[new_x][new_y].get_team())
			return true;
		return false;
		break;
	case bishop:
		return is_bishop_move_valid(current_x, current_y, new_x, new_y, piece);
		break;
	case queen:
		return (is_rook_move_valid(current_x, current_y, new_x, new_y, piece) || is_bishop_move_valid(current_x, current_y, new_x, new_y, piece));
		break;
	case king:
		if (is_castle(current_x, current_y, new_x, new_y, piece))
			return true;
		if (piece_checked.is_legal(current_x, current_y, new_x, new_y, king)) {
			if (piece[new_x][new_y].get_team() == piece_checked.get_team())
				return false;
			return true;
		}
		return false;
		break;
	}
	return false;
}

bool Board::simulate_move(int current_x, int current_y, int new_x, int new_y, int team) {
	Piece simpiece[8][8];
	King tempwhiteKing(white_king);
	King tempblackKing(black_king);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			simpiece[i][j].piece_copy(piece[i][j]);
		}
	}
	if (piece[current_x][current_y].get_type() == king) {
		if (piece[current_x][current_y].get_team() == white) {
			tempwhiteKing.set_position(new_x, new_y);
		}
		else {
			tempblackKing.set_position(new_x, new_y);
		}
	}
	simpiece[new_x][new_y].piece_copy(piece[current_x][current_y]);
	simpiece[current_x][current_y] = Piece();
	//tab_display(simpiece);
	if (team == white) {
		if (!is_checked(tempwhiteKing.get_x(), tempwhiteKing.get_y(), tempwhiteKing.get_team(), simpiece)) {
			return true;
		}
	}
	if (team == black) {
		if (!is_checked(tempblackKing.get_x(), tempblackKing.get_y(), tempblackKing.get_team(), simpiece)) {
			return true;
		}
	}
	return false;
}

void Board::castle(int king_x, int king_y, int new_x, int new_y) {
	int rook_x, rook_y;
	if (king_x > new_x) {
		rook_x = 0;
	}
	else {
		rook_x = 7;
	}
	rook_y = king_y;	

	Piece temp;
	piece[king_x][king_y].moved();
	piece[rook_x][rook_y].moved();
	int dxk = king_x > rook_x ? -2 : 2;
	int dxr = king_x > rook_x ? -1 : 1;
	if (piece[king_x][king_y].get_team() == white) {
		white_king.set_position(king_x + dxk, king_y);
	}
	else {
		black_king.set_position(king_x + dxk, king_y);
	}
	temp.piece_copy(piece[king_x][king_y]);

	piece[king_x + dxr][king_y].piece_copy(piece[rook_x][rook_y]);
	piece[rook_x][rook_y] = Piece();
	piece[king_x][king_y] = Piece();
	piece[king_x + dxk][king_y].piece_copy(temp);
}
void Board::promote(int x, int y,int type) {
	int team = piece[x][y].get_team();
	piece[x][y] = Piece();
	switch (type) {
		case rook:piece[x][y] = Piece(team, rook); break;
		case knight:piece[x][y] = Piece(team, knight); break;
		case bishop:piece[x][y] = Piece(team, bishop); break;
		case queen:piece[x][y] = Piece(team, queen); break;
	}
}

bool Board::is_pawn_capture(int current_x, int current_y, int new_x, int new_y, Piece(*piece)[8])const {
	if (abs(current_x - new_x) == 1) {
		if (piece[current_x][current_y].get_team() == white && current_y - new_y == 1) {
			if (piece[new_x][new_y].get_team() == black) {
				return true;
			}
		}
		if (piece[current_x][current_y].get_team() == black && new_y - current_y == 1) {
			if (piece[new_x][new_y].get_team() == white) {
				return true;
			}
		}
	}
	return false;
}
bool Board::is_rook_move_valid(int current_x, int current_y, int new_x, int new_y, Piece(*piece)[8]) const {
	if (!piece[current_x][current_y].is_legal(current_x, current_y, new_x, new_y, rook)) {
		return false;
	}
	// Check if the movement is vertical
	if (current_x == new_x) {
		int start = (current_y < new_y ? current_y : new_y) + 1;//start and end of the path
		int end = (current_y > new_y ? current_y : new_y);
		for (int i = start; i < end; ++i) {
			if (piece[current_x][i].get_team() != no_team) {
				return false; // Obstacle detected
			}
		}
	}
	// Check if the movement is horizontal
	else if (current_y == new_y) {
		int start = (current_x < new_x ? current_x : new_x) + 1;
		int end = (current_x > new_x ? current_x : new_x);
		for (int i = start; i < end; ++i) {
			if (piece[i][current_y].get_team() != no_team) {
				return false; // Obstacle detected
			}
		}
	}
	// Check the destination square
	if (piece[new_x][new_y].get_team() == piece[current_x][current_y].get_team()) {
		return false; // Destination square has a piece from the same team
	}
	return true; // Movement is valid
}
bool Board::is_bishop_move_valid(int current_x, int current_y, int new_x, int new_y, Piece(*piece)[8])const {
	if (piece[current_x][current_y].is_legal(current_x, current_y, new_x, new_y, bishop)) {
		int dx = new_x > current_x ? 1 : -1;
		int dy = new_y > current_y ? 1 : -1;

		int startx = (current_x > new_x ? current_x : new_x) + dx;
		int endx = (current_x > new_x ? new_x : current_x) + dx;
		int j = current_y;
		int i = current_x;
		for (int k = 0; k < 8; k++) {
			i += dx;
			j += dy;
			if (i < 0 || i>7 || j < 0 || j>7)
				return false;
			if (i == new_x && j == new_y)
				if (piece[current_x][current_y].get_team() != piece[new_x][new_y].get_team())
					return true;
			if (piece[i][j].get_team() != no_team)
				return false;
		}
	}
	return false; // Move is not legal for bishop
}
bool Board::is_checked(int current_x, int current_y, int team_v, Piece(*piece)[8])const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (piece[i][j].get_team()==no_team)
				continue;
			else if (piece[i][j].get_team() != team_v && check_move(i, j, current_x, current_y, piece)) {
				return true; // King is in check
			}
		}
	}
	return false; // King is not in check
}
bool Board::is_castle(int king_x, int king_y, int new_x, int new_y,Piece(*piece)[8])const {
	if (new_x != 2 && new_x != 6) {
		return false; // Incorrect target column for castling
	}

	int rook_x, rook_y;

	// Check if the king is moving horizontally during castling
	if (king_y == new_y) {
		// Determine the position of the rook based on the king's movement direction
		if (king_x > new_x) {
			rook_x = 0;
			rook_y = king_y;
		}
		else if (king_x < new_x) {
			rook_x = 7;
			rook_y = king_y;
		}
		else {
			return false; // King and rook are not in the same row
		}
	}
	else {
		return false; // King is not moving horizontally during castling
	}

	if ((piece[king_x][king_y].get_type() == king && piece[rook_x][rook_y].get_type() == rook) && (piece[king_x][king_y].get_team() == piece[rook_x][rook_y].get_team())) {
		if (piece[king_x][king_y].is_first_move() && piece[rook_x][rook_y].is_first_move()) {
			if (king_x < rook_x) {
				for (int current_x = king_x + 1; current_x < rook_x; current_x++) {
					if (piece[current_x][king_y].get_type() != empty) {
						return false;
					}
					if (is_checked(king_x + 1, king_y, piece[king_x][rook_y].get_team(), piece) || is_checked(king_x + 2, king_y, piece[king_x][rook_y].get_team(), piece)) {
						return false;
					}
				}
			}
			else if (king_x > rook_x) {
				for (int current_x = rook_x + 1; current_x < king_x; current_x++) {
					if (piece[current_x][rook_y].get_type() != empty) {
						return false;
					}
				}
				if (is_checked(king_x - 1, king_y, piece[king_x][rook_y].get_team(), piece) || is_checked(king_x - 2, king_y, piece[king_x][rook_y].get_team(), piece)) {
					return false;
				}
			}if (!is_checked(king_x, king_y, piece[king_x][king_y].get_team(), piece)) {
				return true;
			}
		}
	}
	return false;
}
bool Board::have_possible_moves(int team) {
	for (int current_x = 0; current_x < 8; current_x++) {
		for (int current_y = 0; current_y < 8; current_y++) {
			if (piece[current_x][current_y].get_team() != team)
				continue;
			for (int new_x = 0; new_x < 8; new_x++) {
				for (int new_y = 0; new_y < 8; new_y++) {
					if (check_move(current_x, current_y, new_x, new_y, piece) && simulate_move(current_x, current_y, new_x, new_y, team)) {
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

int Board::game_status() {
	bool white_can_move = have_possible_moves(white);
	bool white_checked = is_checked(white_king.get_x(), white_king.get_y(), white, piece);
	bool black_can_move = have_possible_moves(black);
	bool black_checked = is_checked(black_king.get_x(), black_king.get_y(), black, piece);
	if (white_can_move&&black_can_move) {
		return ongoing;
	}
	else if (!white_can_move&&white_checked) {
		return black;
	}else if(!black_can_move&&black_checked) {
		return white;
	}
	else if(!white_can_move && !white_checked || !black_can_move && !black_checked) {
		return stalemate;
	}
}

void Board::highlight_moves(int current_x, int current_y, uint64_t& possible_moves) {
	for (int new_x = 0; new_x < 8; new_x++) {
		for (int new_y = 0; new_y < 8; new_y++) {
			if (check_move(current_x, current_y, new_x, new_y, piece) && simulate_move(current_x, current_y, new_x, new_y, piece[current_x][current_y].get_team())) {
				int index = convert_to_index(new_x, new_y);
				uint64_t mask = (uint64_t)1 << index;
				possible_moves |= mask;
			}
		}
	}
}
void Board::reset() {
	init_array();
	white_king.set_position(4, 7);
	black_king.set_position(4, 0);
}
void Board::tab_display(Piece(*array)[8])const {
	std::cout << "\n";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!array[j][i].get_team())
				std::cout << "0 ";
			else
				std::cout << array[j][i].get_type() << " ";
		}
		std::cout << "\n";
	}
}
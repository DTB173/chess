#include "Board.h"
#include <iostream>
Board::Board() {
	init_array();
	init_textures();
	whiteKing.team = 1;
	whiteKing.x = 4;
	whiteKing.y = 7;
	whiteKing.is_alive = true;

	blackKing.team = 2;
	blackKing.x = 4;
	blackKing.y = 0;
	blackKing.is_alive = true;
}

void Board::render(sf::RenderWindow* window) {
	sf::Texture texture;
	sf::Sprite sprite;
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			Piece current_piece = piece[x][y]; // Access in [x][y] order
			if (current_piece.get_team() == 0)
				continue;
			texture = textures[current_piece.get_team() - 1][current_piece.get_type() - 1];
			sprite.setTexture(texture);
			sprite.setPosition(x * size + size * 0.15 + offset, y * size + size * 0.15 + offset);
			window->draw(sprite);
		}
	}
}

bool Board::piece_move(int x, int y, int nx, int ny, bool turn) {
	if ((turn && (piece[x][y].get_team() == 1)) || (!turn && (piece[x][y].get_team() == 2))) {
		if (check_move(x, y, nx, ny,piece) && simulate_move(x, y, nx, ny, piece[x][y].get_team())) {
			if (piece[x][y].is_first_move()) {
				piece[x][y].moved();
				if (piece[x][y].get_type() == 1&&abs(y-ny)==2) {
					piece[x][y].set_enpass(3);
				}
			}
			if (piece[x][y].get_type() == 6) {
				if (piece[x][y].get_team() == 1) {
					whiteKing.move_king(nx, ny);
				}
				else {
					blackKing.move_king(nx, ny);
				}
			}
			if (piece[nx][ny].get_type() == 6) {
				if (piece[nx][ny].get_team() == 1)
					whiteKing.is_alive = false;
				else
					blackKing.is_alive = false;
			}
			dec_enpass();
			piece[nx][ny].piece_copy(piece[x][y]);
			piece[x][y] = Piece();
			if (piece[nx][y].get_enpass() > 0)
				piece[nx][y] = Piece();
			std::cout<<"\nwhite king: " << is_checked(whiteKing.x, whiteKing.y, 1,piece) << " black king: " << is_checked(blackKing.x, blackKing.y, 2,piece) << "\n"; ;
			tab_display(piece);
			return true;
		}else if ((piece[x][y].get_type() == 6 && piece[nx][ny].get_type() == 2) && piece[x][y].get_team() == piece[nx][ny].get_team()) {
			if (piece[x][y].is_first_move() && piece[nx][ny].is_first_move()) {
				if (is_castle(x, y, nx, ny)) {
					castle(x, y, nx, ny);
					return true;
				}
			}
		}		
	}
	return false;
}
bool Board::check_move(int x, int y, int nx, int ny,Piece (*piece)[8])const {
	Piece piece_checked = piece[x][y];
	switch (piece_checked.get_type()) {
	case 1:
		if (piece[nx][ny].get_team() == 0) {
			if (piece_checked.is_legal(x, y, nx, ny, 1)) {
				if (piece_checked.get_team() == 1 && y>ny ) {
					return true;
				}
				else if(piece_checked.get_team() == 2 && y < ny) {
					return true;
				}
			}
			else if (piece[nx][y].get_enpass() > 0 && piece[nx][y].get_team() != piece[x][y].get_team()) {
				return true;
			}
		}
		else if (pawn_attack(x, y, nx, ny)) {
			return true;
		}
		return false;
		break;
	case 2:
		return rook_path(x, y, nx, ny,piece);
		break;
	case 3:
		if (piece_checked.is_legal(x, y, nx, ny, 3))
			return true;
		return false;
		break;
	case 4:
		return bishop_path(x, y, nx, ny,piece);
		break;
	case 5:
		return (rook_path(x, y, nx, ny,piece) || bishop_path(x, y, nx, ny,piece));
		break;
	case 6:
		if (piece_checked.is_legal(x, y, nx, ny, 6)) {
			if (piece[nx][ny].get_team() == piece_checked.get_team())
				return false;
			return true;
		}
		return false;
		break;
	}
	return false;
}
void Board::King::move_king(int nx, int ny) {
	this->x = nx;
	this->y = ny;
}
void Board::dec_enpass() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (piece[i][j].get_type() == 1)
				piece[i][j].set_enpass(piece[i][j].get_enpass() - 1);
		}
	}
}
void Board::tab_display(Piece (*array)[8])const {
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

int Board::game_status(const bool turn)const {
	return 0;
}
bool Board::pawn_attack(int x, int y, int nx, int ny)const {
	if (abs(x - nx) == 1) { //check horizontal
		int dy = (piece[x][y].get_team() == 1 ? y - ny : ny - y); //check vertical with respect to teams
		if (dy==1&&(piece[x][y].get_team() != piece[nx][ny].get_team()) && piece[nx][ny].get_team() != 0) { //check if vertical =1 and if nx ny contains enemy piece
			return true;
		}
	}
	return false;
}
bool Board::rook_path(int x, int y, int nx, int ny,Piece(*piece)[8]) const {
	if (!piece[x][y].is_legal(x, y, nx, ny, 2)) {
		return false;
	}
	// Check if the movement is vertical
	if (x == nx) {
		int start = (y < ny ? y : ny) + 1;//start and end of the path
		int end = (y > ny ? y : ny);
		for (int i = start; i < end; ++i) {
			if (piece[x][i].get_team() != 0) {
				return false; // Obstacle detected
			}
		}
	}
	// Check if the movement is horizontal
	else if (y == ny) {
		int start = (x < nx ? x : nx) + 1;
		int end = (x > nx ? x : nx);
		for (int i = start; i < end; ++i) {
			if (piece[i][y].get_team() != 0) {
				return false; // Obstacle detected
			}
		}
	}
	// Check the destination square
	if (piece[nx][ny].get_team() == piece[x][y].get_team()) {
		return false; // Destination square has a piece from the same team
	}
	return true; // Movement is valid
}
bool Board::bishop_path(int x, int y, int nx, int ny, Piece(*piece)[8])const {
	if (piece[x][y].is_legal(x, y, nx, ny, 4)) {
		int dx = nx > x ? 1 : -1;
		int dy = ny > y ? 1 : -1;

		int startx = (x > nx ? x : nx)+dx;
		int endx = (x > nx?nx:x)+dx;
		int j = y;
		int i = x;
		for (int k = 0; k < 8;k++) {
			i += dx;
			j += dy;
			if (i < 0 || i>7 || j < 0 || j>7)
				return false;
			if (i == nx && j == ny)
				if (piece[x][y].get_team() != piece[nx][ny].get_team())
					return true;
			if (piece[i][j].get_team() != 0)
				return false;
		}
	}
	return false; // Move is not legal for bishop
}
bool Board::is_checked(int x, int y, int team_v, Piece (*piece)[8])const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!piece[i][j].get_team())
				continue;
			else if (piece[i][j].get_team() != team_v && check_move(i, j, x, y,piece)) {
				return true; // King is in check
			}
		}
	}
	return false; // King is not in check
}
bool Board::is_castle(int kx, int ky, int rx, int ry) {
	if (kx < rx) {
		for (int x = kx + 1; x < rx; x++) {
			if (piece[x][ky].get_type() != 0) {
				return false;
			}
			if (is_checked(kx + 1, ky, piece[kx][ry].get_team(), piece) || is_checked(kx + 2, ky, piece[kx][ry].get_team(), piece)) {
				return false;
			}
		}
	}else if (kx > rx) {
		for (int x = rx + 1; x < kx; x++) {
			if (piece[x][ry].get_type() != 0) {
				return false;
			}
		}
		if (is_checked(kx-1, ky, piece[kx][ry].get_team(), piece)|| is_checked(kx - 2, ky, piece[kx][ry].get_team(), piece)) {
			return false;
		}
	}if (!is_checked(kx, ky, piece[kx][ky].get_team(),piece)) {
		return true;
	}
	return false;
}
void Board::castle(int kx, int ky, int rx, int ry) {
	Piece temp;
	piece[kx][ky].moved();
	piece[rx][ry].moved();
	int dxk = kx > rx ? -2 : 2;
	int dxr = kx > rx ? -1 : 1;
	if (piece[kx][ky].get_team() == 1) {
		whiteKing.move_king(kx+dxk, ky);
	}
	else {
		blackKing.move_king(kx+dxk, ky);
	}
	temp.piece_copy(piece[kx][ky]);

	piece[kx+dxr][ky].piece_copy(piece[rx][ry]);
	piece[rx][ry] = Piece();
	piece[kx][ky] = Piece();
	piece[kx + dxk][ky].piece_copy(temp);
}
bool Board::kingstatus(int team)const {
	if (team == 1) {
		return whiteKing.is_alive;
	}
	else {
		return blackKing.is_alive;
	}
}

Board::King Board::copyKing(King king) {
	King kingcopy;
	kingcopy.is_alive = king.is_alive;
	kingcopy.team = king.team;
	kingcopy.x = king.x;
	kingcopy.y = king.y;
	return kingcopy;
}
bool Board::simulate_move(int x, int y, int nx, int ny, int team) {
	Piece simpiece[8][8];
	King tempwhiteKing;
	King tempblackKing;

	tempwhiteKing = copyKing(whiteKing);
	tempblackKing = copyKing(blackKing);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			simpiece[i][j].piece_copy(piece[i][j]);
		}
	}
	if (piece[x][y].get_type() == 6) {
		if (piece[x][y].get_team() == 1) {
			tempwhiteKing.move_king(nx, ny);
		}
		else {
			tempblackKing.move_king(nx, ny);
		}
	}
	simpiece[nx][ny].piece_copy(piece[x][y]);
	simpiece[x][y] = Piece();
	tab_display(simpiece);
	if (team == 1) {
		if (!is_checked(tempwhiteKing.x, tempwhiteKing.y, 1,simpiece)) {
			return true;
		}
	}
	if (team == 2) {
		if (!is_checked(tempblackKing.x, tempblackKing.y, 2,simpiece)) {
			return true;
		}
	}
	return false;
}
void Board::init_array() {
	// Initialize the board with empty spaces
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			piece[row][col] = Piece();
		}
	}

	// Set up the white pieces
	piece[0][7] = Piece(1, 2); // rook
	piece[1][7] = Piece(1, 3); // knight
	piece[2][7] = Piece(1, 4); // bishop
	piece[3][7] = Piece(1, 5); // queen
	piece[4][7] = Piece(1, 6); // king
	piece[5][7] = Piece(1, 4); // bishop
	piece[6][7] = Piece(1, 3); // knight
	piece[7][7] = Piece(1, 2); // rook

	for (int i = 0; i < 8; ++i) {
		piece[i][6] = Piece(1, 1);
	}

	// Set up the black pieces
	piece[0][0] = Piece(2, 2);
	piece[1][0] = Piece(2, 3);
	piece[2][0] = Piece(2, 4);
	piece[3][0] = Piece(2, 5);
	piece[4][0] = Piece(2, 6);
	piece[5][0] = Piece(2, 4);
	piece[6][0] = Piece(2, 3);
	piece[7][0] = Piece(2, 2);

	for (int i = 0; i < 8; ++i) {
		piece[i][1] = Piece(2, 1);
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
#include "Piece.h"
#include <iostream>
void Piece::set_team(int t) {
	team = t;
}
void Piece::set_type(int t) {
	type = t;
}

Piece::Piece(int team_v, int type_v):team(0),type(0) {
	/*
		type:
		 1 - pawn
		 2 - rook
		 3 - knight
		 4 - bishop
		 5 - queen  
	     6 - king	
	*/
	team = team_v;
	type = type_v;
	switch (type) {
	case 1:name = "pawn"; first_move = true; enpass = 0; break;
	case 2:name = "rook"; first_move = true; break;
	case 3:name = "knight"; break;
	case 4:name = "bishop"; break;
	case 5:name = "queen"; break;
	case 6:name = "king"; first_move = true; break;
	default:name = "unknown"; break;
	}
}

Piece Piece::piece_copy(Piece piece_v) {
	this->set_team(piece_v.get_team());
	this->set_type(piece_v.get_type());
	this->name = piece_v.get_name();
	if(piece_v.get_type()==1||piece_v.get_type()==2||piece_v.get_type()==6)
		this->first_move = piece_v.is_first_move();
	if (piece_v.get_type() == 1)
		this->set_enpass(piece_v.get_enpass());
	return *this;
}
void Piece::moved() {
	this->first_move = false;
}
bool Piece::is_legal(int x, int y, int nx, int ny, int type_v)const {
	switch (type_v) {
	case 1:
		if (first_move) {
			if (x == nx && (abs(y - ny) == 1 || abs(y - ny) == 2)) {
				return true;
			}
		}
		else {
			if (x == nx && abs(y - ny) == 1) {
				return true;
			}
		}
		return false;
	break;
	case 2:
		if (x == nx || y == ny) {
			return true;
		}
		return false;
	break;
	case 3:
		if ((abs(x - nx) == 2 && abs(y - ny) == 1) || abs(x - nx) == 1 && abs(y - ny) == 2) {
			return true;
		}
		return false;
	break;
	case 4:
		if (abs(x - nx) == abs(y - ny)) {
			return true;
		}
		return false;
	break;
	case 5:
		if (abs(x - nx) == abs(y - ny) || x == nx || y == ny) {
			return true;
		}
		return false;
	break;
	case 6:
		if (abs(x - nx) <= 1 && abs(y - ny) <= 1) {
			return true;
		}
		return false;
	break;
	}
	return true;
}


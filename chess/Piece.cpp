#include "Piece.h"
#include <iostream>
void Piece::set_team(uint8_t t) {
	team = t;
}
void Piece::set_type(uint8_t t) {
	type = t;
}

Piece::Piece(uint8_t team_v, uint8_t type_v):team(no_team),type(empty) {
	team = team_v;
	type = type_v;
	switch (type) {
	case pawn:name = "pawn"; first_move = true; enpass = 0; break;
	case rook:name = "rook"; first_move = true; break;
	case knight:name = "knight"; break;
	case bishop:name = "bishop"; break;
	case queen:name = "queen"; break;
	case king:name = "king"; first_move = true; break;
	default:name = "unknown"; break;
	}
}

Piece Piece::piece_copy(Piece piece_v) {
	this->set_team(piece_v.get_team());
	this->set_type(piece_v.get_type());
	this->name = piece_v.get_name();
	if(piece_v.get_type()==pawn||piece_v.get_type()==rook||piece_v.get_type()==king)
		this->first_move = piece_v.is_first_move();
	if (piece_v.get_type() == pawn)
		this->set_enpass(piece_v.get_enpass());
	return *this;
}
void Piece::moved() {
	this->first_move = false;
}
bool Piece::is_legal(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, uint8_t type_v)const {
	switch (type_v) {
	case pawn:
		if (first_move) {
			if (current_x == new_x && (abs(current_y - new_y) == 1 || abs(current_y - new_y) == 2)) {
				return true;
			}
		}
		else {
			if (current_x == new_x && abs(current_y - new_y) == 1) {
				return true;
			}
		}
		return false;
	break;
	case rook:
		if (current_x == new_x || current_y == new_y) {
			return true;
		}
		return false;
	break;
	case knight:
		if ((abs(current_x - new_x) == 2 && abs(current_y - new_y) == 1) || abs(current_x - new_x) == 1 && abs(current_y - new_y) == 2) {
			return true;
		}
		return false;
	break;
	case bishop:
		if (abs(current_x - new_x) == abs(current_y - new_y)) {
			return true;
		}
		return false;
	break;
	case queen:
		if (abs(current_x - new_x) == abs(current_y - new_y) || current_x == new_x || current_y == new_y) {
			return true;
		}
		return false;
	break;
	case king:
		if (abs(current_x - new_x) <= 1 && abs(current_y - new_y) <= 1) {
			return true;
		}
		return false;
	break;
	}
	return true;
}
int Piece::get_value() {
	switch (this->get_type()) {
	case pawn:return 1; break;
	case rook:return 5; break;
	case knight:return 3; break;
	case bishop:return 3; break;
	case queen:return 9; break;
	case king:return 1000; break;
	default:return 0; break;
	}
}

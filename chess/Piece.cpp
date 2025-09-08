#include "Piece.h"
#include <iostream>

void Piece::set_team(Color t) {
	m_team = t;
}
void Piece::set_type(Type t) {
	m_type = t;
}

Piece::Piece(Color team_v, Type type_v)
	:m_type{ type_v }
	, m_team{ team_v } {
}

Piece::Piece(const Piece& piece_v) {
	m_team = piece_v.m_team;
	m_type = piece_v.m_type;
	if(piece_v.get_type()==pawn||piece_v.get_type()==rook||piece_v.get_type()==king)
		m_first_move = piece_v.is_first_move();
	if (piece_v.get_type() == pawn)
		m_enpass = piece_v.get_enpass();
}

Piece Piece::operator=(const Piece& piece_v) {
	m_team = piece_v.m_team;
	m_type = piece_v.m_type;
	if (piece_v.get_type() == pawn || piece_v.get_type() == rook || piece_v.get_type() == king)
		m_first_move = piece_v.is_first_move();
	if (piece_v.get_type() == pawn)
		m_enpass = piece_v.get_enpass();
	return *this;
}
void Piece::moved() {
	m_first_move = false;
}
bool Piece::is_legal(const Move& move, uint8_t type_v)const {
	auto [from, to] = move;
	switch (type_v) {
	case pawn:
		if (m_first_move) {
			if (from.x == to.x && (abs(from.y - to.y) == 1 || abs(from.y - to.y) == 2)) {
				return true;
			}
		}
		else {
			if (from.x == to.x && abs(from.y - to.y) == 1) {
				return true;
			}
		}
		return false;
	break;
	case rook:
		if (from.x == to.x || from.y == to.y) {
			return true;
		}
		return false;
	break;
	case knight:
		if ((abs(from.x - to.x) == 2 && abs(from.y - to.y) == 1) || abs(from.x - to.x) == 1 && abs(from.y - to.y) == 2) {
			return true;
		}
		return false;
	break;
	case bishop:
		if (abs(from.x - to.x) == abs(from.y - to.y)) {
			return true;
		}
		return false;
	break;
	case queen:
		if (abs(from.x - to.x) == abs(from.y - to.y) || from.x == to.x || from.y == to.y) {
			return true;
		}
		return false;
	break;
	case king:
		if (abs(from.x - to.x) <= 1 && abs(from.y - to.y) <= 1) {
			return true;
		}
		return false;
	break;
	}
	return true;
}
int Piece::get_value()const {
	static constexpr const int values[] = {0, 1, 5, 3, 3, 9, 1000};
	return values[get_type()];
}

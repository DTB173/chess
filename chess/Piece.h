#pragma once
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>

enum piece_types {
	empty = 0,
	pawn = 1,
	rook = 2,
	knight = 3,
	bishop = 4,
	queen = 5,
	king = 6
};
enum team_color {
	no_team = 0,
	white = 1,
	black = 2
};

class Piece
{
	int team;
	int type;
	std::string name;
	bool first_move = false;
	int enpass=0;
public:
	Piece() : team(empty), type(no_team), name("unknown"), first_move(false),enpass(0) {}
	Piece(uint8_t team_v, uint8_t type_v);
	Piece piece_copy(Piece piece_v);

	int get_team()const { return team; };
	int get_type()const { return type; };
	std::string get_name()const { return name; };
	int get_enpass()const { return enpass; };

	void set_team(uint8_t t);
	void set_type(uint8_t t);
	void set_enpass(int8_t val) { this->enpass = val; };
	void moved();	
	
	bool is_first_move()const { return first_move; };
	bool is_legal(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, uint8_t type_v) const;
	int get_value();
};
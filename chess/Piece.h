#pragma once
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
class Piece
{
	int team;
	int type;
	std::string name;
	bool first_move = false;
	int enpass = false;
public:
	int get_team()const { return team; };
	int get_type()const { return type; };
	std::string get_name()const { return name; };
	Piece piece_copy(Piece piece_v);
	void set_team(int t);
	void set_type(int t);
	void moved();
	int get_enpass()const { return enpass; };
	void set_enpass(int val) { this->enpass=val; };
	bool is_first_move()const { return first_move; };
	Piece() : team(0), type(0), name("unknown"),first_move(false) {}
	Piece(int team_v, int type_v);
	bool is_legal(int x, int y, int nx, int ny, int type_v) const;
};

class Pawn : public Piece{};
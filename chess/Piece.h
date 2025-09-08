#pragma once
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace TypeAndColor {
	enum Type :int {
		empty,
		pawn,
		rook,
		knight,
		bishop,
		queen,
		king,
};
	enum Color {
	no_team = 0,
	white = 1,
		black = 2,
};
}

struct Position {
	int x;
	int y;
};

struct Move{
	Position from;
	Position to;
};

using namespace TypeAndColor;

struct Piece
{
private:
	Color m_team{};
	Type m_type{};
	int m_enpass{};
	bool m_first_move{ true };
public:
	Piece() 
		: m_team{ Color::no_team }
		, m_type{Type::empty} {
	}
	Piece(Color team_v, Type type_v);
	Piece (const Piece& piece_v);
	Piece operator=(const Piece& piece_v);

	Color get_team()const { return m_team; };
	Type get_type()const { return m_type; };
	int get_enpass()const { return m_enpass; };

	void set_team(Color t);
	void set_type(Type t);
	void set_enpass(int8_t val) { m_enpass = val < 0 ? 0 : val; };
	void moved();	
	
	bool is_first_move()const { return m_first_move; };
	bool is_legal(const Move& move, uint8_t type_v) const;
	int get_value()const;
};
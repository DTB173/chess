#pragma once
#include "Piece.h"
using TypeAndColor::Color;

struct King{
private:
	Position m_pos{};
	Color m_team{};
	bool m_is_alive = true;
public:
	King() = default;
	King(const Position& pos, Color team) {
		m_pos = pos;
		m_team = team;
	}
	King(const King& other) {
		m_pos = other.m_pos;
		m_team = other.m_team;
	}
	Position get_position()const { return m_pos; };
	void set_position(const Position& pos){ m_pos = pos; }

	Color get_team()const { return m_team; };

	bool get_status() const { m_is_alive; };
	void set_status(bool status) { m_is_alive = status; };
};


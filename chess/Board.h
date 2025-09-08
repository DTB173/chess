#pragma once
#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "King.h"

namespace Chess {
	constexpr const int tileSize{ 96 };
	constexpr const int offset{ 50 };
	enum gameState : int {
		ongoing = 0,
		whiteWon,
		blackWon,
		stalemate,
	};
};

using TypeAndColor::Color;
using TypeAndColor::Type;

class Board
{
public:
	Piece piece[8][8];
	King white_king;
	King black_king;
	sf::Texture textures[2][6];
	Board(bool who_starts);
	Board(const Board& other);
	void init_array();
	void init_textures();
	void render(sf::RenderWindow* window);

	void piece_move(const Move& move);
	bool check_move(const Move& move,Piece (*arr)[8])const;
	bool simulate_move(const Move& move, Color team);
	void castle(const Position& kingPos, const Position& newPos);
	void promote(const Position& pos, Type type);

	bool is_pawn_capture(const Move& move, Piece(*piece)[8])const;
	bool is_rook_move_valid(const Move& move, Piece(*arr)[8])const;
	bool is_bishop_move_valid(const Move& move, Piece(*arr)[8])const;

	uint8_t validate_move(const Move& move, bool turn);
	bool is_checked(const Position& pos, Color team, Piece(*arr)[8])const;
	bool is_castle(const Position& kingPos, const Position& newPos, Piece(*piece)[8])const;
	bool have_possible_moves(Color team);
	void dec_enpass();
	Chess::gameState game_status();

	Piece get_piece(const Position& pos)const { return piece[pos.x][pos.y]; };
	uint8_t convert_to_index(const Position& pos) { return 8 * pos.y + pos.x; };
	void highlight_moves(const Position& pos, uint64_t& possible_moves);
	void reset();
	void tab_display()const;
};


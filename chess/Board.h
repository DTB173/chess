#pragma once
#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "King.h"
#define size 96
#define offset 50
#define stalemate 3
#define ongoing 0

class Board
{
	Piece piece[8][8];
	sf::Texture textures[2][6];
	King white_king;
	King black_king;
public:
	Board(bool who_starts);
	void init_array();
	void init_textures();
	void render(sf::RenderWindow* window);

	void piece_move(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y);
	bool check_move(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y,Piece (*arr)[8])const;
	bool simulate_move(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, uint8_t team);
	void castle(uint8_t king_x, uint8_t king_y, uint8_t rook_x, uint8_t rook_y);
	void promote(uint8_t x, uint8_t y,uint8_t type);

	bool is_pawn_capture(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, Piece(*piece)[8])const;
	bool is_rook_move_valid(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, Piece(*arr)[8])const;
	bool is_bishop_move_valid(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, Piece(*arr)[8])const;

	uint8_t validate_move(uint8_t current_x, uint8_t current_y, uint8_t new_x, uint8_t new_y, bool turn);
	bool is_checked(uint8_t current_x, uint8_t current_y, uint8_t team, Piece(*arr)[8])const;
	bool is_castle(uint8_t king_x, uint8_t king_y, uint8_t rook_x, uint8_t rook_y, Piece(*piece)[8])const;
	bool have_possible_moves(uint8_t team);
	void dec_enpass();
	uint8_t game_status();

	Piece get_piece(int current_x, int current_y)const { return piece[current_x][current_y]; };
	Board board_copy()const;
	uint8_t convert_to_index(uint8_t current_x, uint8_t current_y) { return 8 * current_y + current_x; };
	void highlight_moves(uint8_t current_x, uint8_t current_y, uint64_t& possible_moves);
	void reset();
	void tab_display(Piece(*array)[8])const;
};


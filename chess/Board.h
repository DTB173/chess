#pragma once
#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "King.h"
#define size 96
#define offset 50

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

	bool piece_move(int current_x, int current_y, int new_x, int new_y, bool turn);
	bool check_move(int current_x, int current_y, int new_x, int new_y,Piece (*arr)[8])const;
	bool simulate_move(int current_x, int current_y, int new_x, int new_y, int team);
	void castle(int king_x, int king_y, int rook_x, int rook_y);
	void promote(int x, int y);

	bool pawn_attack(int current_x, int current_y, int new_x, int new_y)const;
	bool rook_path(int current_x, int current_y, int new_x, int new_y, Piece(*arr)[8])const;
	bool bishop_path(int current_x, int current_y, int new_x, int new_y, Piece(*arr)[8])const;

	bool is_checked(int current_x, int current_y, int team, Piece(*arr)[8])const;
	bool is_castle(int king_x, int king_y, int rook_x, int rook_y);
	void dec_enpass();
	int game_status(const bool turn)const;

	void tab_display(Piece(*array)[8])const;
};


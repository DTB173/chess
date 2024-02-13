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
	King whiteKing = King(4, 7, 1);
	King blackKing = King(4, 0, 2);
public:
	Board();
	void init_array();
	void init_textures();
	void render(sf::RenderWindow* window);

	bool piece_move(int x, int y, int nx, int ny, bool turn);
	bool check_move(int x, int y, int nx, int ny,Piece (*arr)[8])const;
	bool simulate_move(int x, int y, int nx, int ny, int team);
	void castle(int kx, int ky, int rx, int ry);
	void promote(int x, int y);

	bool pawn_attack(int x, int y, int nx, int ny)const;
	bool rook_path(int x, int y, int nx, int ny, Piece(*arr)[8])const;
	bool bishop_path(int x, int y, int nx, int ny, Piece(*arr)[8])const;

	bool is_checked(int x, int y, int team, Piece(*arr)[8])const;
	bool is_castle(int kx, int ky, int rx, int ry);
	void dec_enpass();
	int game_status(const bool turn)const;

	void tab_display(Piece(*array)[8])const;
};


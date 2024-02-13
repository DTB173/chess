#pragma once
#include "Piece.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#define size 96
#define offset 50
class Board
{
	Piece piece[8][8];
	sf::Texture textures[2][6];
	struct King {
		int x;
		int y;
		int team;
		bool is_alive;
		void move_king(int x, int y);
	};
	King whiteKing;
	King blackKing;
public:
	void init_array();
	void init_textures();
	bool piece_move(int x, int y, int nx, int ny, bool turn);
	bool check_move(int x, int y, int nx, int ny,Piece (*arr)[8])const;
	void render(sf::RenderWindow* window);
	Board();
	void dec_enpass();
	void tab_display(Piece (*array)[8])const;
	int game_status(const bool turn)const;
	bool pawn_attack(int x, int y, int nx, int ny)const;
	bool rook_path(int x, int y, int nx, int ny, Piece(*arr)[8])const;
	bool bishop_path(int x, int y, int nx, int ny, Piece(*arr)[8])const;
	bool is_checked(int x, int y, int team_v,Piece (*arr)[8])const;
	bool simulate_move(int x, int y, int nx, int ny, int team);
	bool is_castle(int kx,int ky,int rx,int ry);
	void castle(int kx, int ky, int rx, int ry);
	bool kingstatus(int team)const;
	King copyKing(King king);
};


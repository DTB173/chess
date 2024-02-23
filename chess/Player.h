#pragma once
#include "Board.h"
#include <vector>
class Player
{
public:
	int min_max(Board board,int depth,bool turn, std::vector<int>& picked_move, int &game_status);
	std::vector<int> pick_move(Board& board, bool turn,int &game_status);
	std::vector<std::vector<int>>generate_moves(Board& board,bool turn, int& game_status);
	int eval_material(Board& board, bool turn)const;
};


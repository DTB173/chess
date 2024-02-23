#include "Player.h"
int Player::min_max(Board board, int depth, bool turn,std::vector<int>&picked_move,int &game_status) {
	std::vector<int>move;
	int best;
	std::vector<std::vector<int>>generated_moves = generate_moves(board,turn,game_status);
	if (depth == 0 || board.game_status() != 0)
		return eval_material(board,turn);
	if (turn == white) {
		best = -10000;
		for (std::vector<int> move : generated_moves) {
			board.piece_move(move[0], move[1], move[2], move[3]);
			int val = -min_max(board, depth - 1, !turn, picked_move,game_status);
			if (val > best) {
				best = val;
				//picked_move.clear();
				//picked_move.push_back(move[1]);
				//picked_move.push_back(move[0]);
				//picked_move.push_back(move[3]);
				//picked_move.push_back(move[2]);
			}				
		}
		return best;
	}else {
		best = 10000;
		for (std::vector<int> move : generated_moves) {
			board.piece_move(move[0], move[1], move[2], move[3]);
			int val = min_max(board, depth - 1, !turn, picked_move,game_status);
			if (val < best) {
				best = val;
				picked_move.clear();
				picked_move.push_back(move[0]);
				picked_move.push_back(move[1]);
				picked_move.push_back(move[2]);
				picked_move.push_back(move[3]);
			}
		}
		return best;
	}
	
}
std::vector<int> Player::pick_move(Board& board,bool turn, int &game_status) {
	std::vector<int> best_move;
	min_max(board, 3, turn, best_move,game_status);
	return best_move;
}

std::vector<std::vector<int>> Player::generate_moves(Board& board, bool turn, int& game_status) {
	std::vector<std::vector<int>> moves;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			if ((turn && board.get_piece(i, j).get_team() == white) || (!turn && board.get_piece(i, j).get_team() == black)) {
				uint64_t possible_moves = 0;
				board.highlight_moves(i, j, possible_moves);

				if (possible_moves != 0) {
					uint64_t mask = 1;
					for (int y = 0; y < 8; ++y) {
						for (int x = 0; x < 8; ++x) {
							if (possible_moves & mask) {
								moves.push_back({ i, j, x, y });
							}
							mask <<= 1;
						}
					}
				}
			}
		}
	}
	return moves;
}

int Player::eval_material(Board& board, bool turn)const {
	int value=0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board.get_piece(i, j).get_team() == no_team)
				continue;
			else if (board.get_piece(i, j).get_team() == white) {
				value += board.get_piece(i, j).get_value();
			}
			else if (board.get_piece(i, j).get_team() == black) {
				value -= board.get_piece(i, j).get_value();
			}
		}
	}
	return value;
}

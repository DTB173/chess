#include "Player.h"
#include <iostream>

int Player::min_max(Board board, int depth, bool turn, std::vector<int>& picked_move, int game_status) {
    std::vector<std::vector<int>> generated_moves = generate_moves(board, turn, game_status);
	int best;

    if (depth == 0 || board.game_status() != Chess::ongoing) {
        return eval_material(board, turn);
    }

    if (generated_moves.empty()) {
        return eval_material(board, turn);
    }

    if (turn) { // White's turn (maximizing)
		best = -10000;
        for (const auto& move : generated_moves) {
            Board temp_board(board);
            std::vector<int> temp_move;
            int val = -min_max(temp_board, depth - 1, !turn, temp_move, game_status);
			if (val > best) {
				best = val;
                picked_move = move;
			}				
		}
		return best;
    }
    else { // Black's turn (minimizing)
		best = 10000;
        for (const auto& move : generated_moves) {
            Board temp_board(board);
            temp_board.piece_move({ {move[0], move[1]}, {move[2], move[3]} });
            std::vector<int> temp_move;
            int val = min_max(temp_board, depth - 1, !turn, temp_move, game_status);
			if (val < best) {
				best = val;
                picked_move = move;
			}
		}
		return best;
	}
	
}

std::vector<int> Player::pick_move(Board& board, bool turn, int game_status) {
	std::vector<int> best_move;
    int eval = min_max(board, 2, turn, best_move, game_status);
    if (best_move.empty()) {
        std::vector<std::vector<int>> moves = generate_moves(board, turn, game_status);
        if (!moves.empty()) {
            best_move = moves[0];
        }
    }
	return best_move;
}

std::vector<std::vector<int>> Player::generate_moves(Board& board, bool turn, int& game_status) {
	std::vector<std::vector<int>> moves;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
            if ((turn && board.get_piece({ i, j }).get_team() == TypeAndColor::white) ||
                (!turn && board.get_piece({ i, j }).get_team() == TypeAndColor::black)) {
				uint64_t possible_moves = 0;
                board.highlight_moves({ i, j }, possible_moves);

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

int Player::eval_material(Board& board, bool turn) const {
    int value = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
            if (board.get_piece({ i, j }).get_team() == TypeAndColor::no_team)
				continue;
            else if (board.get_piece({ i, j }).get_team() == TypeAndColor::white) {
                value += board.get_piece({ i, j }).get_value();
			}
            else if (board.get_piece({ i, j }).get_team() == TypeAndColor::black) {
                value -= board.get_piece({ i, j }).get_value();
			}
		}
	}
	return value;
}
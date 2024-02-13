#include "King.h"
King::King(int x_v, int y_v, int team_v) {
	this->current_x = x_v;
	this->current_y = y_v;
	this->team = team_v;
}
King King::copy_king(King king) {
	King kingcopy(king.current_x,king.current_y,king.team);
	return kingcopy;
}

void King::set_position(int new_x, int new_y) {
	this->current_x = new_x;
	this->current_y = new_y;
}
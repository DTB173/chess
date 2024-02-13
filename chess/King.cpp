#include "King.h"
King::King(int x_v, int y_v, int team_v) {
	this->x = x_v;
	this->y = y_v;
	this->team = team_v;
}
King King::copyKing(King king) {
	King kingcopy(king.x,king.y,king.team);
	return kingcopy;
}

void King::set_position(int nx, int ny) {
	this->x = nx;
	this->y = ny;
}
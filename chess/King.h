#pragma once
class King
{
private:
	int x;
	int y;
	int team;
	bool is_alive = true;
public:
	King(int x, int y, int team);
	King copyKing(King king);
	bool get_status() const { return this->is_alive; };
	int get_x()const { return this->x; };
	int get_y()const { return this->y; };
	int get_team()const { return this->team; };
	void set_status(bool status) { this->is_alive = status; };
	void set_position(int x_v, int y_v);
};


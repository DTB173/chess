#pragma once
class King
{
private:
	int current_x;
	int current_y;
	int team;
	bool is_alive = true;
public:
	King(int current_x, int current_y, int team);
	King copy_king(King king_v);
	bool get_status() const { return this->is_alive; };
	int get_x()const { return this->current_x; };
	int get_y()const { return this->current_y; };
	int get_team()const { return this->team; };
	void set_status(bool status) { this->is_alive = status; };
	void set_position(int new_x, int new_y);
};


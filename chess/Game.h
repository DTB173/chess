#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>
class Game
{
	Board board;
	sf::RenderWindow* window;
	sf::Event ev;
	bool ingame;
	bool turn=true;
	sf::Vector2i mouse_pos_1;
	sf::Vector2i mouse_pos_2;
	sf::RectangleShape square_white;
	sf::RectangleShape square_black;
	sf::RectangleShape square_selected;
	sf::RectangleShape possible_move;
	bool first_cord = false;
	bool second_cord = false;
	int current_x, current_y, new_x, new_y;
	uint64_t possible_moves;
	sf::Font font;
	sf::Text text;
	sf::Text cords;
public:
	Game();
	~Game();

	void update();
	void render();

	int game_status()const;
	bool which_turn()const { return turn; };
	bool is_ingame()const { return ingame; };
	void display_highlighted(uint64_t possible_moves);
};


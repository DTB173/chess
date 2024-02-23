#pragma once
#include "Player.h"
//#include "Board.h"
#include "Button.h"
#include <vector>
#include <SFML/Graphics.hpp>
class Game
{
	Board board;
	Player player;
	Button button;
	sf::RenderWindow* window;
	sf::Event ev;
	bool ingame;
	bool ending;
	bool turn=true;
	bool selecting = true;
	sf::Vector2i mouse_pos_1;
	sf::Vector2i mouse_pos_2;
	sf::RectangleShape square_white;
	sf::RectangleShape square_black;
	sf::RectangleShape square_selected;
	sf::RectangleShape possible_move;
	bool first_cord = false;
	bool second_cord = false;
	int current_x, current_y, new_x, new_y;
	int game_status=0;
	std::vector<int>picked_move;
	uint64_t possible_moves;
	sf::Font font;
	sf::Text text;
	sf::Text cords;
public:
	Game();
	~Game();

	void update();
	void render();

	bool which_turn()const { return turn; };
	bool is_ingame()const { return ingame; };
	void display_highlighted(uint64_t possible_moves);
	int display_promotion(int new_y);
};


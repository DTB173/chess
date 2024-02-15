#include "Game.h"
#include <iostream>
Game::Game():board(true) {
	ingame = true;
	window = new sf::RenderWindow(sf::VideoMode(size * 8 + offset * 2, size * 8 + offset * 2), "Chess", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(30);
	square_white = sf::RectangleShape(sf::Vector2f(size, size));
	square_black = sf::RectangleShape(sf::Vector2f(size, size));
	square_selected = sf::RectangleShape(sf::Vector2f(size, size));
	possible_move = sf::RectangleShape(sf::Vector2f(size,size));
	square_white.setFillColor(sf::Color(238,238,210));
	square_black.setFillColor(sf::Color(118,150,86));
	square_selected.setFillColor(sf::Color(186,202,68));
	possible_move.setFillColor(sf::Color(0,255,255,128));

	if (!font.loadFromFile("Fonts/aerial.ttf"))
		std::cout << "nie wczytano czcionki" << std::endl;
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);
	cords.setFont(font);
	cords.setFillColor(sf::Color::Black);
	cords.setCharacterSize(20);
	text.setString("White turn");
	text.setPosition(window->getSize().x / 2 - 90, 10);
	//board.tab_display();
}
Game::~Game() {
	delete window;
	window = nullptr;
}
void Game::update() {
	while (window->pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window->close();
			ingame = false;
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape) {
				window->close();
				ingame = false;
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left && !first_cord && !second_cord) {
				possible_moves = 0;
				mouse_pos_1 = sf::Mouse::getPosition(*window);
				current_x = (mouse_pos_1.x - offset) / size;
				current_y = (mouse_pos_1.y - offset) / size;
				if (current_x < 0 || current_x>7 || current_y < 0 || current_y>7)
					continue;
				first_cord = true;
				square_selected.setPosition(current_x * size + offset, current_y * size + offset);
				board.highlight_moves(current_x, current_y, possible_moves);
			}
			else if (ev.mouseButton.button == sf::Mouse::Left && first_cord && !second_cord) {
				mouse_pos_2 = sf::Mouse::getPosition(*window);
				new_x = (mouse_pos_2.x - offset) / size;
				new_y = (mouse_pos_2.y - offset) / size;
				second_cord = true;
			}
			if (first_cord && second_cord) {
				if (board.piece_move(current_x, current_y, new_x, new_y, turn))
					turn = !turn;
				first_cord = second_cord = false;
			}
		}	
		if (turn)
			text.setString("White turn");
		else
			text.setString("Black turn");
	}
	
	if (board.game_status() != ongoing) {
		if (board.game_status() == white) {
			std::cout << "white won";
		}else if(board.game_status() == black) {
			std::cout << "black won";
		}else if (board.game_status() == stalemate)
			std::cout << "draw";
		//ingame = false;
	}	 
}
void Game::render() {
	window->clear(sf::Color(128, 128, 128));
	const char* ycords[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
	const char* xcords[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	
	for (int i = 0; i < 64; i +=2) {
		// Draw coordinates
		if (i%8==0) {
			cords.setString(ycords[i/8]);
			cords.setPosition(20, i/8 * size + offset + 40);
			window->draw(cords);
		}
		// Draw squares
		if ((i / 8) % 2) {
			square_white.setPosition((i % 8) * size + size + offset, (i / 8) * size + offset);
			square_black.setPosition((i % 8) * size + offset, (i / 8) * size + offset);
		}
		else {
			square_white.setPosition((i % 8) * size + offset, (i / 8) * size + offset);
			square_black.setPosition((i % 8) * size + size + offset, (i / 8) * size + offset);
		}

		window->draw(text);
		window->draw(square_white);
		window->draw(square_black);
	}
	if (first_cord) {
		window->draw(square_selected);
		display_highlighted(possible_moves);
	}
	board.render(window);

	for (int i = 0; i < 8; ++i) {
			cords.setString(xcords[i]);
			cords.setPosition(i * size + offset + 40, (size * 8 + offset * 2) - size + 50);
			window->draw(cords);
	}
	window->display();
}

void Game::display_highlighted(uint64_t possible_moves) {
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			int bit_index = y * 8 + x;
			uint64_t mask = (uint64_t)1 << bit_index;
			if (possible_moves & mask) {
				possible_move.setPosition(x * size + offset, y * size + offset);
				window->draw(possible_move);
			}
		}
	}
}
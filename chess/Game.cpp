#include "Game.h"
#include <iostream>
Game::Game():board(true) {
	using Chess::offset;
	using Chess::tileSize;

	ingame = true;
	ending = false;
	move = { -1,-1,-1,-1 };
	//picked_move = {0,0,0,0};
	possible_moves = 0;
	window = new sf::RenderWindow(sf::VideoMode(tileSize * 8 + offset * 2, tileSize * 8 + offset * 2), "Chess", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(30);
	square_white = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	square_black = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	square_selected = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	possible_move = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	square_white.setFillColor(sf::Color(238,238,210));
	square_black.setFillColor(sf::Color(118,150,86));
	square_selected.setFillColor(sf::Color(186,202,68));
	possible_move.setFillColor(sf::Color(0,255,255,128));

	if (!font.loadFromFile("Fonts/CooperHewitt.otf"))
		std::cout << "nie wczytano czcionki" << std::endl;
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(25);
	cords.setFont(font);
	cords.setFillColor(sf::Color::Black);
	cords.setCharacterSize(20);
	text.setString("White turn");
	text.setPosition(window->getSize().x / 2 - 55, 10);
}
Game::~Game() {
	delete window;
	window = nullptr;
}
void Game::update() {
	using namespace Chess;

	while (window->pollEvent(ev)) {
		if (!turn && !ending) { // Black's turn (AI)
			std::vector<int> ai_move = player.pick_move(board, false, game_status);
			if (!ai_move.empty()) {
				Move m{ {ai_move[0], ai_move[1]}, {ai_move[2], ai_move[3]} };
				int move_type = board.validate_move(m, false);
				if (move_type > 0) {
					board.piece_move(m);
					if (move_type == 2) {
						bool isPromoting = true;
						Color promotionTeam = black;
						Position promotionPos = m.to;
						Type selectedPromotion = TypeAndColor::empty;
					}
					else {
				turn = !turn;
						game_status = board.game_status();
			}
		}
			}
			continue;
		}
		if (ending) {
			if (ev.type == sf::Event::KeyPressed || sf::Event::Closed)
				ingame = false;
			if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
				mouse_pos_1 = sf::Mouse::getPosition(*window);
				if (mouse_pos_1.x > 230 && mouse_pos_1.x < 640 && mouse_pos_1.y>400 && mouse_pos_1.y < 464) {
					game_status = ongoing;
					ending = false;
					turn = white;
					board.reset();
					text.setFillColor(sf::Color::Black);
					text.setCharacterSize(25);
					text.setString("White turn");
					text.setPosition(window->getSize().x / 2 - 45, 10);
					window->clear();
					board.render(window);
					window->display();
				}
			}
		}
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
				move.from.x = static_cast<int>(mouse_pos_1.x - offset) / tileSize;
				move.from.y = static_cast<int>(mouse_pos_1.y - offset) / tileSize;
				if (move.from.x < 0 || move.from.x>7 || move.from.y < 0 || move.from.y>7)
					continue;
				first_cord = true;
				square_selected.setPosition(move.from.x * tileSize + offset, move.from.y * tileSize + offset);
				board.highlight_moves(move.from, possible_moves);
			}
			else if (ev.mouseButton.button == sf::Mouse::Left && first_cord && !second_cord) {
				mouse_pos_2 = sf::Mouse::getPosition(*window);
				move.to.x = static_cast<int>(mouse_pos_2.x - offset) / tileSize;
				move.to.y = static_cast<int>(mouse_pos_2.y - offset) / tileSize;
				second_cord = true;
			}
			if (first_cord && second_cord) {
				int move_type = board.validate_move(move, turn);
				if (move_type > 0) {
					board.piece_move(move);
					if (move_type == 2) {
						auto piece_type = display_promotion(move.to.y);
						board.promote(move.to, piece_type);
					}
					turn = !turn;
					game_status = board.game_status();
				}
				first_cord = second_cord = false;
			}
		}	
		if (turn)
			text.setString("White turn");
		else
			text.setString("Black turn");
	}

	if (game_status != ongoing) {
		if (game_status == whiteWon) {
			text.setString("White won");
		}else if(game_status == blackWon) {
			text.setString("Black won");
		}
		else if (game_status == stalemate)
			text.setString("Draw");
		ending = true;
	}
}
void Game::render() {
	using Chess::offset;
	using Chess::tileSize;
	window->clear(sf::Color(128, 128, 128));
	static const char* ycords[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
	static const char* xcords[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	if (ending) {
		text.setCharacterSize(50);
		if (text.getString() == "Draw") {
			text.setPosition(window->getSize().x / 2-55, window->getSize().y / 3);
		}
		else {
			text.setPosition(window->getSize().x / 3+30, window->getSize().y / 3);
		}
		button.render(window, window->getSize().x/4+10, window->getSize().y / 2-40, 140, 15);
	}
	else {
		for (int i = 0; i < 64; i += 2) {
			// Draw coordinates
			if (i % 8 == 0) {
				cords.setString(ycords[i / 8]);
				cords.setPosition(20, i / 8 * tileSize + offset + 40);
				window->draw(cords);
			}
			// Draw squares
			if ((i / 8) % 2) {
				square_white.setPosition((i % 8) * tileSize + tileSize + offset, (i / 8) * tileSize + offset);
				square_black.setPosition((i % 8) * tileSize + offset, (i / 8) * tileSize + offset);
			}
			else {
				square_white.setPosition((i % 8) * tileSize + offset, (i / 8) * tileSize + offset);
				square_black.setPosition((i % 8) * tileSize + tileSize + offset, (i / 8) * tileSize + offset);
			}
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
			cords.setPosition(i * tileSize + offset + 40, (tileSize * 8 + offset * 2) - tileSize + 50);
			window->draw(cords);
		}
	}
	window->draw(text);
	window->display();
}

void Game::display_highlighted(uint64_t possible_moves) {
	using Chess::offset;
	using Chess::tileSize;

	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			int bit_index = y * 8 + x;
			uint64_t mask = (uint64_t)1 << bit_index;
			if (possible_moves & mask) {
				possible_move.setPosition(x * tileSize + offset, y * tileSize + offset);
				window->draw(possible_move);
			}
		}
	}
}

TypeAndColor::Type Game::display_promotion(int new_y) {
	using Chess::offset;
	using Chess::tileSize;

	int team = new_y == 0 ? white : black;
	sf::RectangleShape tile(sf::Vector2f(tileSize + offset, tileSize + offset));
	tile.setFillColor(sf::Color(238, 238, 210));
	tile.setOutlineColor(sf::Color::Black);
	tile.setOutlineThickness(-5);
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setString("Select piece");
	text.setFillColor(sf::Color::Black);
	text.setPosition(window->getSize().x * 0.33 + 5, window->getSize().y * 0.3 + 15);
	sf::Texture background;
	background.create(window->getSize().x, window->getSize().y);
	background.update(*window);
	sf::Sprite bgsprite(background);
	sf::Sprite sprite;
	int k = -1;
	bool selecting = true;
	while (selecting) {
		window->pollEvent(ev);
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
		if (mouse_pos.y > 368 && mouse_pos.y < 504) {
			if (mouse_pos.x > 155 && mouse_pos.x < 290) k = 0;
			else if (mouse_pos.x > 295 && mouse_pos.x < 431) k = 1;
			else if (mouse_pos.x > 436 && mouse_pos.x < 572) k = 2;
			else if (mouse_pos.x > 577 && mouse_pos.x < 713) k = 3;
			}
		window->clear();
		window->draw(bgsprite);
		window->draw(text);
		for (int i = 0; i < 4; i++) {
			tile.setPosition(window->getSize().x * 0.173 + (tileSize + offset - 5) * i, window->getSize().y * 0.39 + 25);
			if (i == k) {
				tile.setFillColor(sf::Color(118, 150, 86));
				window->draw(tile);
				tile.setFillColor(sf::Color(238, 238, 210));
			}
			else {
				window->draw(tile);
		}
		}
		for (int i = 0; i < 4; i++) {
			sprite.setTexture(board.textures[team - 1][i + 1]); // Use board's textures (rook, knight, bishop, queen)
			sprite.setPosition(window->getSize().x * 0.22 + (tileSize + offset - 5) * i, window->getSize().y * 0.44 + 17);
			window->draw(sprite);
		}
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
			if (ev.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
				if (mouse_pos.y > 368 && mouse_pos.y < 504) {
					if (mouse_pos.x > 155 && mouse_pos.x < 290) return TypeAndColor::rook;
					else if (mouse_pos.x > 295 && mouse_pos.x < 431) return TypeAndColor::knight;
					else if (mouse_pos.x > 436 && mouse_pos.x < 572) return TypeAndColor::bishop;
					else if (mouse_pos.x > 577 && mouse_pos.x < 713) return TypeAndColor::queen;
					}
					}
			break;
		}
		window->display();
	}
	return TypeAndColor::queen; // Fallback (should not reach here)
}

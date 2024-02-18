#include "Button.h"
Button::Button() {
    int size_x = 300, size_y = 50;
    if (!font.loadFromFile("Fonts/CooperHewitt.otf"))
        return;
    if (!bgtexture.loadFromFile("bg.png")) {
        return;
    }
    bgsprite.setTexture(bgtexture);
    bgsprite.setPosition(bgoffset, bgoffset);
    button_text.setFont(font);
    button_text.setFillColor(sf::Color::Black);
    button_text.setString("Play again");
    button_background.setSize(sf::Vector2f(1.4*size_x, 1.5*size_y));
    button_background.setFillColor(sf::Color::White);
}
void Button::render(sf::RenderWindow* window, int pos_x, int pos_y, int offset_x, int offset_y) {
    button_background.setPosition(pos_x, pos_y);
    button_background.setOutlineColor(sf::Color::Black);
    button_background.setOutlineThickness(-5);
    button_text.setPosition(pos_x + offset_x, pos_y + offset_y);
    window->draw(bgsprite);
    window->draw(button_background);
    window->draw(button_text);
}


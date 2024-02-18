#pragma once
#include "SFML/Graphics.hpp"
class Button
{
private:
    int size_x=0;
    int size_y=0;
    int bgoffset=50;
    sf::Text button_text;
    sf::RectangleShape button_background;
    sf::Font font;
    sf::Texture bgtexture;
    sf::Sprite bgsprite;
public:
    Button();
    void render(sf::RenderWindow* window = nullptr, int pos_x = 0, int pos_y = 0, int offset_x = 0, int offset_y = 0);
};


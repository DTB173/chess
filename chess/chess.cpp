#include <SFML/Graphics.hpp>
#include "Game.h"

int main(void) {
    Game game;
    while (game.is_ingame()) {
        game.update();
        game.render();
    }
    return 0;
}

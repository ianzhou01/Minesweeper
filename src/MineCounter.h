#pragma once
#include <SFML/Graphics.hpp>

struct MineCounter {
    int mineCount, startingCount, totalSquares, minesOver;
    sf::Texture digitTextures[11];
    sf::Sprite hundreds, tens, ones, negative;

    MineCounter(int startingCount, const sf::Texture &digits, int rows, int cols);
    void draw(sf::RenderWindow &window);
    void decrement();
    void increment();
    void reset(int mines);
};
#pragma once
#include <SFML/Graphics.hpp>

struct Button {
    sf::Sprite sprite; // Contains the sprite to be actually drawn
    sf::Texture texture;
    sf::Color originalColor, darkColor;
    bool clicked;

    explicit Button(float x, float y, sf::Texture &texture);
    void draw(sf::RenderWindow &window) const;

    // Button state functions
    void update(const sf::Vector2f &mousePos);
    bool isMouseOver(const sf::Vector2f &mousePos) const;
};

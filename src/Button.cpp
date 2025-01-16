#include "Button.h"

Button::Button(float x, float y, sf::Texture &texture) : texture(texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    clicked = false;
    originalColor = sprite.getColor();
    darkColor = originalColor; // Set up clicked color (darker, depressed button)
    darkColor.r /= 2;
    darkColor.g /= 2;
    darkColor.b /= 2;
}

bool Button::isMouseOver(const sf::Vector2f &mousePos) const {
    return sprite.getGlobalBounds().contains(mousePos); // Check if mouse is over button
}

void Button::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

// Button state functions
void Button::update(const sf::Vector2f& mousePos) {
    // Check if the mouse is over the button and left-clicked
    clicked = isMouseOver(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if (clicked) {
        sprite.setColor(darkColor);
    }
    else {
        sprite.setColor(originalColor);
    }
}


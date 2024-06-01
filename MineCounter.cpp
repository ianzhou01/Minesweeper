#include "MineCounter.h"

MineCounter::MineCounter(int startingCount, const sf::Texture &digits, int rows, int cols) : mineCount(startingCount), startingCount(startingCount), totalSquares(rows * cols), minesOver(0) {
    if (startingCount > totalSquares) {
        throw std::runtime_error("Invalid mine count: more mines than squares available.");
    }
    sf::IntRect toCopy(0, 0, 21, 32);
    for (int i=0; i < 11; i++) {
        // Load each number (21 x 32) square into pre-existing textures zero, one...
        toCopy.left = i * 21;
        sf::Sprite digitSprite(digits);
        digitSprite.setTextureRect(toCopy);
        digitTextures[i].loadFromImage(digits.copyToImage(), toCopy);
    }
    negative.setTexture(digitTextures[10]);
    negative.setPosition(12.f, 32.f*((float)rows + 0.5f) + 16.f);
    hundreds.setPosition(33.f, 32.f*((float)rows + 0.5f) + 16.f);
    tens.setPosition(54.f, 32.f*((float)rows + 0.5f) + 16.f);
    ones.setPosition(75.f, 32.f*((float)rows + 0.5f) + 16.f);
}

void MineCounter::draw(sf::RenderWindow &window) {
    if (mineCount >= 0) {
        hundreds.setTexture(digitTextures[mineCount / 100]);
        tens.setTexture(digitTextures[(mineCount % 100) / 10]);
        ones.setTexture(digitTextures[mineCount % 10]);
    }
    else { // "---" for negative mines
        minesOver = std::abs(mineCount);
        hundreds.setTexture(digitTextures[minesOver / 100]);
        tens.setTexture(digitTextures[(minesOver % 100) / 10]);
        ones.setTexture(digitTextures[minesOver % 10]);
        window.draw(negative);
    }
    window.draw(hundreds);
    window.draw(tens);
    window.draw(ones);
}

void MineCounter::decrement() {
    --mineCount;
}

void MineCounter::increment() {
    ++mineCount;
}

void MineCounter::reset(int mines) {
    mineCount = mines;
}

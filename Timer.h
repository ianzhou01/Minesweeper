#pragma once
#include "Functions.h"
#include <SFML/Graphics.hpp>
#include <chrono>

struct Timer {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = clock::duration;

    time_point start;

    bool running, paused;
    duration pausedTime;
    // Sprites for digits
    sf::Texture digitTextures[11];
    sf::Sprite minTen, minOne, secTen, secOne;

    Timer(const sf::Texture &digits, int rows, int cols);
    duration getElapsedTime() const;
    int getElapsedSeconds() const;
    void reset();

    void startTimer();
    void pauseTimer();
    void resumeTimer();
    void stopTimer();
    void draw(sf::RenderWindow &window);

};
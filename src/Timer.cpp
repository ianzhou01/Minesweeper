#include "Timer.h"

Timer::Timer(const sf::Texture& digits, const int rows, const int cols) : running(false), paused(false) {
    sf::IntRect toCopy(0, 0, 21, 32);
    for (int i=0; i < 11; i++) {
        // Load each number (21 x 32) square into pre-existing textures zero, one...
        toCopy.left = i * 21;
        sf::Sprite digitSprite(digits);
        digitSprite.setTextureRect(toCopy);
        digitTextures[i].loadFromImage(digits.copyToImage(), toCopy);
    }
    // Initialize number slots
    minTen.setTexture(digitTextures[10]);
    minTen.setPosition(((float)cols*32.f) - 97.f, 32.f*((float)rows + 0.5f) + 16.f);
    minOne.setTexture(digitTextures[10]);
    minOne.setPosition(((float)cols*32.f) - 76.f, 32.f*((float)rows + 0.5f) + 16.f);
    secTen.setTexture(digitTextures[10]);
    secTen.setPosition(((float)cols*32.f) - 54.f, 32.f*((float)rows + 0.5f) + 16.f);
    secOne.setTexture(digitTextures[10]);
    secOne.setPosition(((float)cols*32.f) - 33.f, 32.f*((float)rows + 0.5f) + 16.f);
}

void Timer::startTimer() {
    if (!running) {
        start = clock::now();
        running = true;
    }
}

void Timer::stopTimer() {
    if (running) {
        running = false;
        paused = false;
        pausedTime = duration(0);
    }
}

void Timer::pauseTimer() {
    paused = true;
    pausedTime += clock::now() - start;
}

void Timer::resumeTimer() {
    if (running && paused) {
        start = clock::now(); // Resets count, adds back in getElapsedTime()
        paused = false;
    }
}

void Timer::reset() {
    running = false;
    paused = false;
    pausedTime = duration(0);
}

Timer::duration Timer::getElapsedTime() const {
    if (running) {
        if (paused) {
            return pausedTime;
        }
        else {
            return clock::now() - start + pausedTime; // As resume() resets time, add it back
        }
    }
    else {
        return duration(0);
    }
}

int Timer::getElapsedSeconds() const { // turn precision time into seconds
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(getElapsedTime()).count());
}

void Timer::draw(sf::RenderWindow &window) {
    int seconds = getElapsedSeconds();
    std::vector<int> minSec = secondsToMinSec(seconds);
    if (minSec[0] > 99) {// Timer upper bound for minutes (to avoid overflow)
        minSec[0] = 99;
        pauseTimer();
    }
    // Reset textures
    minTen.setTexture(digitTextures[minSec[0]]);
    minOne.setTexture(digitTextures[minSec[1]]);
    secTen.setTexture(digitTextures[minSec[2]]);
    secOne.setTexture(digitTextures[minSec[3]]);

    window.draw(minTen);
    window.draw(minOne);
    window.draw(secTen);
    window.draw(secOne);
}



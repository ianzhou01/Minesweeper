#pragma once
#include "Functions.h"
#include <SFML/Graphics.hpp>

struct WelcomeWindow {
    int width, height;

    WelcomeWindow(int, int);
    std::string operator()() const;
};
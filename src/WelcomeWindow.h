#pragma once
#include <string>

struct WelcomeWindow {
    int width, height;

    WelcomeWindow(int, int);
    std::string operator()() const;
};


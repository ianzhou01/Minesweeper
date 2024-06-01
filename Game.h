#pragma once
#include "Board.h"
#include "Button.h"

struct Game {
    int width, height, rows, cols, mines;
    std::string name;
    bool gameStarted;
    Game(int rows, int cols, int mines, std::string name);
    void operator()();
};


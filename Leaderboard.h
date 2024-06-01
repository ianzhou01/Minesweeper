#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include "Functions.h"
#pragma once

struct Leaderboard {
    vector<int> scores;
    vector<std::string> names;
    int rows, cols, recentPlacement;
    float width, height;
    bool recentlyUpdated;

    Leaderboard(int rows, int cols);

    int update(std::string &name, int time);
    void operator()();
    void refresh();
    void fileUpdate();
};
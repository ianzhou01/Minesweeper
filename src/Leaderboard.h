#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#pragma once

struct Leaderboard {
    std::vector<int> scores;
    std::vector<std::string> names;
    int rows, cols, recentPlacement;
    float width, height;
    bool recentlyUpdated;

    Leaderboard(int rows, int cols);

    int update(std::string &name, int time);
    void operator()();
    void refresh();
    void fileUpdate();
};
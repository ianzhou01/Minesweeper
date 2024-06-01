#pragma once
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Board.h"

using namespace std;

void centerTextOrigin(sf::Text &text);

void setText(sf::Text &text, float x, float y);

bool getConfig(const std::string &filename, int arr[]);

sf::Text renderText(const std::string& msg, const sf::Font& font, int size, sf::Color color, bool bold, bool underlined);

void setDims(int &rows, int &cols, int &width, int &height, int &leaderW, int &leaderH, int &mines, const int dims[]);

std::map<std::string, sf::Texture> loadTextures();

std::vector<int> secondsToMinSec(int seconds);

int timeToSeconds(const string &time);

std::string secondsToTime(int seconds);

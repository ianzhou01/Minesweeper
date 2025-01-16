#include "Functions.h"

void centerTextOrigin(sf::Text &text) {
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width/2.0f,
                   text.getLocalBounds().top + text.getLocalBounds().height/2.0f);
}

void setText(sf::Text &text, float x, float y){
    centerTextOrigin(text);
    text.setPosition(sf::Vector2f(x, y));
}

bool getConfig(const std::string &filename, int arr[]) {
    std::ifstream config(filename);
    if (!config)
        return false;
    std::string num;
    for (int i=0; i<=2; ++i) {
        std::getline(config, num);
        arr[i] = std::stoi(num);
    }
    return true;
}

sf::Text renderText(const std::string& msg, const sf::Font& font, int size, sf::Color color, bool bold, bool underlined) {
    sf::Text text(msg, font, size);
    if (bold)
        text.setStyle(sf::Text::Bold);
    if (underlined)
        text.setStyle(sf::Text::Underlined);
    text.setFillColor(color);
    return text;
}

void setDims(int &rows, int &cols, int &width, int &height, int &leaderW, int &leaderH, int &mines, const int dims[]) {
    cols = dims[0];
    rows = dims[1];
    width = dims[0] * 32;
    height = (dims[1] * 32) + 100;
    leaderW = dims[0] * 16;
    leaderH = (dims[1] * 16) + 50;
    mines = dims[2];
}

std::map<std::string, sf::Texture> loadTextures() {
    std::map<std::string, sf::Texture> textures;
    sf::Texture debug, digits, face_happy, face_lose, face_win, flag, leaderboard, mine,
            one, two, three, four, five, six, seven, eight,
            pause, play, tile_hidden, tile_revealed;
    if (!debug.loadFromFile("files/images/debug.png"))
        throw std::runtime_error("debug texture not found!");
    textures["debug"] = debug;

    if (!digits.loadFromFile("files/images/digits.png"))
        throw std::runtime_error("digits texture not found!");
    textures["digits"] = digits;

    if (!face_happy.loadFromFile("files/images/face_happy.png"))
        throw std::runtime_error("happy face texture not found!");
    textures["face_happy"] = face_happy;

    if (!face_lose.loadFromFile("files/images/face_lose.png"))
        throw std::runtime_error("sad face texture not found!");
    textures["face_lose"] = face_lose;

    if (!face_win.loadFromFile("files/images/face_win.png"))
        throw std::runtime_error("win face texture not found!");
    textures["face_win"] = face_win;

    if (!flag.loadFromFile("files/images/flag.png"))
        throw std::runtime_error("flag texture not found!");
    textures["flag"] = flag;

    if (!leaderboard.loadFromFile("files/images/leaderboard.png"))
        throw std::runtime_error("leaderboard texture not found!");
    textures["leaderboard"] = leaderboard;

    if (!mine.loadFromFile("files/images/mine.png"))
        throw std::runtime_error("mine texture not found!");
    textures["mine"] = mine;

    if (!one.loadFromFile("files/images/number_1.png"))
        throw std::runtime_error("one texture not found!");
    textures["one"] = one;

    if (!two.loadFromFile("files/images/number_2.png"))
        throw std::runtime_error("two texture not found!");
    textures["two"] = two;

    if (!three.loadFromFile("files/images/number_3.png"))
        throw std::runtime_error("three texture not found!");
    textures["three"] = three;

    if (!four.loadFromFile("files/images/number_4.png"))
        throw std::runtime_error("four texture not found!");
    textures["four"] = four;

    if (!five.loadFromFile("files/images/number_5.png"))
        throw std::runtime_error("five texture not found!");
    textures["five"] = five;

    if (!six.loadFromFile("files/images/number_6.png"))
        throw std::runtime_error("six texture not found!");
    textures["six"] = six;

    if (!seven.loadFromFile("files/images/number_7.png"))
        throw std::runtime_error("seven texture not found!");
    textures["seven"] = seven;

    if (!eight.loadFromFile("files/images/number_8.png"))
        throw std::runtime_error("eight texture not found!");
    textures["eight"] = eight;

    if (!pause.loadFromFile("files/images/pause.png"))
        throw std::runtime_error("pause texture not found!");
    textures["pause"] = pause;

    if (!play.loadFromFile("files/images/play.png"))
        throw std::runtime_error("play texture not found!");
    textures["play"] = play;

    if (!tile_hidden.loadFromFile("files/images/tile_hidden.png"))
        throw std::runtime_error("hidden tile texture not found!");
    textures["tile_hidden"] = tile_hidden;

    if (!tile_revealed.loadFromFile("files/images/tile_revealed.png"))
        throw std::runtime_error("revealed tile texture not found!");
    textures["tile_revealed"] = tile_revealed;

    return textures;
}

std::vector<int> secondsToMinSec(int seconds) {
    vector<int> time;
    int minutes = static_cast<int>(seconds / 60);
    seconds %= 60;
    time.push_back(minutes / 10);
    time.push_back(minutes % 10);
    time.push_back(seconds / 10);
    time.push_back(seconds % 10);
    return time;
}

int timeToSeconds(const string &time) {
    std::istringstream iss(time);
    std::string buffer;
    int min = 0, sec = 0;

    std::getline(iss, buffer, ':');
    if (!buffer.empty())
        min = std::stoi(buffer);
    std::getline(iss, buffer);
    if (!buffer.empty())
        sec = std::stoi(buffer);

    return min * 60 + sec;
}

std::string secondsToTime(int totalSeconds) {
    // Calculate minutes and seconds
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Convert minutes and seconds to strings
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}
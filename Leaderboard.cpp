#include "Leaderboard.h"

Leaderboard::Leaderboard(int rows, int cols) : rows(rows), cols(cols), width((float)cols * 16), height((float)(rows*16) + 50), recentlyUpdated(false), recentPlacement(-1) {
    int min, sec, time;
    std::string buffer;
    std::ifstream file("files/leaderboard.txt");
    if (!file)
        throw std::runtime_error("File not found!");
    while (std::getline(file, buffer, ',')) { // Load existing info into map
        time = timeToSeconds(buffer);
        std::getline(file, buffer);
        if (!buffer.empty() && buffer[0] == ' ')
            buffer.erase(0, 1); // Get rid of empty space
        scores.push_back(time);
        names.push_back(buffer);
    }
}

int Leaderboard::update(std::string &name, int time) {
    recentPlacement = 1;
    for (int i=0; i<scores.size(); i++) {
        if (time <= scores[i]) { // If time is tied or beaten, we take over
            recentlyUpdated = true;
            names.insert(names.begin() + recentPlacement-1, name);
            scores.insert(scores.begin() + recentPlacement-1, time);

            if (scores.size() != names.size()) {
                throw runtime_error("Leaderboard vector and map do not match!!!");
            }
            if (scores.size() > 5) { // Delete map entry if over 5 entries
                names.pop_back();
                scores.pop_back();
            }
            return recentPlacement;
        }
        recentPlacement++;
    }
    if (!recentlyUpdated && scores.size() < 5) { // Populate leaderboard if less than 5 entries
        scores.push_back(time);
        names.push_back(name);
        recentlyUpdated = true;
        recentPlacement = static_cast<int>(scores.size());
        return recentPlacement;
    }
    return -1;
}



void Leaderboard::operator()() {
    sf::RenderWindow leaderboard(sf::VideoMode((int)width, (int)height), "Leaderboard", sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        throw std::runtime_error("Font not found!");
    }


    std::string playerText;

    for (int i=0; i<scores.size(); ++i) {
        if (recentlyUpdated && i == recentPlacement - 1) {
            playerText += std::to_string(i+1) + ".\t" + secondsToTime(scores[i]) + '\t' + names[i] + "*\n\n";
        }
        else {
            playerText += std::to_string(i+1) + ".\t" + secondsToTime(scores[i]) + '\t' + names[i] + "\n\n";
        }
    }

    sf::Text leaderText = renderText("LEADERBOARD", font, 20, sf::Color::White, true, true);
    setText(leaderText, width / 2.0f, height / 2.0f - 120.f);
    sf::Text players = renderText(playerText, font, 18, sf::Color::White, true, false);
    setText(players, width / 2.0f, height / 2.0f + 20.f);

    sf::Event event;
    while (leaderboard.isOpen()) {
        while (leaderboard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboard.close();
            }
        }
        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(leaderText);
        leaderboard.draw(players);
        leaderboard.display();
    }
}

void Leaderboard::refresh() {
    recentlyUpdated = false;
}

void Leaderboard::fileUpdate() {
    std::ofstream file("files/leaderboard.txt", std::ios::trunc);
    std::string timeStr;
    for (int i=0; i<scores.size(); ++i) {
        timeStr = secondsToTime(scores[i]); // Converts int to MM:SS string format
        file << timeStr << ',' << names[i] << '\n';
    }
    file.close();
}



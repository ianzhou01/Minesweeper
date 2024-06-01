#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

struct Board {
    struct Tile {
        bool isMine, isFlagged, isRevealed;
        int adjacentMineCount;
        std::vector<Tile*> adjacentTiles;
        sf::Sprite sprite, sprite2, mineSprite;
        sf::Color originalColor, darkColor;

        void toggleFlag();
        Tile();
        void reveal(sf::Texture *nums, const sf::Texture& revealed);
        std::vector<bool> update(const sf::Vector2f &mousePos, const sf::Texture &flag, const sf::Texture &hidden, const sf::Texture &revealTexture, sf::Texture [], bool started);
        bool isMouseOver(const sf::Vector2f &mousePos) const;

    };

    Tile** board;
    int rows, cols, numMines, width, height;
    sf::Texture hidden, revealed, mine, flag;
    sf::Texture* numbers;
    bool debugMode, started;
    Tile* lastClicked;
    std::vector<int> emptyIndices;


    Board(int rows, int cols, int mines, int width, int height,
          const sf::Texture& hidden, const sf::Texture& revealed, const sf::Texture& flag, const sf::Texture& mine, sf::Texture []);
    ~Board();
    void draw(sf::RenderWindow &window, bool paused) const;
    void toggleDebug();
    void reset();
    std::vector<bool> updateBoard(const sf::Vector2f& mousePos);
    void initializeMines();
    void initializeAdjacentTiles() const;
    void pauseBoard() const;
    void resumeBoard() const;
    void updateNearbyMineCount() const;
    void handleFirstClickMine(int row, int col) const;
    void handleFirstClick(int row, int col, std::vector<int> &rand) const;
    void recursiveTileReveal(Tile* clickedTile);
    void revealAllMines() const;
    bool checkIfWon() const;
    void resetColor() const;
    void flagAllMines() const;
};

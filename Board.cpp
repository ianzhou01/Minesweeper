#include "Board.h"

// Tile functions
Board::Tile::Tile() : adjacentMineCount(0), isMine(false), isFlagged(false), isRevealed(false) {}

void Board::Tile::toggleFlag() {
    isFlagged = !isFlagged;
}

// Helper function for update
bool Board::Tile::isMouseOver(const sf::Vector2f &mousePos) const {
    return sprite.getGlobalBounds().contains(mousePos); // Check if mouse is over button
}

void Board::Tile::reveal(sf::Texture nums[], const sf::Texture& revealedText) {
    sprite2.setColor(darkColor);
    if (!isMine) { // Numbers only display for safe tiles
        if (adjacentMineCount > 0) {
            sprite2.setTexture(nums[adjacentMineCount - 1]); // Display number on top layer
            sprite.setTexture(revealedText); // Bottom layer is now revealed texture
        }
        else
            sprite2.setTexture(revealedText); // Empty tile; only need top layer to be "revealed"
    }
    isRevealed = true;
}

std::vector<bool> Board::Tile::update(const sf::Vector2f &mousePos, const sf::Texture &flg, const sf::Texture &hid, const sf::Texture &revealTexture,
                                      sf::Texture nums[], bool gameStarted) {
    if (isMouseOver(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!isFlagged && !isRevealed) { // Left-click only works on non-flagged and hidden tiles
            if (!gameStarted) { // Handle case for first left-click being a mine
                return {true, false, true}; // First-click
            }
            return {true, false, false}; // The regular Tile reveal
        }
        else
            return {false, false, false};
    }
    else if (isMouseOver(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Right) && !isRevealed) {
        // Right-click only works on hidden tiles
        sprite2.setColor(darkColor);
        toggleFlag();
        if (isFlagged) {
            sprite2.setTexture(flg);
        }
        else {
            sprite2.setTexture(hid);
        }
        return {true, true, false}; // Updated AND right-clicked
    }
    else {
        sprite2.setColor(originalColor);
        return {false, false, false}; // Update failed/not recentlyUpdated
    }
}



// Board functions
Board::Board(int rows, int cols, int mines, int width, int height,
             const sf::Texture& hidden, const sf::Texture& revealed, const sf::Texture& flag, const sf::Texture& mine,
             sf::Texture numbers[]) :
            rows(rows), cols(cols), numMines(mines), width(width), height(height), hidden(hidden), revealed(revealed),
            flag(flag), mine(mine), debugMode(false), lastClicked(nullptr), started(false) {
    this->numbers = numbers;
    board = new Tile*[rows];
    for (int i=0; i<rows; ++i) {
        board[i] = new Tile[cols];
        for (int j=0; j<cols; j++) {
            board[i][j].sprite.setPosition(32.f * (float)j, 32.f * (float)i); // Order tiles to print horizontal first
            board[i][j].sprite.setTexture(hidden);
            board[i][j].sprite2.setPosition(32.f * (float)j, 32.f * (float)i); // Order tiles to print horizontal first
            board[i][j].sprite2.setTexture(hidden);
            board[i][j].mineSprite.setPosition(32.f * (float)j, 32.f * (float)i); // Order tiles to print horizontal first
            board[i][j].mineSprite.setTexture(mine);

            // Set darker click feature
            board[i][j].originalColor = board[i][j].sprite2.getColor();
            board[i][j].darkColor = board[i][j].originalColor;
            board[i][j].darkColor.r /= 2;
            board[i][j].darkColor.g /= 2;
            board[i][j].darkColor.b /= 2;
        }
    }
}

Board::~Board() {
    for (int i=0; i<rows; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

void Board::draw(sf::RenderWindow &window, bool paused) const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            window.draw(board[i][j].sprite);
            if (paused && started) { // Display paused mode sprite2
                window.draw(board[i][j].sprite2);
            }
            else {
                if (debugMode && board[i][j].isMine) {
                    // If debug mode is enabled and the tile is a mine, draw the mine texture
                    window.draw(board[i][j].mineSprite);
                }
                else {
                    // Otherwise, draw the regular tile sprite
                    window.draw(board[i][j].sprite2);
                }
            }
        }
    }
}

void Board::reset() {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            board[i][j].sprite.setTexture(hidden);
            board[i][j].sprite2.setTexture(hidden);
            board[i][j].isRevealed = board[i][j].isFlagged = board[i][j].isMine = false; // set both booleans to false
            board[i][j].adjacentMineCount = 0; // Reset mine count -- BOARD CONSTRUCTION DEPENDENT BEHAVIOR
        }
    }
    debugMode = false;
}

std::vector<bool> Board::updateBoard(const sf::Vector2f& mousePos) { // Second item: flag ON/OFF
    std::vector<bool> tileUpdateConditions;
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            tileUpdateConditions = board[i][j].update(mousePos, flag, hidden, revealed, numbers, started);
            if (tileUpdateConditions[0]) { // Tile clicked
                if (tileUpdateConditions[1]) {// Tile right-clicked
                    if (board[i][j].isFlagged)
                        return {true, true, true}; // Tile right-clicked and flagged (decrement mine counter)
                    else
                        return {true, true, false}; // Tile right-clicked and un-flagged (increment mine counter)
                }
                else { // Left-click: (true, false, ...)
                    lastClicked = &board[i][j];
                    if (!tileUpdateConditions[2]) { // Regular Tile reveal: (true, false, false)
                        recursiveTileReveal(lastClicked);
                        return {true, false, false};
                    }
                    else { // First click: (true, false, true)
                        handleFirstClickMine(i, j);
                        handleFirstClick(i, j, emptyIndices);
                        recursiveTileReveal(lastClicked);
                        board[i][j].reveal(numbers, revealed); // Still need to reveal first tile!
                        return {true, false, true}; // FIRST CLICK MINE INCIDENT
                    }

                }
            }
        }
    }
    return {false, false, false}; // No tile was clicked, no tile right-clicked, third entry null
}

void Board::initializeMines() { // Remember to initialize adjacentMine vector and adjacentMineCount
    if (rows * cols < numMines + 1)
        throw std::runtime_error("Cannot initialize with equal # of mines as tiles.");
    std::mt19937 mt(time(0));
    std::uniform_int_distribution<int> dist(1, rows * cols - 1); // leave out top-left corner for potential relocation
    int random;
    std::vector<int> randomIndices;
    std::vector<int> emptyIndices;
    // Populate "Taboo" Indices
    for (int i=0; i<8; ++i) { // At least 8 empty indices to ensure first click is island
        random = dist(mt);
        while (std::count(emptyIndices.begin(), emptyIndices.end(), random) > 0) {
            random = dist(mt); // If a duplicate of index exists, try again
        }
        emptyIndices.push_back(random);
    }

    for (int i=0; i<numMines; ++i) { // Initialize random mine locations
        random = dist(mt);
        while (std::count(randomIndices.begin(), randomIndices.end(), random) > 0 || std::count(emptyIndices.begin(), emptyIndices.end(), random) > 0) {
            random = dist(mt); // If a duplicate of index exists or if taboo, try again
        }
        randomIndices.push_back(random);
    }
    for (int index : randomIndices) { // Populate mines
        int row = index / cols;
        int col = index % cols;
        board[row][col].isMine = true;
    }
    this->emptyIndices = emptyIndices;
}

void Board::pauseBoard() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            board[i][j].sprite2.setTexture(revealed);
        }
    }
}


void Board::resumeBoard() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            if (board[i][j].isFlagged)
                board[i][j].sprite2.setTexture(flag); // Re-display flag
            else if (board[i][j].isRevealed) { // Re-display number or revealed tile accordingly
                if (board[i][j].adjacentMineCount > 0)
                    board[i][j].sprite2.setTexture(numbers[board[i][j].adjacentMineCount - 1]);
                else
                    board[i][j].sprite2.setTexture(revealed);
            }
            else {
                board[i][j].sprite2.setTexture(hidden); // Return to hidden state
            }
        }
    }
}

void Board::toggleDebug() {
    debugMode = !debugMode;
}

void Board::initializeAdjacentTiles() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::vector<Tile*> adj;
            // Add adjacent tiles to vector
            if (i > 0) {
                adj.push_back(&board[i - 1][j]); // Top tile
                if (j > 0)
                    adj.push_back(&board[i - 1][j - 1]); // Top left tile
                if (j < cols - 1)
                    adj.push_back(&board[i - 1][j + 1]); // Top right tile
            }
            if (i < rows - 1) {
                adj.push_back(&board[i + 1][j]); // Bottom tile
                if (j > 0)
                    adj.push_back(&board[i + 1][j - 1]); // Bottom left tile
                if (j < cols - 1)
                    adj.push_back(&board[i + 1][j + 1]); // Bottom right tile
            }
            if (j > 0) {
                adj.push_back(&board[i][j - 1]); // Left tile
            }
            if (j < cols - 1) {
                adj.push_back(&board[i][j + 1]); // Right tile
            }
            board[i][j].adjacentTiles = adj; // Initialize the tile with the adjacent tiles vector
        }
    }
}

void Board::updateNearbyMineCount() const {
    int counter;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            counter = 0;
            for (auto Tile : board[i][j].adjacentTiles) {
                if (Tile->isMine)
                    ++counter;
            }
            board[i][j].adjacentMineCount = counter;
        }
    }
}

void Board::handleFirstClickMine(int row, int col) const {
    if (board[row][col].isMine) {
        std::swap(board[0][0].isMine, board[row][col].isMine);
        updateNearbyMineCount();
    }
}



void Board::recursiveTileReveal(Tile* clickedTile) {
    // If the clicked tile is already revealed or flagged, return
    if (clickedTile->isRevealed || clickedTile->isFlagged || clickedTile->isMine) {
        return;
    }
    // Reveal clicked tile
    clickedTile->reveal(numbers, revealed);
    // If clicked tile is a numbered tile, stop recursion
    if (clickedTile->adjacentMineCount > 0) {
        return;
    }
    // Recursively reveal adjacent tiles
    for (auto adjacentTile : clickedTile->adjacentTiles) {
        recursiveTileReveal(adjacentTile);
    }
}

void Board::revealAllMines() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            if (board[i][j].isMine)
                board[i][j].sprite2.setTexture(mine);
        }
    }
}

bool Board::checkIfWon() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            if (!board[i][j].isMine && !board[i][j].isRevealed) {
                return false;
            }
        }
    }
    // If all non-mine tiles revealed, game won

    return true;
}

void Board::resetColor() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            board[i][j].sprite2.setColor(board[i][j].originalColor);
            board[i][j].sprite.setColor(board[i][j].originalColor);
        }
    }
}

void Board::flagAllMines() const {
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            if (board[i][j].isMine)
                board[i][j].sprite2.setTexture(flag);
        }
    }
}

void Board::handleFirstClick(int row, int col, std::vector<int> &rand) const {
    int randrow, randcol, curr = 0;
    for (auto & adjacentTile : board[row][col].adjacentTiles) {
        if (adjacentTile->isMine) { // Swap the mine with the next rand index
            // Convert rand index -> 2-D coords
            randrow = rand[curr] / cols;
            randcol = rand[curr] % cols;
            std::swap(board[randrow][randcol].isMine, adjacentTile->isMine);
            ++curr;
        }
    }
    updateNearbyMineCount(); // update mine counts after all mines are swapped
}


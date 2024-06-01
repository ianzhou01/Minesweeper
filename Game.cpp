#include "Game.h"
#include "Leaderboard.h"
#include <utility>
#include "Functions.h"
#include "Timer.h"
#include "MineCounter.h"

Game::Game(int rows, int cols, int mines, std::string name) : rows(rows), cols(cols), mines(mines), name(std::move(name)) {
    width = cols * 32;
    height = rows * 32 + 100;
    gameStarted = false;
}

void Game::operator()() {
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close | sf::Style::Resize);
    // Activate OpenGL Context before loading textures (NECESSARY)
    gameWindow.setActive(true);
    // Load textures
    std::map<std::string, sf::Texture> textures = loadTextures();
    // Create board
    sf::Texture numbers[8] = {textures["one"], textures["two"], textures["three"], textures["four"],
                              textures["five"], textures["six"], textures["seven"], textures["eight"]};
    Board board(rows, cols, mines, width, height, textures["tile_hidden"], textures["tile_revealed"], textures["flag"], textures["mine"], numbers);
    board.reset();
    board.initializeMines();
    board.initializeAdjacentTiles(); // Must do after mine initialization, as this func counts mines
    board.updateNearbyMineCount();
    // Load Buttons
    Button Face(((float)cols/2.0f) * 32.f - 32.f, 32.f * ((float)rows + 0.5f), textures["face_happy"]);
    Button Debug(((float)cols) * 32.f - 304.f, 32.f * ((float)rows + 0.5f), textures["debug"]);
    Button Pause(((float)cols) * 32.f - 240.f, 32.f * ((float)rows + 0.5f), textures["pause"]);
    Button Play(((float)cols) * 32.f - 240.f, 32.f * ((float)rows + 0.5f), textures["play"]);
    Button LeaderboardButton(((float)cols) * 32.f - 176.f, 32.f * ((float)rows + 0.5f), textures["leaderboard"]);
    Timer timer(textures["digits"], rows, cols);
    MineCounter mineCounter(mines, textures["digits"], rows, cols);
    Leaderboard leaderboard(rows, cols);

    // Mouse tracker
    sf::Vector2f mousePosition;
    bool paused = true, leaderboardpause = false, lost = false, won = false, ended = false;
    std::vector<bool> boardUpdates;

    while (gameWindow.isOpen()) { // Main loop
        sf::Event event{};
        while (gameWindow.pollEvent(event)) { // Waits for an event to happen, then breaks automatically
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            // -----Update all buttons-----
            mousePosition = gameWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            Face.update(mousePosition);

            Debug.update(mousePosition);
            if (Debug.clicked && !lost && !won && (!paused || !gameStarted)) {
                board.toggleDebug();
            }

            LeaderboardButton.update(mousePosition);
            if (gameStarted && !lost && !won) { // Pause only works after the FIRST CLICK
                Play.update(mousePosition); // Still needed for blinking effect
                Pause.update(mousePosition);
                if (Pause.clicked) { // Accounts for Play button, same position
                    paused = !paused;
                    // Pause and resume actions
                    if (paused) {
                        timer.pauseTimer();
                        board.pauseBoard();
                    }
                    if (!paused) {
                        timer.resumeTimer();
                        board.resumeBoard();
                    }
                }
            }


            // -----Buttons recentlyUpdated-----

            // Update board if playing OR if first click
            if (!paused && !lost && !won) {// Only update board if playing and not lost
                boardUpdates = board.updateBoard(mousePosition); // Action: update board
                if (boardUpdates[0]) { // A board update was made!
                    if (boardUpdates[1]) { // Right-click update! {
                        if (boardUpdates[2])
                            mineCounter.decrement(); // Flag was toggled on
                        else
                            mineCounter.increment(); // Flag was toggled off
                    }
                    else { // Left-click update!
                        won = board.checkIfWon();
                        if (won) { // Game won!!!!!
                            ended = true;
                            board.resetColor();
                            board.flagAllMines();
                            timer.pauseTimer();
                            paused = true;
                            Face.sprite.setTexture(textures["face_win"]);
                            if (leaderboard.update(name, timer.getElapsedSeconds()) != -1) // -1 if failed
                                leaderboard.fileUpdate();
                        }
                        if (board.lastClicked->isMine) { // Game lost!
                            board.revealAllMines();
                            lost = true;
                            timer.pauseTimer();
                            paused = true;
                            Face.sprite.setTexture(textures["face_lose"]);
                        }
                    }
                }
            }
            if (!gameStarted) { // FIRST CLICK ON THE BOARD (MUST BE LEFT CLICK) MUST ALSO UPDATE BOARD
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && board.updateBoard(mousePosition)[0]) {
                    gameStarted = true;
                    board.started = true;
                    paused = false;
                    timer.reset();
                    timer.startTimer();
                    // Check if won off first click
                    won = board.checkIfWon();
                    if (won) { // Game won!!!!!
                        board.resetColor();
                        board.flagAllMines();
                        timer.pauseTimer();
                        paused = true;
                        Face.sprite.setTexture(textures["face_win"]);
                        if (leaderboard.update(name, timer.getElapsedSeconds()) > 0) // -1 if failed
                            leaderboard.fileUpdate(); // Updates file to save leaderboard
                        ended = true;
                    }
                }
            }
        }
        if (Face.clicked) { // Reset whole game
            Face.sprite.setTexture(textures["face_happy"]);
            board.reset();
            board.initializeMines();
            board.updateNearbyMineCount();
            timer.stopTimer();
            timer.reset(); // Must reset timer to zero
            mineCounter.reset(mines);
            leaderboard.refresh(); // Leaderboard is reset without *
            paused = true;
            ended = won = lost = leaderboardpause = board.started = gameStarted = false;
        }




        // Update game window
        gameWindow.clear(sf::Color::White);
        // Draw buttons
        Face.draw(gameWindow);
        Debug.draw(gameWindow);
        if (paused)
            Play.draw(gameWindow);
        else
            Pause.draw(gameWindow);
        LeaderboardButton.draw(gameWindow);
        // Draw timer/counter
        timer.draw(gameWindow);
        mineCounter.draw(gameWindow);
        
        // Draw board default
        if (!LeaderboardButton.clicked) {
            board.draw(gameWindow, paused);
            gameWindow.display();
        }
        // Draw board with leaderboard toggle
        else { // Handle board updates for leaderboard differently
            if (!lost && !won) {
                if (gameStarted) { // If game started, behaves like pause/play button
                    // Pause actions (toggling leaderboard shouldn't resume game)
                    if (!paused) { // Pause while going
                        timer.pauseTimer();
                        paused = true;
                        board.pauseBoard();
                        board.draw(gameWindow, paused);
                        gameWindow.display();

                        leaderboard();

                        paused = false;
                        board.resumeBoard();
                        timer.resumeTimer();
                    }
                    else {
                        board.draw(gameWindow, paused);
                        gameWindow.display();
                        leaderboard();
                    }
                }
                else { // LeaderboardButton must also pause board if game hasn't started yet
                    board.pauseBoard();
                    board.draw(gameWindow, paused); // paused == true if !gameStarted
                    gameWindow.display();
                    leaderboard();
                    board.resumeBoard();
                }
            }
            else { // Either lost or won; game has already ended
                board.draw(gameWindow, paused); // paused must be true
                gameWindow.display();
                leaderboard();
                if (lost)
                    board.revealAllMines(); // Go back to displaying mines if lost
            }
        }
        if (ended) {
            leaderboard();
            ended = false;
            continue;
        }
    }
}


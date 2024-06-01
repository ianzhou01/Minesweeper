#include <iostream>
#include "Functions.h"
#include "WelcomeWindow.h"
#include "Game.h"
/*  ----------INFO----------
 *  Programmer: Ian Zhou
 *  Latest stable version date: May 27, 2024
 *  Notes: 4/20: All aspects functional
 *         5/27: Added first-click adjacent tile clearing: all first clicks now create islands.
 */

int main() {
    // Load dimensions from config into buffer
    int* dims = new int[3];
    if (!getConfig("files/config.cfg", dims))
        throw runtime_error("Configuration not found!");
    // Set loaded dimensions to variables
    int rows, cols, width, height, leaderBoardWidth, leaderBoardHeight, totalMines;
    setDims(rows, cols, width, height, leaderBoardWidth, leaderBoardHeight, totalMines, dims);
    // Free dimension buffer array
    delete[] dims;

    // WELCOME WINDOW START //
    WelcomeWindow welcome(width, height);
    string username = welcome(); // Get name
    if (username == "-1") // Welcome window was force closed; don't continue
        return 1;

    // GAME WINDOW START //
    Game game(rows, cols, totalMines, username);
    game();

    return 0;
}

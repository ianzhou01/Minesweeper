Name: Ian Zhou
UFL email: ian.zhou@ufl.edu
System: Windows 11, 64-bit operating system, x64-based processor
Compiler: MinGW Builds 7.3.0 (64-bit)
SFML Version: 2.5.1 (GCC 7.3.0 MinGW (SEH) - 64-bit)
IDE: CLion
Notes: My Minesweeper project is modeled after conventional rules implemented in
minesweeperonline.com and the Google minesweeper game, in which the first
click is always safe. In addition, the first click clears the 3x3 square around it,
ensuring each game starts with an "island." In implementing this, I initialize the 
top left tile to be empty. If the user clicks on a mine, it is relocated to the top left tile. This way, 
Randomness is preserved; as the user cannot know which tile is a mine, 
the top left tile remains unpredictable.
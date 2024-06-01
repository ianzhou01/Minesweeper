cmake_minimum_required(VERSION 3.24)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Minesweeper main.cpp
        Board.cpp
        Button.cpp
        Game.cpp
        Functions.cpp
        Timer.cpp
        MineCounter.cpp
        WelcomeWindow.cpp
        Leaderboard.cpp
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML/include/SFML)
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio)


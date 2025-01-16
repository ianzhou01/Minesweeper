#include "WelcomeWindow.h"

WelcomeWindow::WelcomeWindow(int width, int height) : width(width), height(height) {}

std::string WelcomeWindow::operator()() const {
    // Create window
    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    // Initialize font
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        throw std::runtime_error("Font not found!");
    }
    // Create all welcome text
    sf::Text welcome1 = renderText("WELCOME TO MINESWEEPER!", font, 24, sf::Color::White, true, true);
    setText(welcome1, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f - 150.f);

    sf::Text welcome2 = renderText("Enter your name:", font, 20, sf::Color::White, true, false);
    setText(welcome2, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f - 75.f);

    sf::Text name = renderText("", font, 18, sf::Color::Yellow, true, false);
    setText(name, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f - 45.f);

    sf::Text instructions = renderText("Press [ENTER] to play.", font, 20, sf::Color::White, true, false);
    setText(instructions, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f + 10.f);

    sf::Text cursor = renderText("|", font, 18, sf::Color::White, false, false);

    // Cursor blinking variables
    bool cursorVisible = true;
    sf::Clock cursorClock;
    // Name buffer (for display text and return value)
    std::string inputString;

    // Display welcome window and get name
    while (welcomeWindow.isOpen()) {
        sf::Event event{};
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return "-1";
            }
                // Handle name entering
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8
                    || (event.text.unicode < 123 && event.text.unicode > 96)
                    || (event.text.unicode < 91 && event.text.unicode > 64)) { // Valid characters for name
                    if (event.text.unicode == 8) { // Handle backspace
                        if (!inputString.empty())
                            inputString.pop_back(); // Delete character
                    }
                    else { // Enter the name
                        if (inputString.length() < 10) {
                            if (inputString.empty())
                                inputString += toupper(static_cast<char>(event.text.unicode)); // Map UTF-32 unicode to character
                            else
                                inputString += tolower(static_cast<char>(event.text.unicode));
                        }
                    }
                    name.setString(inputString);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) &&
                            !inputString.empty()) { // Exit welcome window if some name exists and Enter pressed
                    welcomeWindow.close();
                }
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcome1);
        welcomeWindow.draw(welcome2);
        name.setOrigin(name.getLocalBounds().left + name.getLocalBounds().width / 2.0f,
                       name.getLocalBounds().top + name.getLocalBounds().height / 2.0f - 12.5f);
        welcomeWindow.draw(name);
        if (!inputString.empty())
            welcomeWindow.draw(instructions);
        // Blinking effect
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }
        if (cursorVisible) {
            cursor.setPosition(name.getPosition().x + (name.getLocalBounds().width) / 2,
                               static_cast<float>(height) / 2.0f - 45);
            welcomeWindow.draw(cursor);
        }
        // Update welcome window w/ cursor and name
        welcomeWindow.display();
    }
    return inputString;
}


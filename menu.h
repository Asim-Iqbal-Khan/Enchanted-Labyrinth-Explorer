#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"

using namespace std;

enum class MenuState {
    StartGame,
    Instructions,
    Exit
};

int menu() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Main Menu");
    window.setFramerateLimit(60);

    // Create menu items
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font file" << endl;
        return 1;
    }

    sf::Text startText("Start Game", font, 30);
    startText.setPosition(300, 200);
    startText.setFillColor(sf::Color::White);

    sf::Text instructionsText("Instructions", font, 30);
    instructionsText.setPosition(300, 250);
    instructionsText.setFillColor(sf::Color::White);

    sf::Text exitText("Exit", font, 30);
    exitText.setPosition(300, 300);
    exitText.setFillColor(sf::Color::White);

    // Load image
    sf::Texture imageTexture;
    if (!imageTexture.loadFromFile("img/menu1.png")) {
        cerr << "Error loading image file" << endl;
        return 1;
    }

    sf::Sprite imageSprite(imageTexture);
    imageSprite.setPosition(110, 10);

    // Load instructions image
    sf::Texture instructionsImageTexture;
    if (!instructionsImageTexture.loadFromFile("img/instructions.png")) {
        cerr << "Error loading instructions image file" << endl;
        return 1;
    }

    sf::Sprite instructionsImageSprite(instructionsImageTexture);
    instructionsImageSprite.setPosition(-300, -470);

    MenuState currentState = MenuState::StartGame;
    bool showInstructions = false;
    bool isPaused = false; // Flag to indicate whether the game is paused

    // Load music
    sf::Music menuMusic;
    if (!menuMusic.openFromFile("music/menu_music.mp3")) {
        cerr << "Error loading menu music" << endl;
        return 1;
    }

    // Play the menu music
    menuMusic.play();
    menuMusic.setLoop(true); // Loop the music

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (!showInstructions && !isPaused) { // Check if not showing instructions and game is not paused
                    if (event.key.code == sf::Keyboard::Up) {
                        // Move selection up
                        if (currentState == MenuState::StartGame)
                            currentState = MenuState::Exit;
                        else
                            currentState = static_cast<MenuState>(static_cast<int>(currentState) - 1);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        // Move selection down
                        if (currentState == MenuState::Exit)
                            currentState = MenuState::StartGame;
                        else
                            currentState = static_cast<MenuState>(static_cast<int>(currentState) + 1);
                    }
                    else if (event.key.code == sf::Keyboard::Return) {
                        // Execute selected menu item
                        switch (currentState) {
                        case MenuState::StartGame:
                            // Start game
                        {
                            menuMusic.stop();
                            Game g;
                            g.start_game();
                        }
                        break;
                        case MenuState::Instructions:
                            // Toggle showing instructions
                            showInstructions = true;
                            break;
                        case MenuState::Exit:
                            // Exit program
                            window.close();
                            break;
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    showInstructions = false;
                }
                else if (event.key.code == sf::Keyboard::P) {
                    // Toggle pause state
                    isPaused = !isPaused;
                }
            }
        }

        // Highlight the currently selected item
        switch (currentState) {
        case MenuState::StartGame:
            startText.setFillColor(sf::Color::Red);
            instructionsText.setFillColor(sf::Color::White);
            exitText.setFillColor(sf::Color::White);
            break;
        case MenuState::Instructions:
            startText.setFillColor(sf::Color::White);
            instructionsText.setFillColor(sf::Color::Red);
            exitText.setFillColor(sf::Color::White);
            break;
        case MenuState::Exit:
            startText.setFillColor(sf::Color::White);
            instructionsText.setFillColor(sf::Color::White);
            exitText.setFillColor(sf::Color::Red);
            break;
        }

        window.clear(sf::Color::Black);
        window.draw(imageSprite); // Draw the image
        window.draw(startText);
        window.draw(instructionsText);
        window.draw(exitText);

        // Draw instructions image if needed
        if (showInstructions) {
            window.draw(instructionsImageSprite);
        }

        window.display();
    }

    // Stop and release resources when exiting
    menuMusic.stop();
    return 0;
}

#pragma once
#include "boidUI.h"
#include <iostream>

boidUI::boidUI(sf::RenderWindow& window, boidManager& manager) : m_Window(window), m_BoidManager(manager) {
    if (!m_Font.loadFromFile("assets/arial.ttf")) {
        std::cout << "Failed to load font" << std::endl;
    }
    createButtons();
}

// Main run function for the UI, called repeatedly to update the UI
void boidUI::run() {
    // Handle any user input events
    handleEvents();
    // Clear the window with a white background
    m_Window.clear(sf::Color::White);
    // Draw all buttons and their corresponding texts
    for (size_t i = 0; i < m_Buttons.size(); ++i) {
        m_Window.draw(m_Buttons[i]);
        m_Window.draw(m_ButtonTexts[i]);
    }
    // Display the updated contents of the window
    m_Window.display();
}

// Print the selected boid type
void boidUI::handleEvents() {
    sf::Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_Window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_Window);
                for (size_t i = 0; i < m_Buttons.size(); ++i) {
                    if (m_Buttons[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        m_BoidManager.m_selectedType = static_cast<boidType>(i);
                        // Print the selected boid type
                        switch (m_BoidManager.m_selectedType) {

                        case boidType::FLOCKING:
                            std::cout << "Selected Type: Flocking" << std::endl;
                            break;
                        case boidType::CLUMP:
                            std::cout << "Selected Type: Clump" << std::endl;
                            break;
                        case boidType::WANDER:
                            std::cout << "Selected Type: Wander" << std::endl;
                            break;
                        case boidType::PURSUE:
                            std::cout << "Selected Type: Pursue" << std::endl;
                            break;
                        case boidType::ARRIVAL:
                            std::cout << "Selected Type: Arrival" << std::endl;
                            break;
                        case boidType::SEEK:
                            std::cout << "Selected Type: Seek" << std::endl;
                            break;   
                        case boidType::QUEUE:
                            std::cout << "Selected Type: Queue" << std::endl;
                            break;
                        case boidType::NONNORMALIZEDFLOCKING:
                            std::cout << "Selected Type: Non Normalized Flocking" << std::endl;
                            break;
                        default:
                            std::cout << "Unknown Type" << std::endl;
                            break;
                        }

                        break;
                    }
                }
            }
        }
    }
}
// Function to create buttons for the UI
void boidUI::createButtons() {
    const int buttonWidth = 180;
    const int buttonHeight = 30;
    const int buttonPadding = 10;

    // Labels for each button corresponding to a boid type
    std::vector<std::string> buttonLabels = {
        "Flocking", "Clump", "Wander", "Pursue", "Arrival", "Seek", "Queue" , "NNFlocking"
    };

    // Create and position each button and its text
    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(10, i * (buttonHeight + buttonPadding) + 10);
        button.setFillColor(sf::Color::Green);
        m_Buttons.push_back(button);

        sf::Text buttonText(buttonLabels[i], m_Font, 16);
        buttonText.setPosition(button.getPosition() + sf::Vector2f(10, 5));
        buttonText.setFillColor(sf::Color::Black);
        m_ButtonTexts.push_back(buttonText);
    }
}
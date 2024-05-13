#pragma once
#include <SFML/Graphics.hpp>
#include "boidManager.h"

class boidUI {
public:
    // Constructor for boidUI, takes references to an SFML RenderWindow and a boidManager.
    boidUI(sf::RenderWindow& window, boidManager& manager);

    // Public method to start the UI interaction loop.
    void run();

private:
    // Private method to handle user input events.
    void handleEvents();
    // Private method to create buttons on the UI.
    void createButtons();

    // Reference to the SFML RenderWindow object where the UI will be drawn.
    sf::RenderWindow& m_Window;
    // Reference to the boidManager object to interact with the boid simulation.
    boidManager& m_BoidManager;
    // Vector of RectangleShape objects to represent buttons in the UI.
    std::vector<sf::RectangleShape> m_Buttons;
    // Vector of Text objects for labeling the buttons.
    std::vector<sf::Text> m_ButtonTexts;
    // Font object to style the text displayed in the UI.
    sf::Font m_Font;
};
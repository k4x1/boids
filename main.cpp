// Include necessary headers
#include <SFML/Graphics.hpp>
#include <iostream>
#include "boid.h"
#include "boidManager.h"
#include <vector>
#include <future>
#include <ctime>
#include "boidUI.h"

int main()
{
    // Create two SFML RenderWindows, one for the main display and one for the UI
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Window");
    sf::RenderWindow uiWindow(sf::VideoMode(200, 350), "Boid UI");

    // Create instances of boidManager and boidUI, passing references where needed
    boidManager boidManager(&window);
    boidUI boidUI(uiWindow, boidManager);

    // Main loop that continues until either window is closed
    while (window.isOpen() && uiWindow.isOpen())
    {
        sf::Event event;

        // Event polling loop for the main window
        while (window.pollEvent(event))
        {
            // Close the window if the close event is triggered
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Check for mouse button presses
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                // Create a boid at the mouse position if the left button is pressed
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    boidManager.createBoid(mousePosition);
                }

                // Add an obstacle at the mouse position if the right button is pressed
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    boidManager.addObstacle(mousePosition, 100.0f);
                }
            }
        }

        // Clear the main window with a light grey color
        window.clear(sf::Color(200, 200, 200));
        // Update the boidManager which handles all boid behavior and rendering
        boidManager.update();
        // Display the updated contents of the main window
        window.display();

        // Run the UI update loop
        boidUI.run();
    }

    return 0;
}

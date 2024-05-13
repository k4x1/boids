#pragma once
#include <SFML/Graphics.hpp>

// Defines an Obstacle in the game world, represented as a circle.
struct Obstacle {
    sf::Vector2f m_Pos;       // Position of the obstacle
    float m_Radius;           // Radius of the obstacle
    sf::CircleShape m_Shape;  // Graphical representation of the obstacle

    Obstacle(sf::Vector2f pos, float radius); // Constructor
};

#include "obstacle.h"

// Constructor for Obstacle, initializes and sets up the circle shape.
Obstacle::Obstacle(sf::Vector2f pos, float radius) : m_Pos(pos), m_Radius(radius) {
    m_Shape.setRadius(m_Radius);
    m_Shape.setPosition(m_Pos);
    m_Shape.setOrigin(m_Radius, m_Radius); // Set origin to center for proper positioning
    m_Shape.setFillColor(sf::Color::Black); // Default color
}

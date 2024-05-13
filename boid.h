#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include "Obstacle.h"

// Class representing a boid, which is an autonomous agent with various behaviors.
class boid {
    sf::Vector2f m_Size;          // Size of the boid
    float m_Speed;                // Speed of the boid
    sf::Vector2f m_Direction;     // Current moving direction of the boid
    float m_Rot;                  // Rotation angle of the boid
    float m_WanderAngle = 5.0f;   // Angle used in the wandering behavior

public:
    sf::Vector2f m_Pos;           // Position of the boid
    sf::Sprite m_Sprite;          // Graphical representation of the boid

    boid(sf::Texture* _tex);       // Constructor
    ~boid();                      // Destructor
    void Move();                  // Method to update boid's position based on its direction
    void Flocking(std::vector<boid>& flock); // Flocking behavior method
    void Seek(const sf::Vector2f& target); // Seek behavior method
    void Pursue(sf::Vector2f targetPos, sf::Vector2f targetVelocity); // Pursue behavior method
    void Wander(); // Wander behavior method
    void Arrival(const sf::Vector2f& target, float slowingDistance); // Arrival behavior method
    void Clump(const std::vector<boid>& flock); // Clumping behavior method
    void ObjectAvoidance(const std::vector<Obstacle>& obstacles); // Obstacle avoidance method
    void nonNormalizedFlocking(std::vector<boid>& flock); // Alternative flocking method
    void Queue(const std::vector<boid>& flock, const sf::Vector2f& target); // Queueing behavior method
};

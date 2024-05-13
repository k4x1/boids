#pragma once
#include "boid.h"
#include <vector>
#include "obstacle.h"

enum boidType {
	FLOCKING,
	CLUMP,
	WANDER,
	PURSUE,
	ARRIVAL,
	SEEK,
	QUEUE,
	NONNORMALIZEDFLOCKING
};
class boidManager
{
public:
	sf::Texture m_BoidTexture;
	std::vector<boid> m_boidVector;
	sf::RenderWindow* m_windowRef;
	std::vector<Obstacle>  m_obstacles;
	sf::Vector2i m_lastMousePosition;
	sf::Clock m_clock;
	boidType m_selectedType;
	boidManager(sf::RenderWindow* _windowRef);
	~boidManager();
	void createBoid(sf::Vector2i _mousePosition);
	void update();
	void addObstacle(const sf::Vector2i position, float radius);

}; 


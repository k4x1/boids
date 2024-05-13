// Include the header file for the boidManager class
#include "boidManager.h"

// Constructor for the boidManager class
boidManager::boidManager(sf::RenderWindow* _windowRef)
{
    // Store the reference to the SFML window
    m_windowRef = _windowRef;
    // Attempt to load a texture for the boids, print an error if it fails
    if (!m_BoidTexture.loadFromFile("assets/boid.png"))
    {
        std::cout << "loading failed" << std::endl;
    }
}

// Destructor for the boidManager class
boidManager::~boidManager()
{
  
}

// Function to create a new boid at the mouse position
void boidManager::createBoid(sf::Vector2i _mousePosition)
{
    // Create a new boid with the loaded texture
    boid newBoid(&m_BoidTexture);
    // Set the position of the new boid to the mouse position
    newBoid.m_Pos = sf::Vector2f(_mousePosition);
    // Add the new boid to the vector of boids
    m_boidVector.push_back(newBoid);
    // Output the current number of boids
    std::cout << m_boidVector.size() << std::endl;
}

// Function to update the state of all boids
void boidManager::update()
{
    // Get the current mouse position relative to the window
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_windowRef);
    // Restart the clock and calculate the elapsed time in seconds
    sf::Time elapsed = m_clock.restart();
    float deltaTime = elapsed.asSeconds();

    // Calculate the velocity of the mouse
    sf::Vector2f mouseVelocity = static_cast<sf::Vector2f>(mousePosition - m_lastMousePosition) / deltaTime;
    m_lastMousePosition = mousePosition;

    // Draw all obstacles
    for (auto& obstacle : m_obstacles)
    {
        m_windowRef->draw(obstacle.m_Shape);
    }
    // Update and draw each boid based on the selected behavior
    for (auto& boid : m_boidVector)
    {
        switch (m_selectedType) {
        case FLOCKING:
            boid.Flocking(m_boidVector);
            break;
        case CLUMP:
            boid.Clump(m_boidVector);
            break;
        case WANDER:
            boid.Wander();
            break;
        case PURSUE:
            boid.Pursue(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)), mouseVelocity);
            break;
        case ARRIVAL:
            boid.Arrival(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)), 200.0f);
            break;
        case SEEK:
            boid.Seek(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
            break;
        case QUEUE:
            boid.Queue(m_boidVector, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
            break;
        case NONNORMALIZEDFLOCKING:
            boid.nonNormalizedFlocking(m_boidVector);
            break;
        default:
            std::cout << "Unknown behavior" << std::endl;
            break;
        }
        // Additional behaviors like object avoidance and movement
        boid.ObjectAvoidance(m_obstacles);
        boid.Move();
        // Draw the boid
        m_windowRef->draw(boid.m_Sprite);
    }
}

// Function to add an obstacle to the simulation
void boidManager::addObstacle(const sf::Vector2i position, float radius) {
    // Create and add an obstacle with the given position and radius
    m_obstacles.push_back(Obstacle{ sf::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y)), radius });
}

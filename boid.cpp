#include "boid.h"



// Calculate the Euclidean distance between two vectors
float vectorDistance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return std::sqrt(dx * dx + dy * dy);
}

// Calculate the magnitude of a vector
float VectorMagnitude(const sf::Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}
// Normalize a vector to have a magnitude of 1
void Normalize(sf::Vector2f& v) {
	float length = VectorMagnitude(v);
	if (length > 0) {
		v /= length;
	}
}
// Limit the magnitude of a vector to a maximum value
void LimitMagnitude(sf::Vector2f& v, float max) {
	float currentMagnitude = VectorMagnitude(v);
	if (currentMagnitude > max) {
		Normalize(v);
		v *= max;
	}
}
// Constructor for the boid class
boid::boid(sf::Texture* _tex)
{
	m_Rot = 0;
	m_Direction = sf::Vector2f(0.1f, 0);
	m_Sprite.setTexture(*_tex);
	m_Sprite.setScale(sf::Vector2f(0.15f, 0.15f));
	m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2.0f, m_Sprite.getLocalBounds().height / 2.0f);
	
	m_Speed =30.0f;
	 
}
// Destructor for the boid class
boid::~boid()
{
	
}

// Move the boid based on its direction
void boid::Move()
{
	// Calculate the current speed of the boid using the Pythagorean theorem
	float currentSpeed = std::sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);

	// Check if the current speed exceeds the maximum speed allowed (1.0f in this case)
	if (currentSpeed > 1.0f) {
		// Calculate the ratio of the maximum speed to the current speed
		float speedRatio = 1.0f / currentSpeed;

		// Adjust the direction vector to fit within the maximum speed by normalizing it
		m_Direction *= speedRatio;
	}

	// Update the position of the boid by adding the direction vector scaled by a speed factor (0.3f)
	m_Pos += m_Direction * 0.3f;

	// Implement wrapping behavior for the boid when it goes out of bounds (assuming a window size of 1000x1000)
	// If the boid's x-coordinate is greater than 1000, wrap it to 0; if less than 0, wrap it to 1000
	m_Pos.x = (m_Pos.x > 1000) ? 0 : (m_Pos.x < 0) ? 1000 : m_Pos.x;
	// If the boid's y-coordinate is greater than 1000, wrap it to 0; if less than 0, wrap it to 1000
	m_Pos.y = (m_Pos.y > 1000) ? 0 : (m_Pos.y < 0) ? 1000 : m_Pos.y;

	// Calculate the rotation of the boid in degrees based on its direction vector
	// atan2 returns the angle in radians, so it is converted to degrees (radians * 180 / PI)
	m_Rot = std::atan2(m_Direction.y, m_Direction.x) * 180.0f / M_PI;

	// Set the rotation and position of the boid's sprite for rendering
	m_Sprite.setRotation(m_Rot);
	m_Sprite.setPosition(m_Pos);

	
}

//old flocking function, might add to options, behaviour flocks correctly but the boids only like going to the bottom right
void boid::nonNormalizedFlocking(std::vector<boid>& flock) {
	const float viewRadius = 200.0f;          // Distance within which a boid considers others for flocking
	const float separationDistance = 50.0f;   // Minimum distance boids try to maintain from each other
	const float separationStrength = 20.0f;   // How strongly boids avoid each other
	const float cohesionStrength = 0.5f;      // How strongly boids move towards the group center
	const float alignmentStrength = 2.0f;     // How strongly boids align their direction with the group


	
	sf::Vector2f distanceForce(0, 0);
	sf::Vector2f separationForce(0, 0);
	sf::Vector2f cohesionForce(0, 0);
	sf::Vector2f alignemntForce(0, 0);

	int separationCount = 0;
	int cohesionCount = 0;
	int alignemntCount = 0;

	// Iterate over all boids in the flock
	for (auto& other : flock) {
		if (&other != this) {   // Skip self in calculations
			float distance = vectorDistance(m_Pos, other.m_Pos);
			
			if (abs(distance) < viewRadius) {
				if (abs(distance) < separationDistance) {
					// Check for separation
					separationCount++;
					separationForce += (m_Pos - other.m_Pos);
				}
				cohesionForce += other.m_Pos;
				cohesionCount++;
				alignemntForce += other.m_Direction;
				alignemntCount++;
			}
		}
	}
	// Calculate average forces if applicable
	separationForce = separationCount != 0 ? sf::Vector2f(separationForce.x / separationCount, separationForce.y / separationCount) : separationForce;
	cohesionForce = cohesionCount != 0 ? sf::Vector2f(cohesionForce.x / cohesionCount, cohesionForce.y / cohesionCount) : cohesionForce;
	alignemntForce = alignemntCount != 0 ? sf::Vector2f(alignemntForce.x / alignemntCount, alignemntForce.y / alignemntCount) : alignemntForce;


	// Combine all forces with their respective strengths and apply to boid's direction
	
	distanceForce = (separationForce * separationStrength) + (cohesionForce * cohesionStrength) + (alignemntForce * alignmentStrength);
	m_Direction += sf::Vector2f(distanceForce.x / 2000, distanceForce.y / 2000);


}

// Function to calculate flocking behavior for a boid
void boid::Flocking(std::vector<boid>& flock) {
	// Constants defining the behavior of the boids
	const float viewRadius = 200.0f;          // Distance within which a boid considers others for flocking
	const float separationDistance = 30.0f;   // Minimum distance boids try to maintain from each other
	const float separationStrength = 0.1f;    // How strongly boids avoid each other
	const float cohesionStrength = 0.05f;     // How strongly boids move towards the group center
	const float alignmentStrength = 0.1f;     // How strongly boids align their direction with the group

	// Variables to accumulate forces and count boids within certain distances
	sf::Vector2f separationForce(0, 0);       
	sf::Vector2f cohesionForce(0, 0);         
	sf::Vector2f alignmentForce(0, 0);        
	int separationCount = 0;                  
	int cohesionCount = 0;                    
	int alignmentCount = 0;                   

	// Iterate over all boids in the flock
	for (auto& other : flock) {
		if (&other != this) {                 // Skip self in calculations
			sf::Vector2f diff = m_Pos - other.m_Pos; // Vector from other boid to this boid
			float distance = VectorMagnitude(diff);  // Calculate distance to other boid

			// Check if the other boid is within the view radius
			if (distance < viewRadius) {
				// Apply separation logic if within separation distance
				if (distance < separationDistance) {
					separationCount++;
					Normalize(diff);                 // Normalize the difference vector
					separationForce += diff / distance; // Accumulate normalized separation force
				}
				// Accumulate data for cohesion
				cohesionForce += other.m_Pos;
				cohesionCount++;

				// Accumulate data for alignment
				alignmentForce += other.m_Direction;
				alignmentCount++;
			}
		}
	}

	// Normalize and calculate average forces if applicable
	if (separationCount > 0) {
		separationForce /= float(separationCount); // Average the separation force
		Normalize(separationForce);                // Normalize the average separation force
	}
	if (cohesionCount > 0) {
		cohesionForce /= float(cohesionCount);     // Average the cohesion force
		cohesionForce = (cohesionForce - m_Pos) / 100.0f; // Calculate direction to the center of mass
		Normalize(cohesionForce);                  // Normalize the cohesion force
	}
	if (alignmentCount > 0) {
		alignmentForce /= float(alignmentCount);   // Average the alignment force
		Normalize(alignmentForce);                 // Normalize the alignment force
	}

	// Combine all forces with their respective strengths
	sf::Vector2f steeringForce = (separationForce * separationStrength) +
		(cohesionForce * cohesionStrength) +
		(alignmentForce * alignmentStrength);

	// Limit the magnitude of the steering force to the maximum speed of the boid
	LimitMagnitude(steeringForce, m_Speed);
	m_Direction += steeringForce; // Apply the steering force to the boid's direction
}

// Function to steer the boid towards a specified target
void boid::Seek(const sf::Vector2f& target) {
	const float maxSpeed = 1.0f;              // Maximum speed the boid can move
	const float steeringStrength = 0.1f;      // Maximum strength of the steering force

	// Calculate the desired direction towards the target
	sf::Vector2f desiredDirection = target - m_Pos; // Vector from current position to target

	// Calculate the magnitude of the desired direction
	float length = std::sqrt(desiredDirection.x * desiredDirection.x + desiredDirection.y * desiredDirection.y);
	if (length > 0) {
		desiredDirection /= length;          // Normalize the direction vector
	}

	// Scale the desired direction by the maximum speed
	desiredDirection *= maxSpeed;

	// Calculate the steering force as the difference between desired direction and current direction
	sf::Vector2f steeringForce = desiredDirection - m_Direction;

	// Calculate the magnitude of the steering force
	float steeringLength = std::sqrt(steeringForce.x * steeringForce.x + steeringForce.y * steeringForce.y);
	if (steeringLength > steeringStrength) {
		steeringForce /= steeringLength;     // Normalize the steering force
		steeringForce *= steeringStrength;   // Scale the steering force to the maximum steering strength
	}

	// Apply the steering force to the boid's direction
	m_Direction += steeringForce;

	// Ensure the boid's speed does not exceed the maximum speed
	float currentSpeed = std::sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);
	if (currentSpeed > maxSpeed) {
		m_Direction /= currentSpeed;         // Normalize the direction vector
		m_Direction *= maxSpeed;             // Scale the direction vector to the maximum speed
	}
}
// Function to pursue a moving target
void boid::Pursue(sf::Vector2f targetPos, sf::Vector2f targetVelocity) {
	float predictionTime = 1.0f;  // Time ahead in the future to predict the target's position

	// Calculate the predicted position of the target based on its current position and velocity
	sf::Vector2f predictedPosition = targetPos + targetVelocity * predictionTime;

	// Call the Seek function to move towards the predicted position of the target
	Seek(predictedPosition);
}

// Function to simulate wandering behavior for a boid
void boid::Wander() {
	float circleDistance = 100.0f;  // Distance from the boid to the wandering circle's center
	float circleRadius = 30.0f;     // Radius of the wandering circle
	float angleChange = 0.1f;       // Maximum change in angle per update

	// Calculate the circle center position based on the boid's current direction
	sf::Vector2f circleCenter = m_Direction;
	float length = std::sqrt(circleCenter.x * circleCenter.x + circleCenter.y * circleCenter.y);
	if (length > 0) {
		circleCenter /= length;  // Normalize the direction vector
	}
	circleCenter *= circleDistance;  // Move the circle center ahead of the boid
	circleCenter += m_Pos;           // Position the circle center in world space

	// Randomly change the wander angle using a uniform distribution
	std::random_device rd;  // Random number generator
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1, 1); // Uniform distribution between -1 and 1
	m_WanderAngle += float(dis(gen) * angleChange); // Adjust the wander angle

	// Calculate the displacement vector pointing to the edge of the circle
	sf::Vector2f displacement(0, -1); // Initial displacement vector
	displacement *= circleRadius;     // Scale displacement to the radius of the circle

	// Normalize the displacement vector
	float len = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
	if (len > 0) {
		displacement /= len;
	}

	// Rotate the displacement vector by the wander angle
	displacement.x = displacement.x * cos(m_WanderAngle) - displacement.y * sin(m_WanderAngle);
	displacement.y = displacement.x * sin(m_WanderAngle) + displacement.y * cos(m_WanderAngle);

	// Calculate the target position on the perimeter of the circle
	sf::Vector2f wanderTarget = circleCenter + displacement;

	// Seek towards the calculated target position
	Seek(wanderTarget);
}


void boid::Arrival(const sf::Vector2f& target, float slowingDistance)
{
	const float maxSpeed = 1.0f;
	const float steeringStrength = 0.1f;

	sf::Vector2f desiredDirection = target - m_Pos;

	float distance = std::sqrt(desiredDirection.x * desiredDirection.x + desiredDirection.y * desiredDirection.y);
	if (distance > 0) {
		desiredDirection /= distance;
	}

	float speed = maxSpeed;
	if (distance < slowingDistance) {
		speed = maxSpeed * (distance / slowingDistance);
	}

	desiredDirection *= speed;

	sf::Vector2f steeringForce = desiredDirection - m_Direction;

	float steeringLength = std::sqrt(steeringForce.x * steeringForce.x + steeringForce.y * steeringForce.y);
	if (steeringLength > steeringStrength) {
		steeringForce /= steeringLength;
		steeringForce *= steeringStrength;
	}

	m_Direction += steeringForce;

	float currentSpeed = std::sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);
	if (currentSpeed > maxSpeed) {
		m_Direction /= currentSpeed;
		m_Direction *= maxSpeed;
	}


}

void boid::Clump(const std::vector<boid>& flock) {
	//failed attempt to make queue behaviour but since the pattern it makes is really really cool I decided to keep it

	const float queueRadius = 500.0f;
	const float queueDistance = 200.0f;
	const float queueStrength = 1.0f;

	sf::Vector2f queueForce(0.0f, 0.0f);
	int queueCount = 0;

	for (const auto& other : flock) {
		if (&other != this) {
			float distance = vectorDistance(m_Pos, other.m_Pos);
			if (distance < queueRadius) {
				sf::Vector2f offset = other.m_Pos - m_Pos;
				float offsetLength = std::sqrt(offset.x * offset.x + offset.y * offset.y);
				if (offsetLength > 0) {
					offset /= offsetLength;
				}
				offset *= queueDistance;

				sf::Vector2f desiredPos = other.m_Pos - offset;
				sf::Vector2f forceToDesired = desiredPos - m_Pos;
				queueForce += forceToDesired;
				queueCount++;
			}
		}
	}

	if (queueCount > 0) {
		queueForce /= static_cast<float>(queueCount);
		float queueLength = std::sqrt(queueForce.x * queueForce.x + queueForce.y * queueForce.y);
		if (queueLength > queueStrength) {
			queueForce /= queueLength;
			queueForce *= queueStrength;
		}
		m_Direction += queueForce;
	}


}
// Define the ObjectAvoidance method for the boid class, taking a vector of Obstacles as input.
void boid::ObjectAvoidance(const std::vector<Obstacle>& obstacles) {
	// Set the minimum distance at which avoidance starts.
	float avoidDistance = 10.0f;
	// Initialize a vector to accumulate steering forces.
	sf::Vector2f steer(0, 0);

	// Iterate over each obstacle in the provided vector.
	for (const auto& obstacle : obstacles) {
		// Calculate the vector from the obstacle to the boid.
		sf::Vector2f diff = m_Pos - obstacle.m_Pos;
		// Compute the Euclidean distance between the boid and the obstacle.
		float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		// Calculate the buffer zone which includes the obstacle's radius plus the avoidance distance.
		float buffer = avoidDistance + obstacle.m_Radius;

		// Check if the distance is less than the buffer zone.
		if (dist < buffer) {
			// Normalize the difference vector.
			diff /= dist;
			// Calculate the weight for the steering force based on the proximity to the obstacle.
			float weight = (buffer - dist) / buffer;
			// Apply the weighted steering force.
			steer += diff * weight;
		}
	}

	// Check if there is a non-zero steering force.
	if (steer != sf::Vector2f(0, 0)) {
		// Normalize the steering force.
		steer /= std::sqrt(steer.x * steer.x + steer.y * steer.y);
		// Adjust the boid's direction by the steering force.
		m_Direction += steer;
	}
}
// Define the Queue method for the boid class, taking a vector of boids (flock) and a target position as input.
void boid::Queue(const std::vector<boid>& flock, const sf::Vector2f& target) {
	// Initialize a vector to accumulate queueing forces, though it remains unused in this implementation.
	sf::Vector2f queueingForce(0.0f, 0.0f);
	// Start by assuming the first boid in the flock is the one to follow.
	boid agentToFollow = flock[0];

	// If this boid is the first in the flock, it should seek the target instead of following another boid.
	if (this == &flock[0]) {
		Seek(target);
		return; // Exit the function after seeking the target.
	}

	// Loop through the flock to find this boid and set the boid in front of it as the one to follow.
	for (int i = 0; i < flock.size(); i++) {
		if (this == &flock[i]) {
			agentToFollow = flock[i - 1]; // Set the previous boid in the array as the agent to follow.
		}
	}

	// Calculate the position vector to the front agent from this boid.
	sf::Vector2f frontAgentPos = agentToFollow.m_Pos;
	sf::Vector2f toFrontAgent = frontAgentPos - m_Pos;
	// Calculate the distance to the front agent.
	float distance = VectorMagnitude(toFrontAgent);

	// Define the desired distance to maintain in the queue.
	const float QUEUE_DISTANCE = 50.0f;

	// If the distance to the front agent is greater than the queue distance, seek towards the front agent's position.
	if (distance > QUEUE_DISTANCE) {
		Seek(agentToFollow.m_Pos);
	}

	// Apply the queueing force to the boid's direction (though queueingForce is not modified in this code).
	m_Direction += queueingForce;
	// Normalize the direction vector to ensure it's a unit vector.
	Normalize(m_Direction);
	// Limit the magnitude of the direction vector to the boid's speed to maintain consistent movement.
	LimitMagnitude(m_Direction, m_Speed);
}

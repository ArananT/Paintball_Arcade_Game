#include "player_game_object.h"
#include <iostream>
namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PlayerGameObject::PlayerGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture), playerCollider(position.x, position.y, 0.45f) {
		shoot = new Timer();
		shoot->Start(0.5);
	}


	PlayerGameObject::~PlayerGameObject() {
		std::cout << "playerdtor" << std::endl;
		delete shoot;

	}


	void PlayerGameObject::startshoot() {
		if (shoot != nullptr) {
			shoot->Start(0.5);
		}
	}

	bool PlayerGameObject::shootready() {
		if (shoot != nullptr) {
			return shoot->Finished();
		}
		return false;
	}
	void PlayerGameObject::Accelerate(glm::vec3 acceleration) {
		glm::vec3 new_velocity = velocity + acceleration;

		// Calculate the magnitude of the new velocity
		float velocity_magnitude = sqrt(new_velocity.x * new_velocity.x +
			new_velocity.y * new_velocity.y +
			new_velocity.z * new_velocity.z);

		// Check if the new velocity exceeds the maximum speed of 2 units this should limit it to 2 units per second
		if (velocity_magnitude > 0.2f) {
			
			new_velocity = (new_velocity / velocity_magnitude) * 0.2f;
		}

		// Update the player's velocity
		velocity = new_velocity;
	}


	glm::vec3 PlayerGameObject::getVelocity() { return velocity; }


	// Update function for moving the player object around
	void PlayerGameObject::Update(double delta_time) {

		// Special player updates go here
		position_ += velocity * static_cast<float>(delta_time);
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game


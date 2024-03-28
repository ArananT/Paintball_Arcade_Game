#ifndef ENEMY_GAME_OBJECT_H
#define ENEMY_GAME_OBJECT_H

#include "game_object.h" 
#include "Timer.h" 
#include <glm/glm.hpp>

namespace game {

    class EnemyGameObject : public GameObject {

    public:
        enum class State {
            Patrolling,
            Intercepting
        };

        // Constructor and Destructor
        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        virtual ~EnemyGameObject();

        // Override these methods from GameObject
        virtual void Update(double delta_time) override;
        virtual void Render(glm::mat4 view_matrix, double current_time) override;

        // Methods for handling interception behavior
        void interceptStart();
        bool interceptDone();

        glm::vec3 GetVelocity() const;

        State getCurrentState() const;

        void setState(State newstate);

        // Method to set a new velocity
        void SetVelocity(const glm::vec3& newVelocity);


        // Setter for the target position (e.g., player's position)
        void SetTargetPosition(const glm::vec3& targetPos);

    private:
        Timer* interceptTimer; // Timer to manage modifications to the velocity vector
        glm::vec3 velocity; // Current velocity of the enemy
        glm::vec3 targetPosition; // Current target position

        // Attributes for patrolling behavior
        float patrolAngle; // Current angle in the patrol circle
        float patrolRadius; // Radius of the patrol circle
        glm::vec3 patrolCenter; // Center of the patrol circle

    
        State currentState; // Current state of the enemy
    };

} // namespace game

#endif // ENEMY_GAME_OBJECT_H

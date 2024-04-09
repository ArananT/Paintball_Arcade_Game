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

        void BlindStart();
        bool BlindFinished();

    private:
        Timer* interceptTimer; // Timer to manage modifications to the velocity vector
        glm::vec3 velocity; // Current velocity of the enemy
        glm::vec3 targetPosition; // Current target position

        // Attributes for patrolling behavior
        float patrolAngle; // Current angle in the patrol circle
        float patrolRadius; // Radius of the patrol circle
        glm::vec3 patrolCenter; // Center of the patrol circle
        State currentState; // Current state of the enemy

        bool blind; // If the enemy is hit by a paint bucket
        Timer* blindTimer; // Timer to manage the length of enemy blindness
    };
    class Shooter : public EnemyGameObject {
    public:

        enum class State {
            Patrolling,
            Intercepting
        };

        void setState(State newstate);

        Shooter::Shooter(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        virtual Shooter::~Shooter();

        virtual void Update(double delta_time);

        void interceptStart();
        bool interceptDone();

        void shootStart();
        bool shootDone();

        void SetTargetPosition(const glm::vec3& targetPos);

        bool canShoot();

    private:
        Timer* interceptTimer; // Timer to manage modifications to the velocity vector
        glm::vec3 velocity; // Current velocity of the enemy
        glm::vec3 targetPosition; // Current target position

        // Attributes for patrolling behavior
        float patrolAngle; // Current angle in the patrol circle
        float patrolRadius; // Radius of the patrol circle
        glm::vec3 patrolCenter; // Center of the patrol circle
        State currentState; // Current state of the enemy
        bool canshoot;
        Timer* shootTimer;
    };

    class StaticEnemy : public EnemyGameObject {
    public:
        virtual void update(double delta_time);
    };

    class InterceptingEnemy : public EnemyGameObject {
    public:
        virtual void update(double delta_time);
    };
} // namespace game

#endif // ENEMY_GAME_OBJECT_H

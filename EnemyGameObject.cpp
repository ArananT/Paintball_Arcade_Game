#include "EnemyGameObject.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
namespace game {

    EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
        : GameObject(position, geom, shader, texture), patrolAngle(0.0f), patrolRadius(1.0f), patrolCenter(glm::vec3(1.0f + position.x, 1.0f + position.y, 0.0f)) {
        scale_ = glm::vec2(1.0f, 1.0f);
        // Assuming State is defined and Patrolling is a value in the State enum
        currentState = State::Patrolling;
        interceptTimer = new Timer();
        interceptStart();

    }
    EnemyGameObject::~EnemyGameObject() {
        delete interceptTimer;
    }

    glm::vec3 EnemyGameObject::GetVelocity() const {
        return velocity; // Return the current velocity
    }

    EnemyGameObject::State EnemyGameObject::getCurrentState() const {
        return currentState;
    }

    void EnemyGameObject::setState(State newstate)
    {
        currentState = newstate;
    }

    void EnemyGameObject::SetVelocity(const glm::vec3& newVelocity) {
        velocity = newVelocity; // Update the velocity to the new value
    }

    void EnemyGameObject::SetTargetPosition(const glm::vec3& targetPos) {
        targetPosition = targetPos;
    }


    void EnemyGameObject::Update(double delta_time) {

        GameObject::Update(delta_time);

        if (currentState == State::Intercepting) {

            if (interceptDone()) {
                // Calculate direction towards the player
                glm::vec3 direction = glm::normalize(targetPosition - position_);
                // Update velocity direction without changing its magnitude
                float speed = 0.02f;
                velocity = direction * speed;

                interceptStart();
            }

            // Move the enemy based on its velocity
            position_ += velocity * static_cast<float>(delta_time);
        }
        // Assuming currentState is a member variable that is already set to Patrolling
        else if (currentState == State::Patrolling) {
            // Increment the angle to move along the circle
            patrolAngle += 0.1 * delta_time; // Increase by some value to change the speed

            // Use the defined value of 2 * PI
            const float twoPi = 2.0f * static_cast<float>(M_PI); // Or directly use 6.28318530718f if M_PI is not available
            if (patrolAngle > twoPi) {
                patrolAngle -= twoPi;
            }

            // Parametric equation for a circle
            float newX = patrolCenter.x + patrolRadius * cos(patrolAngle);
            float newY = patrolCenter.y + patrolRadius * sin(patrolAngle);

            // Update the position of the enemy game object
            SetPosition(glm::vec3(newX, newY, position_.z));
        }
    }

    void EnemyGameObject::Render(glm::mat4 view_matrix, double current_time) {
        shader_->Enable();
        shader_->SetUniformMat4("view_matrix", view_matrix);

        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0f));
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0f));
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
        glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

        shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
        geometry_->SetGeometry(shader_->GetShaderProgram());
        glBindTexture(GL_TEXTURE_2D, texture_);
        glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
    }


    void EnemyGameObject::interceptStart() {

        if (interceptTimer != nullptr) { // Always check if the pointer is not null
            interceptTimer->Start(2.0);
        }
    }
    bool EnemyGameObject::interceptDone() {
        if (interceptTimer != nullptr) { // Always check if the pointer is not null

            return interceptTimer->Finished();
        }
        return false; // Assume not started if timer is null
    }
    
    //SHOOTER_CLASS

 

    Shooter::Shooter(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
        : EnemyGameObject(position, geom, shader, texture), patrolAngle(0.0f), patrolRadius(0.0f), patrolCenter(glm::vec3(0.0f + position.x, 0.0f + position.y, 0.0f)) {
        scale_ = glm::vec2(1.0f, 1.0f);
        // Assuming State is defined and Patrolling is a value in the State enum
        currentState = State::Patrolling;
        interceptTimer = new Timer();
        shootTimer = new Timer();
        interceptStart();
        shootStart();

    }
    Shooter::~Shooter() {
        delete interceptTimer;
        delete shootTimer;
    }

    


    void Shooter::Update(double delta_time) {

        GameObject::Update(delta_time);

        if (currentState == State::Intercepting) {
            // Calculate direction towards the player
            glm::vec3 direction = glm::normalize(targetPosition - position_);

            float angleDegrees = std::atan2(direction.y, direction.x);


            SetRotation(angleDegrees);
            if (interceptDone()) {

                std::cout << "TIMERDONE" << std::endl;

                
                glm::vec3 direction = glm::normalize(targetPosition - position_);
                // Update velocity direction without changing its magnitude
                float speed = 0.2f;
                velocity = direction * speed;
                SetVelocity(velocity);
                std::cout << "TargetPosition_X" << velocity.x << std::endl;
                std::cout << "TargetPosition_Y" << velocity.y << std::endl;
                interceptStart();
            }

        }
        // Assuming currentState is a member variable that is already set to Patrolling
        else if (currentState == State::Patrolling) {
            // Increment the angle to move along the circle
            patrolAngle += 0.1 * delta_time; // Increase by some value to change the speed

            // Use the defined value of 2 * PI
            const float twoPi = 2.0f * static_cast<float>(M_PI); // Or directly use 6.28318530718f if M_PI is not available
            if (patrolAngle > twoPi) {
                patrolAngle -= twoPi;
            }

            // Parametric equation for a circle
            float newX = patrolCenter.x + patrolRadius * cos(patrolAngle);
            float newY = patrolCenter.y + patrolRadius * sin(patrolAngle);

            // Update the position of the enemy game object
            SetPosition(glm::vec3(newX, newY, position_.z));
        }
        
    }



    void Shooter::setState(State newstate) {
        currentState = newstate;
    }

    void Shooter::interceptStart() {

        if (interceptTimer != nullptr) { // Always check if the pointer is not null
            interceptTimer->Start(1.0);
        }
    }
    bool Shooter::interceptDone() {
        if (interceptTimer != nullptr) { // Always check if the pointer is not null

            return interceptTimer->Finished();
        }
        
        return false; // Assume not started if timer is null
    }

    void Shooter::shootStart() {

        if (shootTimer != nullptr) { // Always check if the pointer is not null
            shootTimer->Start(2.0);
        }
    }
    bool Shooter::shootDone() {
        if (shootTimer != nullptr) { // Always check if the pointer is not null
            if (shootTimer->Finished()) {
                shootTimer->Start(2.0);
                return true;
            }
            else {
                return false;
            }
          
        }

        return false; // Assume not started if timer is null
    }

    void Shooter::SetTargetPosition(const glm::vec3& targetPos) {
        targetPosition = targetPos;
    }

    //void StaticEnemy::update() {
    //   
    //}


    //void InterceptingEnemy::update() {
    //}

} // namespace game

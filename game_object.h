#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include "CircleCollider.h"
#include "shader.h"
#include "geometry.h"
#include "timer.h"
#include <cmath>

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        CircleCollider collider;
        private:
            bool is_temporary_;  // To check if the object is temporary
            double creation_time_;  // To store the creation time of the object
            bool collide;
            Timer* explosionTimer;
            Timer* invincibilityTimer;
            
        public:
            // Constructor
            GameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
            ~GameObject();

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            virtual void Render(glm::mat4 view_matrix, double current_time, glm::mat4 transformation_matrix);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            inline bool canCollide(void) const { return collide; }
            inline Timer* getTimer(void) { return explosionTimer; }
            virtual inline bool isCollect(void) { return false; }
            virtual inline bool isEnemy(void) { return false; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters

            //Start explosion timer
            inline void boomStart() {
                if (explosionTimer != nullptr) { // Always check if the pointer is not null
                    explosionTimer->Start(5.0); // Use the arrow operator (->) to call methods on the pointer
                }
            }

            // Abilities
            inline void invincibleStart() {
                if (invincibilityTimer != nullptr) { // Always check if the pointer is not null
                    invincibilityTimer->Start(10.0); // Use the arrow operator (->) to call methods on the pointer
                }
            }
            inline bool invincibleDone() {
                if (invincibilityTimer != nullptr) { // Always check if the pointer is not null
                    
                    return invincibilityTimer->Finished(); // Use the arrow operator (->) to call methods on the pointer
                }
                return false; // Assume not started if timer is null
            }

            //explosion time (timer finished)
            inline bool boomTime() {
                if (explosionTimer != nullptr) { // Always check if the pointer is not null
                    return explosionTimer->Finished(); // Use the arrow operator (->) to call methods on the pointer
                }
                return false; // Assume not started if timer is null
            }

            inline void SetPosition(const glm::vec3& position) {
                position_ = position;
                collider.x = position.x;  // Update the collider's position
                collider.y = position.y;
            }
            inline void toggleCollide() {
                collide = false;
            }

            glm::mat4 BladeTransform(const glm::vec3& PlayerPosition, const float& playerAngle);

            bool RayCircleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                const glm::vec3& circleCenter, float circleRadius);

            float FindCollisionTime(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& raySpeed,
                const glm::vec3& circleCenter, float circleRadius, float deltaTime);


            inline void SetScale(glm::vec2 scale) { scale_ = scale; }
            void SetRotation(float angle);

            inline float GetAngle() { return angle_; }

            inline

            void SetTex(GLuint texture) { texture_ = texture; }
            GLuint GetTex() const { return texture_; }

            //GetCollider()
            CircleCollider& GetCollider() { return collider; }

            // Methods for handling temporary status
            void SetTemporary(bool is_temporary) {
                is_temporary_ = is_temporary;
            }

            bool IsTemporary() const {
                return is_temporary_;
            }

            // Methods for handling creation time
            void SetCreationTime(double time) {
                creation_time_ = time;
            }

            double GetCreationTime() const {
                return creation_time_;
            }

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;
            // radius 
            float radius_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_

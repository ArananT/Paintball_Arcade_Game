#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {
        CircleCollider playerCollider;
    private:
        glm::vec3 velocity;
        Timer* shoot;
        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
            virtual ~PlayerGameObject();
            void startshoot();
            bool shootready();
            

            void Accelerate(glm::vec3 acceleration);


            float getVelocity();



            // Update function for moving the player object around
            void Update(double delta_time) override;
           


    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_

#ifndef COLLECTABLE_GAME_OBJECT_H
#define COLLECTABLE_GAME_OBJECT_H

#include "game_object.h"
#include "shader.h"
#include "geometry.h"
#include <glm/glm.hpp>

namespace game {
    class CollectableGameObject : public game::GameObject {

        CircleCollider collectableCollider;
    public:
        // Constructor matching the GameObject's constructor signature
        CollectableGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Destructor
        virtual ~CollectableGameObject();

        // Override any necessary methods from GameObject
        virtual void Update(double delta_time) override;
        virtual void Render(glm::mat4 view_matrix, double current_time) override;
        bool isCollect(void) override { return true; }
    };

        // Additional methods specific to CollectableGameObject
    };

#endif // COLLECTABLE_GAME_OBJECT_H

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "CircleCollider.h"
#include "game_object.h"

namespace game {

    GameObject::GameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
        : collider(position.x, position.y, .45f), is_temporary_(false), creation_time_(0.0), collide(true),  scale_((1.0f, 1.0f)) {


        // Initialize all attributes
        radius_ = 0.0;
        position_ = position;
        
        angle_ = 0.0;
        geometry_ = geom;
        shader_ = shader;
        texture_ = texture;
        explosionTimer = new Timer();
        invincibilityTimer = new Timer();
    }

    GameObject::~GameObject() {
        delete explosionTimer;
        delete invincibilityTimer;
    }

glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}

glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0f);
    return dir;
}





void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}


void GameObject::Update(double delta_time) {
    collider.x = position_.x;
    collider.y = position_.y;

}


void GameObject::Render(glm::mat4 view_matrix, double current_time) {

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0f));


    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}


//function where the heirarchical transformations of the blade are done 
glm::mat4 GameObject::BladeTransform(const glm::vec3& PlayerPosition, const float& playerAngle) {
    // The position of the blade is set to the player's position, 
    // meaning they share the same center point
    position_ = PlayerPosition;

    // Apply the player's rotation to the blade
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), playerAngle, glm::vec3(0, 0, 1));

    // Update the blade's angle to rotate independently
    // The blade's own rotation angle is relative to the player's center/rotation
    angle_ += 0.06; // Increment by some amount per frame/update

    // Create the blade's own rotation matrix
    glm::mat4 bladeRotationMatrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0, 0, 1));

    // The blade's final transformation matrix is the player's rotation matrix
    // followed by the blade's rotation matrix
    glm::mat4 transformation_matrix = rotationMatrix * bladeRotationMatrix;
    return transformation_matrix;
}


//Custom Rendering only for the blade so that it can use the heirarcichal transformations
void GameObject::Render(glm::mat4 view_matrix, double current_time,glm::mat4 transformation_matrix) {

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0f));


    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    transformation_matrix = transformation_matrix ;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

bool GameObject::RayCircleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                    const glm::vec3& circleCenter, float circleRadius) {
    glm::vec3 E = circleCenter - rayOrigin;
    float dotProduct = glm::dot(E, rayDirection);
    glm::vec3 F = rayOrigin + dotProduct * rayDirection;
    float distance = glm::length(F - circleCenter); // Corrected distance calculation

    return distance <= circleRadius;
}


// Returns the collision time 't' if there is a collision within the frame, otherwise returns -1
float GameObject::FindCollisionTime(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,const glm::vec3&raySpeed,
    const glm::vec3& circleCenter, float circleRadius, float deltaTime) {

    //Ray Circle Collision detection
    glm::vec3 E = circleCenter - rayOrigin;
    float dotProduct = glm::dot(E, rayDirection);
    glm::vec3 F = rayOrigin + dotProduct * rayDirection;
    float distanceToRay = glm::length(F - circleCenter);

    // No collision if the circle is further from the ray than its radius at the closest point
    if (distanceToRay > circleRadius) { return -1; }

    // Compute the distance along the ray from the closest approach to the circle's edge
    float halfChordDistance = sqrt(circleRadius * circleRadius - distanceToRay * distanceToRay);
    float rayLengthToCollision = dotProduct - halfChordDistance;  // Subtract because we want the first point of intersection

    // Calculate the time of collision based on the speed of the ray
     float raySpeeds = glm::length(raySpeed);  // Assumes rayDirection is not normalized
    float collisionTime = rayLengthToCollision / raySpeeds;

    // Check if the collision occurs within this frame
    if (collisionTime >= 0 && collisionTime <= deltaTime) {
        return collisionTime;
    }
    else {
        return -1;  // No collision within the time frame
    }
}

bool GameObject::inTile(GameObject* tile) {
    float minX = tile->GetPosition().x - 20.0f / 2.0f;
    float minY = tile->GetPosition().y - 20.0f / 2.0f;
    float maxX = tile->GetPosition().x + 20.0f / 2.0f;
    float maxY = tile->GetPosition().y + 20.0f / 2.0f;

    if (position_.x >= minX && position_.x <= maxX && position_.y >= minY && position_.y <= maxY) {
        return true;
    }

    return false;
}


} // namespace game

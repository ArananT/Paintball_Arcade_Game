#include "CircleCollider.h"
#include <cmath>

CircleCollider::CircleCollider(float x, float y, float radius) : x(x), y(y), radius(radius) {}

bool CircleCollider::collidesWith(const CircleCollider& other) const {
    float distanceX = x - other.x;
    float distanceY = y - other.y;
    float distance = sqrt(distanceX * distanceX + distanceY * distanceY);
    return distance <= (radius + other.radius);
}


float CircleCollider::GetRadius() {
    return radius;
}
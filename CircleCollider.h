#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

class CircleCollider {
public:
    float x, y;
    float radius;

    CircleCollider(float x, float y, float radius);

    float GetRadius();

    bool collidesWith(const CircleCollider& other) const;
};

#endif // CIRCLECOLLIDER_H

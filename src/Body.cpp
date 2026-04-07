#include "Body.h"

void Body::AddForce(Vector2 force)
{
    acceleration += Vector2Scale(force, 1.0f / mass);
}

void Body::ExplicitEuler(float dt)
{
    position += velocity * dt;
    velocity += acceleration * dt;
}

void Body::SemiImplicitEuler(float dt)
{
    velocity += acceleration * dt;
    position += velocity * dt;
}
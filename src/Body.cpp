#include "body.h"

void Body::AddForce(Vector2 force, ForceMode forceMode)
{
    // Static bodies never move under any circumstances
    if (bodyType == BodyType::Static) return;

    // Kinematic bodies ignore physics forces and gravity,
    // but can have their velocity set directly (VelocityChange).
    // This lets them be scripted/moved manually.
    if (bodyType == BodyType::Kinematic)
    {
        if (forceMode == ForceMode::VelocityChange)
            velocity += force;
        return;
    }

    // Dynamic bodies respond to all force modes
    switch (forceMode)
    {
    case ForceMode::Force:
        acceleration += force * inverseMass;
        break;
    case ForceMode::Impulse:
        velocity += force * inverseMass;
        break;
    case ForceMode::Acceleration:
        acceleration += force;
        break;
    case ForceMode::VelocityChange:
        velocity += force;
        break;
    }
}

void Body::Draw() const
{
    DrawCircleV(position, size, LIME);
    DrawCircleLinesV(position, size, GREEN);
}
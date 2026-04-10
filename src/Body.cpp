#include "body.h"

void Body::AddForce(Vector2 force, ForceMode forceMode)
{
    if (bodyType != BodyType::Dynamic) return;

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

    acceleration += force / mass;
}

void Body::Draw() const
{
    DrawCircleV(position, size, LIME);
}
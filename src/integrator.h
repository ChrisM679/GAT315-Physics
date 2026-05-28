#pragma once
#include "body.h"

inline void SemiImplicitEuler(Body& body, float dt)
{
    // Static bodies never move
    if (body.bodyType == BodyType::Static) return;

    // Kinematic bodies move by velocity only — no acceleration
    if (body.bodyType == BodyType::Kinematic)
    {
        body.position += body.velocity * dt;
        body.velocity *= (1.0f / (1.0f + body.damping * dt));
        return;
    }

    // Dynamic bodies integrate acceleration into velocity, then velocity into position
    body.velocity += body.acceleration * dt;
    body.position += body.velocity * dt;
    body.velocity *= (1.0f / (1.0f + body.damping * dt));
}
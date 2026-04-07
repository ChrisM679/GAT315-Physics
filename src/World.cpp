#include "World.h"
#include "raylib.h"

World::World() : gravity({ 0, 9.81f })
{
    bodies.reserve(1000);
}

void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::Step(float dt)
{
    for (auto& body : bodies)
        body.acceleration = Vector2{ 0, 0 };

    for (auto& body : bodies)
        body.AddForce(gravity * 100.0f);

    for (auto& body : bodies)
        body.SemiImplicitEuler(dt);

    for (auto& body : bodies)
    {
        if (body.position.x + body.size > GetScreenWidth())
        {
            body.position.x = GetScreenWidth() - body.size;
            body.velocity.x *= -body.restitution;
        }

        if (body.position.x - body.size < 0)
        {
            body.position.x = body.size;
            body.velocity.x *= -body.restitution;
        }

        if (body.position.y + body.size > GetScreenHeight())
        {
            body.position.y = GetScreenHeight() - body.size;
            body.velocity.y *= -body.restitution;
        }
    }
}

void World::Draw() const
{
    for (const auto& body : bodies)
    {
        DrawCircleV(body.position, body.size, LIME);
    }
}
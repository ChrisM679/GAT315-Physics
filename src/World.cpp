#include "world.h"
#include "integrator.h"
#include "effector.h"
#include "collision.h"

Vector2 World::gravity{ 0, 9.81f };

void World::Step(float dt)
{
    for (auto& body : bodies) body.acceleration = gravity * body.gravityScale * 100.0f;

	for (auto& effector : effectors) effector->Apply(bodies);

    /*
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 position = GetMousePosition();
        for (auto& body : bodies)
        {
            Vector2 direction = body.position - position;

            if (Vector2Length(direction) <= 100.0f)
            {
                Vector2 force = Vector2Normalize(direction) * 10000.0f;
            }
        }

        DrawCircleLinesV(position, 100.0f, RED);
    }
    */

    for (auto& body : bodies) SemiImplicitEuler(body, dt);
    UpdateCollision();
}

void World::Draw()
{
    for (auto& body : bodies) body.Draw();
    for (auto& effector : effectors) effector->Draw();
}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);

	// collision
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
		if (body.position.y - body.size < 0)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}
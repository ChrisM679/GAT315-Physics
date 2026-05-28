#include "world.h"
#include "integrator.h"
#include "effector.h"
#include "collision.h"

Vector2 World::gravity{ 0, 9.81f };

void World::Step(float dt)
{
	// Reset acceleration each frame before accumulating forces.
	// Previously this was setting acceleration directly which overwrote
	// anything effectors added, and never cleared it between frames.
	for (auto& body : bodies) body.acceleration = { 0.0f, 0.0f };

	// Apply gravity as an acceleration force (world units, no 100x scale needed)
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);

	// Apply effectors (point, drag, area, etc.)
	for (auto& effector : effectors) effector->Apply(bodies);

	// Integrate
	for (auto& body : bodies) SemiImplicitEuler(body, dt);

	for (int i = 0; i < 4; ++i) UpdateCollision();
}

void World::Draw()
{
	for (const auto& body : bodies) body.Draw();
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

Body* World::GetBodyIntersect(const Vector2 position)
{
	for (auto& body : bodies)
	{
		if (CheckCollisionPointCircle(position, body.position, body.size))
		{
			return &body;
		}
	}
	return nullptr;
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	// Collision boundaries use world-space units via boundsMin/boundsMax
	for (auto& body : bodies)
	{
		if (body.position.x + body.size > boundsMax.x)
		{
			body.position.x = boundsMax.x - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < boundsMin.x)
		{
			body.position.x = boundsMin.x + body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > boundsMax.y)
		{
			body.position.y = boundsMax.y - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < boundsMin.y)
		{
			body.position.y = boundsMin.y + body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}
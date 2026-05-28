#pragma once

#include "body.h"
#include "collision.h"
#include <vector>

class World
{
public:
	World()
	{
		bodies.reserve(1000);
	}

	void Step(float dt);
	void Draw();

	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	Body* GetBodyIntersect(const Vector2 position);

	static void SetGravity(Vector2 newGravity) { gravity = newGravity; }

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

private:
	void UpdateCollision();

private:
	static Vector2 gravity;

	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };

	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<class Contact> contacts;

};
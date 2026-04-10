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

private:
	void UpdateCollision();

private:
    static Vector2 gravity;

	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
    std::vector<class Contact> contacts;

};
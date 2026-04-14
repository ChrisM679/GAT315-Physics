#pragma once

#include "body.h"
#include <vector>

struct Contact
{
	Body* bodyA;
	Body* bodyB;

	float restitution;
	float depth;
	Vector2 normal;
};

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts);
void SeparateContacts(std::vector<Contact>& contacts);
void ResolveContacts(std::vector<Contact>& contacts);
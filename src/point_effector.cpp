#include "point_effector.h"

void PointEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		Vector2 direction = Vector2Subtract(body->position, position);
		float dist = Vector2Length(direction);

		if (dist < 0.001f) continue;

		Vector2 force = Vector2Scale(Vector2Normalize(direction), forceMagnitude);
		body->AddForce(force);
	}
}

void PointEffector::Draw()
{
	Effector::Draw();
	DrawCircleLinesV(position, size, WHITE);
}
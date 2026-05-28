#include "drag_effector.h"

void DragEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		Vector2 force = Vector2Scale(Vector2Negate(body->velocity), drag);
		body->AddForce(force);
	}
}

void DragEffector::Draw()
{
	Effector::Draw();
	DrawCircleLinesV(position, size, BLUE);
}
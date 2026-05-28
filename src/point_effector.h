#pragma once
#include "effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(const Vector2& position, float size, float forceMagnitude) :
		Effector(position, size),
		forceMagnitude(forceMagnitude) { }

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	float forceMagnitude;
};
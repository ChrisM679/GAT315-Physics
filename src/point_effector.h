#pragma once

#include "effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(const Vector2& position, float size, float forceMagnitude) : 
		position(position), 
		size(size), 
		forceMagnitude(forceMagnitude) 
	{ }

	void Apply(std::vector<Body>& bodies) override;
	void Draw();

private:
	Vector2 position{ 0,0 };
	float size = 50.0f;
	float forceMagnitude = 100.0f;
};
#pragma once
#include "effector.h"

class GravitationalEffector : public Effector
{
public:
	GravitationalEffector(const Vector2& position, float size, float strength) :
		Effector(position, size),
		strength(strength) { }

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	float strength;
};
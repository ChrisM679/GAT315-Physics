#pragma once

#include <vector>
#include "body.h"

class Effector
{
public:
    virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw() {}
};
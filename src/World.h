#pragma once

#include "Body.h"
#include <vector>

struct World
{
    std::vector<Body> bodies;
    Vector2 gravity;

    World();

    void AddBody(const Body& body);
    void Step(float dt);
    void Draw() const;
};
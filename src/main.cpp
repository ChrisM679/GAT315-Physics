/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"
#include <vector>
#include <string>

#include "body.h"
#include "random.h"
#include "integrator.h"
#include "world.h"
#include "point_effector.h"
#include "gravitation_effector.h"

int main()
{

    SetRandomSeed(5);

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1920, 1080, "Hello Raylib");

    SearchAndSetResourceDir("resources");
    Texture wabbit = LoadTexture("wabbit_alpha.png");

	//SetTargetFPS(60);

    World world;
	//world.AddEffector(new PointEffector(Vector2{ 400,300 }, 100.0f, 500.0f));
	//world.AddEffector(new GravitationalEffector(100000.0f));

	float timeAccum = 0.0f;
	float fiexedTimeStep = 1.0f / 60.0f;

    // Game loop
    while (!WindowShouldClose()) // Run the loop until the user presses ESCAPE or presses the Close button on the window
    {
        float dt = GetFrameTime();

        // Spawn bodies on left mouse click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || 
            (IsKeyDown(KEY_GRAVE) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
        {
            Body body;

			body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;

            body.position = GetMousePosition();

            float angle = GetRandomFloat() * 2.0f * PI;
            Vector2 direction = { cosf(angle), sinf(angle) };

			//body.AddForce(direction * (50.0f + GetRandomFloat(300.0f)), ForceMode::VelocityChange);

            body.size = 5.0f + GetRandomFloat(20.0f);
            body.restitution = 0.5f + GetRandomFloat(0.5f);
            body.mass = body.size;
			body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
            body.gravityScale = 0.0f;
			body.damping = 0.1f;

            world.AddBody(body);
        }
        // Update the world
		timeAccum += dt;

        while (timeAccum >= fiexedTimeStep)
        {
            world.Step(fiexedTimeStep);
            timeAccum -= fiexedTimeStep;
        }

        // Drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(BLACK);

        // Draw some text using the default font
		std::string fpsText = "FPS: ";
        fpsText += std::to_string(GetFPS());
        DrawText(fpsText.c_str(), 40, 40, 20, WHITE);

        // Draw our texture to the screen
        //DrawTexture(wabbit, 400, 200, WHITE);

        // Draw the world
        world.Draw();

        // End the frame and get ready for the next one (display frame, poll input, etc...)
        EndDrawing();
    }

    // Cleanup
    // Unload our texture so it can be cleaned up
    UnloadTexture(wabbit);

    // Destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}

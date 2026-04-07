/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"
#include "World.h"
#include "Random.h"
#include <cmath>

int main()
{
    // Initialize the world
    World world;

    SetRandomSeed(5);

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1920, 1080, "Hello Raylib");

    SearchAndSetResourceDir("resources");
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    // Game loop
    while (!WindowShouldClose()) // Run the loop until the user presses ESCAPE or presses the Close button on the window
    {
        float dt = GetFrameTime();

        // Spawn bodies on left mouse click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_GRAVE) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
        {
            Body body;
            body.position = GetMousePosition();

            float angle = GetRandomFloat() * 2.0f * PI;
            Vector2 direction = { cosf(angle), sinf(angle) };

            body.velocity = direction * (50.0f + GetRandomFloat(300.0f));
            body.acceleration = Vector2{ 0, 0 };
            body.size = 5.0f + GetRandomFloat(20.0f);
            body.restitution = 0.5f + GetRandomFloat(0.5f);
            body.mass = 1.0f;

            world.AddBody(body);
        }

        // Apply forces on right mouse button hold
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 position = GetMousePosition();
            for (auto& body : world.bodies)
            {
                Vector2 direction = position - body.position;
                if (Vector2Length(direction) <= 100.0f)
                {
                    Vector2 force = Vector2Normalize(direction) * 10000.0f;
                    body.AddForce(force);
                }
            }
            DrawCircleLinesV(position, 100, ORANGE);
        }

        // Update the world
        world.Step(dt);

        // Drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(BLACK);

        // Draw some text using the default font
        DrawText("Hello Raylib", 200, 200, 20, WHITE);

        // Draw our texture to the screen
        DrawTexture(wabbit, 400, 200, WHITE);

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

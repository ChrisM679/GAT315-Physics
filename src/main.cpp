#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <string>

#include "Body.h"
#include "World.h"
#include "World_Camera.h"
#include "Random.h"
#include "Point_Effector.h"
#include "Gravitation_Effector.h"
#include "Area_Effector.h"
#include "Drag_Effector.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);

GuiPhysicsState state;

int main()
{
	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	World world;
	// Camera centered on screen, 5 world units as half-height
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);

	// Set world bounds: bottom-left screen corner -> top-right screen corner in world units
	world.SetBounds(
		world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }),
		world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 })
	);

	GuiLoadStyle("raygui/styles/cyber");

	float timeAccum = 0.0f;

	// Get GUI state
	state = InitGuiPhysics();

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	// Place initial effectors using world coordinates
	//world.AddEffector(new PointEffector(world_camera.ScreenToWorld({ 300, 200 }), 2.0f, 10.0f));
	//world.AddEffector(new GravitationalEffector(world_camera.ScreenToWorld({ 900, 600 }), 2.0f, 300.0f));
	//world.AddEffector(new AreaEffector(world_camera.ScreenToWorld({ 900, 200 }), 2.0f, 0, 10.0f));
	//world.AddEffector(new DragEffector(world_camera.ScreenToWorld({ 300, 600 }), 2.0f, 1.0f));

	// game loop
	while (!WindowShouldClose())
	{
		float fixedTimeStep = 1.0f / state.FPSValue;
		float dt = fminf(GetFrameTime(), 0.1f);

		if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
		if (IsKeyPressed(KEY_TAB))   state.PhysicsPanelActive = !state.PhysicsPanelActive;

		// Gravity: positive Y = up in world space (camera flips Y)
		World::SetGravity(Vector2{ 0.0f, state.GravityValue });

		bool mouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(GetMousePosition(), Rectangle{ state.anchor02.x, state.anchor02.y, 304, 664 });
		if (!mouseOverGui)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
				(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(world, world_camera);
				}
				else
				{
					AddBody(world, world_camera);
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Vector2 mouseWorld = world_camera.ScreenToWorld(GetMousePosition());
			for (auto& body : world.GetBodies()) {
				Vector2 direction = mouseWorld - body.position;
				if (Vector2Length(direction) <= 2.0f) {
					Vector2 force = Vector2Normalize(direction) * 100.0f;
					body.AddForce(force);
				}
			}
		}

		if (state.SimulateActive) {
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}

		// DRAW
		BeginDrawing();
		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";
		fpsText += std::to_string(GetFPS());
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		world_camera.Begin();	// set world camera transform
		world.Draw();			// draw using world camera transform
		world_camera.End();		// remove world camera transform

		GuiPhysics(&state);

		EndDrawing();
	}

	// cleanup
	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;
	body.bodyType = (BodyType)state.BodyTypeActive;

	// Convert mouse screen position to world coordinates
	body.position = camera.ScreenToWorld(GetMousePosition());

	float angle = GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce(direction * state.BodyVelocityValue, ForceMode::VelocityChange);

	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;

	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	// Convert mouse screen position to world coordinates
	Vector2 position = camera.ScreenToWorld(GetMousePosition());
	float size = state.EffectorSizeValue; // use effector size, not body size

	Effector* effector = nullptr;
	switch ((EffectorType)state.EffectorTypeActive)
	{
	case EffectorType::Gravitation:
		effector = new GravitationalEffector(position, size, state.EffectorForceValue);
		break;
	case EffectorType::Point:
		effector = new PointEffector(position, size, state.EffectorForceValue);
		break;
	case EffectorType::Area:
		effector = new AreaEffector(position, size, state.EffectorAngleValue, state.EffectorForceValue);
		break;
	case EffectorType::Drag:
		effector = new DragEffector(position, size, state.EffectorForceValue);
		break;
	}

	if (effector) world.AddEffector(effector);
}
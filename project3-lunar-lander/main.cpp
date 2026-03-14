/**
* Author: Bobby Li
* Assignment: Lunar Lander
* Date due: 03/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#6bc1eb";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  TILES_DIMENSIONS = { 100.0f, 100.0f },
                  ATLAS_DIMENSIONS = { 4, 4 };
constexpr float   FIXED_TIMESTEP   = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Entity *gChicken = nullptr;
Entity *gWater   = nullptr;
Entity *gGrass   = nullptr;
Entity *gPlane   = nullptr;

Font defaultFont;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chicken Lander");

    defaultFont = GetFontDefault();

    std::map<Direction, std::vector<int>> chickenAnimationAtlas = {
        {LEFT,  { 12, 13, 14, 15 }},
        {RIGHT, { 12, 13, 14, 15 }}
    };

    // Assets from https://pixelplant.itch.io/chicken-sprite-sheet?download
    gChicken = new Entity(
        {1000.0f, 200.0f},
        {80.0f, 80.0f},
        "assets/chicken.png",
        ATLAS,
        ATLAS_DIMENSIONS,
        chickenAnimationAtlas,
        PLAYER
    );

    gChicken->setColliderDimensions({
        gChicken->getScale().x / 1.4f,
        gChicken->getScale().y / 1.4f
    });
    gChicken->setDirection(LEFT);

    // Assets from https://cccam.itch.io/some-32x-tiles
    gWater = new Entity[10];
    for (int i = 0; i < 10; i++) {
        gWater[i].setTexture("assets/water.png");
        gWater[i].setEntityType(WATER);
        gWater[i].setScale(TILES_DIMENSIONS);
        gWater[i].setColliderDimensions(TILES_DIMENSIONS);
        if (i < 5) gWater[i].setPosition({50.0f + i * 100.0f, 870.0f});
        else gWater[i].setPosition({750.0f + (i-4) * 100.0f, 870.0f});
    }

    // Same as above
    gGrass = new Entity[6];
    for (int i = 0; i < 6; i++) {
        gGrass[i].setTexture("assets/grass.png");
        gGrass[i].setEntityType(GRASS);
        gGrass[i].setScale(TILES_DIMENSIONS);
        gGrass[i].setColliderDimensions(TILES_DIMENSIONS);
        if (i < 3) gGrass[i].setPosition({550.0f + i * 100.0f, 850.0f});
        else gGrass[i].setPosition({1250.0f + (i-2) * 100.0f, 850.0f});
    }

    // Assets from https://kenney-assets.itch.io/pixel-shmup
    gPlane = new Entity(
        {0.0f, 400.0f},
        {120.0f, 120.0f},
        "assets/plane.png",
        PLANE
    );
    gPlane->setColliderDimensions({gPlane->getScale().x / 2.0f,
                                   gPlane->getScale().y / 2.0f});
    gPlane->moveRight();

    SetTargetFPS(FPS);
}

void processInput()
{
    gChicken->resetMovement();

    if      (IsKeyDown(KEY_A)) gChicken->rotateClockwise();
    else if (IsKeyDown(KEY_D)) gChicken->rotateCounterClockwise();

    if (IsKeyDown(KEY_S) && gChicken->getEnergy() != 0) {
        gChicken->accelerate();
    }
    else gChicken->decelerate();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks     = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gChicken->update(FIXED_TIMESTEP, gWater, 10, gGrass, 6, gPlane, 1);
        gPlane->update(FIXED_TIMESTEP, gWater, 10, gGrass, 6, gPlane, 1);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    DrawText("Energy", 50, 50, 50, WHITE);
    DrawText(TextFormat("%d", gChicken->getEnergy()), 250, 50, 50, WHITE);
    DrawText("A = Rotate right, D = Rotate left, S = Fly", 600, 50, 30, WHITE);
    DrawText("Stay away from water, you can't swim", 600, 100, 30, WHITE);

    gChicken->render();
    gPlane->render();
    for (int i = 0; i < 10; i++) {
        gWater[i].render();
    }
    for (int i = 0; i < 6; i++) {
        gGrass[i].render();
    }

    if (!gChicken->isActive()) {
        gPlane->deactivate();
        if (gChicken->getChickenStatus() == WIN) {
            DrawText("Landing Successful", 550, 500, 50, WHITE);
        } else if (gChicken->getChickenStatus() == LOSE) {
            DrawText("Landing Failed", 550, 500, 50, WHITE);
        }
    }

    EndDrawing();
}

void shutdown()
{
    delete gChicken;
    delete gPlane;
    delete[] gWater;
    delete[] gGrass;

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}

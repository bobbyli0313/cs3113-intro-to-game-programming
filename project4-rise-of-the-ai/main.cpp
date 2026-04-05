/**
* Author: Bobby Li
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/LevelC.h"
#include "CS3113/Win.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 6;

constexpr Vector2 ORIGIN       = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Menu *gMenu = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
Lose *gLose = nullptr;
Win *gWin = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    gCurrentScene = scene;
    gCurrentScene->initialise();
    gCamera.target = gCurrentScene->getState().adrian->getPosition();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2067: A Space Odyssey");
    InitAudioDevice();

    gMenu   = new Menu(ORIGIN, "#63b2cc");
    gLevelA = new LevelA(ORIGIN, "#63b2cc");
    gLevelB = new LevelB(ORIGIN, "#63b2cc");
    gLevelC = new LevelC(ORIGIN, "#63b2cc");
    gLose   = new Lose(ORIGIN, "#63b2cc");
    gWin    = new Win(ORIGIN, "#63b2cc");

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gLose);
    gLevels.push_back(gWin);

    switchToScene(gLevels[0]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput() 
{
    gCurrentScene->getState().adrian->resetMovement();

    // Menu, enter to start game
    if (IsKeyDown(KEY_ENTER) && gCurrentScene->getState().map == nullptr) {
        switchToScene(gLevelA);
    }

    // Win/Lose, r to restart game
    if (IsKeyDown(KEY_R) && gCurrentScene->getState().map == nullptr) {
        switchToScene(gLevelA);
    }

    if      (IsKeyDown(KEY_ONE))   switchToScene(gLevelA);
    else if (IsKeyDown(KEY_TWO))   switchToScene(gLevelB);
    else if (IsKeyDown(KEY_THREE)) switchToScene(gLevelC);

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().adrian->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().adrian->moveRight();

    if (IsKeyPressed(KEY_W) && 
        gCurrentScene->getState().adrian->isCollidingBottom())
    {
        gCurrentScene->getState().adrian->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().adrian->getMovement()) > 1.0f) 
        gCurrentScene->getState().adrian->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
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
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        Vector2 currentPlayerPosition = {gCurrentScene->getState().adrian->getPosition().x, ORIGIN.y };
        panCamera(&gCamera, &currentPlayerPosition);
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);
    if (gCurrentScene->getState().map != nullptr) { // If scene has a map
        DrawText(TextFormat("%d", gCurrentScene->getState().adrian->getLives()), 
                            // Display lives relative to adrian's position
                            gCurrentScene->getState().adrian->getPosition().x,
                            gCurrentScene->getState().adrian->getPosition().y - 100, 
                            50, WHITE);
    }

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() 
{
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete gLose;
    delete gWin;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0)
        {
            int remainLives = gCurrentScene->getState().adrian->getLives();
            int id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
            gCurrentScene->getState().adrian->setLives(remainLives);
        }

        render();
    }

    shutdown();

    return 0;
}
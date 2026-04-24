/**
* Author: Bobby Li
* Assignment: Student's Choice
* Date due: 2026-04-24, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/LevelC.h"
#include "CS3113/Win.h"
#include "CS3113/ShaderProgram.h"

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
ShaderProgram gShader;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Menu *gMenu = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
Lose *gLose = nullptr;
Win *gWin = nullptr;

Vector2 gLightPosition = { 0.0f, 0.0f };

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
    PlaySound(gCurrentScene->getState().nextLevelSound);
    gCamera.target = gCurrentScene->getState().character->getPosition();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Introduction to Maze Escaping");
    InitAudioDevice();

    gMenu   = new Menu(ORIGIN, "#152861");
    gLevelA = new LevelA(ORIGIN, "#040404");
    gLevelB = new LevelB(ORIGIN, "#040404");
    gLevelC = new LevelC(ORIGIN, "#040404");
    gLose   = new Lose(ORIGIN, "#26225f");
    gWin    = new Win(ORIGIN, "#689dae");

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gLose);
    gLevels.push_back(gWin);

    switchToScene(gLevels[0]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.2f;

    gShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");

    SetTargetFPS(FPS);
}

void processInput() 
{
    gCurrentScene->getState().character->resetMovement();

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

    if (gCurrentScene->getState().map != nullptr &&
        gCurrentScene->getState().ai->getAIState() != ENGAGE) {
        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().character->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().character->moveRight();
        else if (IsKeyDown(KEY_W)) gCurrentScene->getState().character->moveUp();
        else if (IsKeyDown(KEY_S)) gCurrentScene->getState().character->moveDown();
    }

    if (GetLength(gCurrentScene->getState().character->getMovement()) > 1.0f) 
        gCurrentScene->getState().character->normaliseMovement();

    if (gCurrentScene->getState().map != nullptr && 
        gCurrentScene->getState().ai->getAIState() == ENGAGE) {
            if (gCurrentScene == gLevelA) {
                if (IsKeyDown(KEY_G)) {
                    PlaySound(gCurrentScene->getState().rightAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                } else if (IsKeyDown(KEY_E) || IsKeyDown(KEY_F)) {
                    PlaySound(gCurrentScene->getState().wrongAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                    gCurrentScene->getState().character->setLives(
                        gCurrentScene->getState().character->getLives() - 1
                    );
                }
            } else if (gCurrentScene == gLevelB) {
                if (IsKeyDown(KEY_F)) {
                    PlaySound(gCurrentScene->getState().rightAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                } else if (IsKeyDown(KEY_E) || IsKeyDown(KEY_G)) {
                    PlaySound(gCurrentScene->getState().wrongAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                    gCurrentScene->getState().character->setLives(
                        gCurrentScene->getState().character->getLives() - 1
                    );
                }
            } else if (gCurrentScene == gLevelC) {
                if (IsKeyDown(KEY_E)) {
                    PlaySound(gCurrentScene->getState().rightAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                } else if (IsKeyDown(KEY_F) || IsKeyDown(KEY_G)) {
                    PlaySound(gCurrentScene->getState().wrongAnswerSound);
                    gCurrentScene->getState().ai->setAIState(IDLE);
                    gCurrentScene->getState().ai->deactivate();
                    gCurrentScene->getState().character->setLives(
                        gCurrentScene->getState().character->getLives() - 1
                    );
                }
            }
        }

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

        Vector2 currentPlayerPosition = {gCurrentScene->getState().character->getPosition().x,
                                         gCurrentScene->getState().character->getPosition().y};
        panCamera(&gCamera, &currentPlayerPosition);

        gLightPosition = gCurrentScene->getState().character->getPosition();
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);
    gShader.begin();

    gShader.setVector2("lightPosition", gLightPosition);
    gShader.setInt("isEffectOn", 
                    gCurrentScene->getState().character->getShaderStatus());
    gCurrentScene->render();

    gShader.end();
    EndMode2D();

    if (gCurrentScene->getState().map != nullptr && gCurrentScene->getState().ai->getAIState() == ENGAGE) {
        DrawText(gCurrentScene->getState().question, 200, 100, 30, WHITE);
        DrawText(gCurrentScene->getState().answerE, 200, 200, 30, WHITE);
        DrawText(gCurrentScene->getState().answerF, 200, 300, 30, WHITE);
        DrawText(gCurrentScene->getState().answerG, 200, 400, 30, WHITE);
    }
    
    if (gCurrentScene->getState().map != nullptr) {
        DrawText(TextFormat("%d", gCurrentScene->getState().character->getLives()), 100, 100, 50, WHITE);
    }

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

    gShader.unload();

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
            int remainLives = gCurrentScene->getState().character->getLives();
            int id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
            gCurrentScene->getState().character->setLives(remainLives);
        }

        render();
    }

    shutdown();

    return 0;
}
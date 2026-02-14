/**

* Author: Bobby Li

* Assignment: Simple 2D Scene

* Date due: 02/14/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/cs3113.h"
#include "math.h"

enum Background     { DAY, NIGHT };
enum boyDirection   { LEFT, RIGHT };
enum crowdDirection { FORWARD, BACKWARD };


// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60;
constexpr float LIMIT_ANGLE   = 10.0f,
                RADIUS        = 100.0f;

constexpr char BG_COLOR1[] = "#598267",
               BG_COLOR2[] = "#8b705c";

constexpr char CROWD_FP[]  = "assets/crowd.png",
               BOY_FP[]    = "assets/boy.png",
               SHEEP1_FP[] = "assets/sheep1.png",
               SHEEP2_FP[] = "assets/sheep2.png";

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },     
                  CROWD_FORWARD_LIMIT  = { 450.0f, 300.0f },
                  CROWD_BACKWARD_LIMIT = { 150.0f, 100.0f };

// Global Variables
AppStatus gAppStatus           = RUNNING;
Background gBackground         = DAY;
boyDirection gBoyDirection     = RIGHT;
crowdDirection gCrowdDirection = FORWARD;

float   gScaleFactor   = 0.0f,
        gBoyAngle      = 0.0f,
        gPulseTime     = 0.0f,
        gPreviousTicks = 0.0f,
        gFrameElapsed  = 0.0f;

Vector2 gCrowdPosition  = { 150.0f, 100.0f };
Vector2 gCrowdScale     = { 300.0f, 200.0f };
Vector2 gBoyPosition    = { 400.0f, 400.0f };
Vector2 gSheep1Position = { 700.0f, 400.0f };
Vector2 gSheep2Position = { 800.0f, 450.0f };

Texture2D crowd;
Texture2D boy;
Texture2D sheep1;
Texture2D sheep2;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Boy Who Cried Wolf");

    crowd  = LoadTexture(CROWD_FP);
    boy    = LoadTexture(BOY_FP);
    sheep1 = LoadTexture(SHEEP1_FP);
    sheep2 = LoadTexture(SHEEP2_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 3.0f * deltaTime;

    // Crowd -----------------------------------------------------
    // Change scale based on time
    gCrowdScale = {
        300.0f + 15.0f * cos(gPulseTime),
        200.0f + 10.0f * cos(gPulseTime)
    };

    // Horizontal movement
    gCrowdPosition.x += deltaTime * 
    (gCrowdDirection == FORWARD) ? 0.9f : -0.9f;

    // Vertical movement
    gCrowdPosition.y += deltaTime * 
    (gCrowdDirection == FORWARD) ? 0.6f : -0.6f;

    // Move backward and change to night if reaches forward limit
    if (gCrowdPosition.x > CROWD_FORWARD_LIMIT.x) {
        gCrowdDirection = BACKWARD;
        gBackground = NIGHT;
    }

    // Move forward and change to day if reaches backward limit
    if (gCrowdPosition.x < CROWD_BACKWARD_LIMIT.x) {
        gCrowdDirection = FORWARD;
        gBackground = DAY;
    }

    // Boy -------------------------------------------------------
    // Rotate left and right
    gBoyAngle += deltaTime * (gBoyDirection == RIGHT) ? 1.0f : -1.0f;
    if (gBoyAngle > LIMIT_ANGLE) gBoyDirection = LEFT;
    else if (gBoyAngle < -LIMIT_ANGLE) gBoyDirection = RIGHT;

    // Sheep1 ----------------------------------------------------
    // Move diagonally based on cosine
    gSheep1Position.x += 3.0f * cos(gPulseTime);
    gSheep1Position.y += 3.0f * cos(gPulseTime);

    // Sheep2 ----------------------------------------------------
    // Orbits around sheep1
    gSheep2Position.x = gSheep1Position.x + RADIUS * cos(gPulseTime);
    gSheep2Position.y = gSheep1Position.y + RADIUS * sin(gPulseTime);
}

void render()
{
    BeginDrawing();
    ClearBackground((gBackground == DAY) ? ColorFromHex(BG_COLOR1) :
    ColorFromHex(BG_COLOR2));

    // Crowd -----------------------------------------------------
    Rectangle crowdTextureArea = {
        0.0f, 0.0f, 
        static_cast<float>(crowd.width), 
        static_cast<float>(crowd.height)
    };

    Rectangle crowdDestinationArea = {
        gCrowdPosition.x - static_cast<float>(crowd.width),
        gCrowdPosition.y - static_cast<float>(crowd.height),
        static_cast<float>(gCrowdScale.x),
        static_cast<float>(gCrowdScale.y)
    };

    Vector2 crowdOriginOffset = {
        static_cast<float>(gCrowdScale.x) / 2.0f,
        static_cast<float>(gCrowdScale.y) / 2.0f
    };

    DrawTexturePro(
        crowd,
        crowdTextureArea,
        crowdDestinationArea,
        crowdOriginOffset,
        0.0f,
        WHITE
    );

    // Boy -------------------------------------------------------
    Rectangle boyTextureArea = {
        0.0f, 0.0f, 
        static_cast<float>(boy.width), 
        static_cast<float>(boy.height)
    };

    Rectangle boyDestinationArea = {
        gBoyPosition.x - static_cast<float>(boy.width),
        gBoyPosition.y - static_cast<float>(boy.height),
        static_cast<float>(boy.width),
        static_cast<float>(boy.height)
    };

    Vector2 boyOriginOffset = {
        static_cast<float>(boy.width) / 2.0f,
        static_cast<float>(boy.height) / 2.0f
    };

    DrawTexturePro(
        boy,
        boyTextureArea,
        boyDestinationArea,
        boyOriginOffset,
        gBoyAngle,
        WHITE
    );

    // Sheep1 ----------------------------------------------------
    Rectangle sheep1TextureArea = {
        0.0f, 0.0f, 
        static_cast<float>(sheep1.width), 
        static_cast<float>(sheep1.height)
    };

    Rectangle sheep1DestinationArea = {
        gSheep1Position.x - static_cast<float>(sheep1.width),
        gSheep1Position.y - static_cast<float>(sheep1.height),
        static_cast<float>(sheep1.width),
        static_cast<float>(sheep1.height)
    };

    Vector2 sheep1OriginOffset = {
        static_cast<float>(sheep1.width) / 2.0f,
        static_cast<float>(sheep1.height) / 2.0f
    };

    DrawTexturePro(
        sheep1,
        sheep1TextureArea,
        sheep1DestinationArea,
        sheep1OriginOffset,
        0.0f,
        WHITE
    );

    // Sheep2 ----------------------------------------------------
    Rectangle sheep2TextureArea = {
        0.0f, 0.0f, 
        static_cast<float>(sheep2.width), 
        static_cast<float>(sheep2.height)
    };

    Rectangle sheep2DestinationArea = {
        gSheep2Position.x - static_cast<float>(sheep2.width),
        gSheep2Position.y - static_cast<float>(sheep2.height),
        static_cast<float>(sheep2.width),
        static_cast<float>(sheep2.height)
    };

    Vector2 sheep2OriginOffset = {
        static_cast<float>(sheep2.width) / 2.0f,
        static_cast<float>(sheep2.height) / 2.0f
    };

    DrawTexturePro(
        sheep2,
        sheep2TextureArea,
        sheep2DestinationArea,
        sheep2OriginOffset,
        0.0f,
        WHITE
    );

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
    UnloadTexture(crowd);
    UnloadTexture(boy);
    UnloadTexture(sheep1);
    UnloadTexture(sheep2);
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
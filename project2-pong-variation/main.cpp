/**
* Author: Bobby Li
* Assignment: Pong Clone
* Date due: 02/28/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include "math.h"

enum ballDirection { LEFT, RIGHT };
enum gameMode      { ONEPLAYER, TWOPLAYER };
enum gameOver      { NO, P1WON, P2WON };
enum numOfBalls    { ONE, TWO, THREE };

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600,
                SCREEN_HEIGHT = 900,
                FPS           = 120;
          float SPEED         = 400.0f,
                AUTO_LIMIT_HI = 250.0f,
                AUTO_LIMIT_LO = 650.0f,
                BALL_OFFSET   = 150.0f;

constexpr char BG_COLOR[] = "#405b89";

// Images from Flipline Studio Wiki
constexpr char PLAYER1_FP[] = "assets/marty.png",
               PLAYER2_FP[] = "assets/papa.png",
               BURGER_FP[]  = "assets/burger.png";

constexpr Vector2 ORIGIN        = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  leftBallMove  = { 1.0f, 1.0f },
                  rightBallMove = { -1.0f, 1.0f };

// Global Variables
AppStatus gAppStatus          = RUNNING;
ballDirection gBall1Direction = RIGHT;
ballDirection gBall2Direction = LEFT;
ballDirection gBall3Direction = RIGHT;

gameMode gGameMode     = TWOPLAYER;
gameOver gGameOver     = NO;
numOfBalls gNumOfBalls = ONE;

float gPreviousTicks = 0.0f;
float gAngle = 0.0f;

int gPlayer1Score = 0;
int gPlayer2Score = 0;

Vector2 gPlayer1Position = { ORIGIN.x - 550.0f, ORIGIN.y },
        gPlayer1Movement = { 0.0f, 0.0f },
        gPlayer1Scale    = { 100.0f, 250.0f };

Vector2 gPlayer2Position = { ORIGIN.x + 500.0f, ORIGIN.y },
        gPlayer2Movement = { 0.0f, 1.0f },
        gPlayer2Scale    = { 100.0f, 250.0f };

Vector2 gBall1Position = { ORIGIN.x, ORIGIN.y },
        gBall1Movement = rightBallMove,
        gBall1Scale    = { 50.0f, 50.0f };

Vector2 gBall2Position = { ORIGIN.x, ORIGIN.y + BALL_OFFSET },
        gBall2Movement = leftBallMove,
        gBall2Scale    = { 50.0f, 50.0f };

Vector2 gBall3Position = { ORIGIN.x, ORIGIN.y - BALL_OFFSET },
        gBall3Movement = rightBallMove,
        gBall3Scale    = { 50.0f, 50.0f };

Texture2D player1;
Texture2D player2;
Texture2D ball1;
Texture2D ball2;
Texture2D ball3;

Font defaultFont;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB);
void ballMove(Vector2 *ballPos, const Vector2 *ballMovement, 
              const float deltaTime);
void processScore(Vector2 *ballPos, Vector2 *ballMovement, 
                  ballDirection *ballDirection);
void processCollide(Vector2 *ballPos, const Vector2 *ballScale, 
                    Vector2 *ballMovement);
void processBallCollide(Vector2 *ballAPosition, const Vector2 *ballAScale, 
                        Vector2 *ballAMovement, Vector2 *ballBPosition, const 
                        Vector2 *ballBScale, Vector2 *ballBMovement);

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong's Burgeria");

    defaultFont = GetFontDefault();

    player1 = LoadTexture(PLAYER1_FP);
    player2 = LoadTexture(PLAYER2_FP);
    ball1   = LoadTexture(BURGER_FP);
    ball2   = LoadTexture(BURGER_FP);
    ball3   = LoadTexture(BURGER_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (IsKeyDown(KEY_T)) {
        gGameMode = (gGameMode == TWOPLAYER) ? ONEPLAYER : TWOPLAYER;
    }

    if (IsKeyDown(KEY_ONE)) {
        gNumOfBalls = ONE;
        gBall1Position = { ORIGIN.x, ORIGIN.y };
    }
    else if (IsKeyDown(KEY_TWO)) {
        gNumOfBalls = TWO;
        gBall1Position = { ORIGIN.x, ORIGIN.y };
        gBall2Position = { ORIGIN.x, ORIGIN.y + BALL_OFFSET };
    }
    else if (IsKeyDown(KEY_THREE)) {
        gNumOfBalls = THREE;
        gBall1Position = { ORIGIN.x, ORIGIN.y };
        gBall2Position = { ORIGIN.x, ORIGIN.y + BALL_OFFSET };
        gBall3Position = { ORIGIN.x, ORIGIN.y - BALL_OFFSET };
    }

    gPlayer1Movement = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W) && 
        gPlayer1Position.y - gPlayer1Scale.y / 2.0f > 0.0f) {
            gPlayer1Movement.y = -1;
        }
    else if (IsKeyDown(KEY_S) && 
             gPlayer1Position.y + gPlayer1Scale.y / 2.0f < SCREEN_HEIGHT) {
                gPlayer1Movement.y = 1;
             }

    if (gGameMode == TWOPLAYER) gPlayer2Movement = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_UP) && 
        gPlayer2Position.y - gPlayer2Scale.y / 2.0f > 0.0f &&
        gGameMode == TWOPLAYER) {
            gPlayer2Movement.y = -1;
        }
    else if (IsKeyDown(KEY_DOWN) && 
             gPlayer2Position.y + gPlayer2Scale.y / 2.0f < SCREEN_HEIGHT &&
             gGameMode == TWOPLAYER) {
                gPlayer2Movement.y = 1;
             }

    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPlayer1Position.y += SPEED * gPlayer1Movement.y * deltaTime;
    gPlayer2Position.y += SPEED * gPlayer2Movement.y * deltaTime;

    if (gNumOfBalls == ONE) {
        ballMove(&gBall1Position, &gBall1Movement, deltaTime);
        processScore(&gBall1Position, &gBall1Movement, &gBall1Direction);
        processCollide(&gBall1Position, &gBall1Scale, &gBall1Movement);
    }

    if (gNumOfBalls == TWO) {
        ballMove(&gBall1Position, &gBall1Movement, deltaTime);
        processScore(&gBall1Position, &gBall1Movement, &gBall1Direction);
        processCollide(&gBall1Position, &gBall1Scale, &gBall1Movement);

        ballMove(&gBall2Position, &gBall2Movement, deltaTime);
        processScore(&gBall2Position, &gBall2Movement, &gBall2Direction);
        processCollide(&gBall2Position, &gBall2Scale, &gBall2Movement);

        processBallCollide(&gBall1Position, &gBall1Scale, &gBall1Movement, 
                           &gBall2Position, &gBall2Scale, &gBall2Movement);
    }

    if (gNumOfBalls == THREE) {
        ballMove(&gBall1Position, &gBall1Movement, deltaTime);
        processScore(&gBall1Position, &gBall1Movement, &gBall1Direction);
        processCollide(&gBall1Position, &gBall1Scale, &gBall1Movement);

        ballMove(&gBall2Position, &gBall2Movement, deltaTime);
        processScore(&gBall2Position, &gBall2Movement, &gBall2Direction);
        processCollide(&gBall2Position, &gBall2Scale, &gBall2Movement);

        ballMove(&gBall3Position, &gBall3Movement, deltaTime);
        processScore(&gBall3Position, &gBall3Movement, &gBall3Direction);
        processCollide(&gBall3Position, &gBall3Scale, &gBall3Movement);

        processBallCollide(&gBall1Position, &gBall1Scale, &gBall1Movement, 
                           &gBall2Position, &gBall2Scale, &gBall2Movement);
        processBallCollide(&gBall1Position, &gBall1Scale, &gBall1Movement, 
                           &gBall3Position, &gBall3Scale, &gBall3Movement);
        processBallCollide(&gBall2Position, &gBall2Scale, &gBall2Movement, 
                           &gBall3Position, &gBall3Scale, &gBall3Movement);
    }

    if (gGameMode == ONEPLAYER) {
        // Start auto movement of P2
        if (gPlayer2Movement.y == 0.0f) gPlayer2Movement.y = 1.0f;
        if (gPlayer2Position.y < AUTO_LIMIT_HI || gPlayer2Position.y > 
            AUTO_LIMIT_LO) {
            // Make sure within limit
            gPlayer2Movement.y = -gPlayer2Movement.y;
        }
    }

    // Game over
    if (gPlayer1Score == 8 || gPlayer2Score == 8) {
        gGameOver = (gPlayer1Score == 8) ? P1WON : P2WON;

        // Reset balls
        gBall1Position = ORIGIN;
        gBall1Movement.y = 0.0f;
        if (gNumOfBalls == TWO || gNumOfBalls == THREE) {
            gBall2Position = ORIGIN;
            gBall2Movement.y = 0.0f;
            if (gNumOfBalls == THREE) {
                gBall3Position = ORIGIN;
                gBall3Movement.y = 0.0f;
            }
        }

        // Stop P2 auto movement
        if (gGameMode == ONEPLAYER) gPlayer2Movement.y = 0.0f;
    }

}

// Move the ball
void ballMove(Vector2 *ballPos, const Vector2 *ballMovement, 
              const float deltaTime) 
{
    ballPos->x += SPEED * ballMovement->x * deltaTime;
    ballPos->y += SPEED * ballMovement->y * deltaTime;
}

// Determine if a ball reaches score condition
void processScore(Vector2 *ballPos, Vector2 *ballMovement, 
                  ballDirection *ballDirection) 
{
    if (ballPos->x < 100.0f || ballPos->x > 1500.0f) {
        if (ballPos->x < 100.0f) gPlayer2Score += 1;
        if (ballPos->x > 1500.0f) gPlayer1Score += 1;

        // Reset ball
        *ballPos = ORIGIN;
        *ballDirection = (*ballDirection == RIGHT) ? LEFT : RIGHT;
        *ballMovement = (*ballDirection == RIGHT) ? 
                        rightBallMove : leftBallMove;
    }
}

// Determine if a ball collides with wall or player
void processCollide(Vector2 *ballPos, const Vector2 *ballScale, 
                    Vector2 *ballMovement) 
{
    // Collides with wall
    if (ballPos->y - ballScale->y / 2.0f < 0.0f || 
        ballPos->y + ballScale->y / 2.0f > SCREEN_HEIGHT) {
        ballMovement->y *= -1.0f;
    }

    // Collides with P1
    if (isColliding(ballPos, ballScale, &gPlayer1Position, &gPlayer1Scale)) {
        ballMovement->x = fabs(ballMovement->x) + 0.1f;
    }

    // Collides with P2
    if (isColliding(ballPos, ballScale, &gPlayer2Position, &gPlayer2Scale)) {
        ballMovement->x = -fabs(ballMovement->x) - 0.1f;
    }
}

// Determine if balls collide with each other
void processBallCollide(Vector2 *ballAPosition, const Vector2 *ballAScale, 
                        Vector2 *ballAMovement, Vector2 *ballBPosition, const 
                        Vector2 *ballBScale, Vector2 *ballBMovement) 
{
    if (isColliding(ballAPosition, ballAScale, ballBPosition, ballBScale)) {
        ballAMovement->x *= -1.0f;
        ballBMovement->x *= -1.0f;
    }
}

bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB) 
{
    float xDistance = fabs(positionA->x - positionB->x) - 
    ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - 
    ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale)
{
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    DrawTexturePro(
        *texture, 
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOR));

    DrawText(TextFormat("%d", gPlayer1Score), 600, 100, 100, WHITE);
    DrawText(TextFormat("%d", gPlayer2Score), 950, 100, 100, WHITE);

    if (gGameOver == P1WON) {
        DrawText("Player 1 Won", 450, 300, 100, WHITE);
    } else if (gGameOver == P2WON) {
        DrawText("Player 2 Won", 450, 300, 100, WHITE);
    }

    renderObject(&player1, &gPlayer1Position, &gPlayer1Scale);

    renderObject(&player2, &gPlayer2Position, &gPlayer2Scale);

    if (gNumOfBalls == ONE) {
        renderObject(&ball1, &gBall1Position, &gBall1Scale);
    }
    else if (gNumOfBalls == TWO) {
        renderObject(&ball1, &gBall1Position, &gBall1Scale);
        renderObject(&ball2, &gBall2Position, &gBall2Scale);
    }
    else if (gNumOfBalls == THREE) {
        renderObject(&ball1, &gBall1Position, &gBall1Scale);
        renderObject(&ball2, &gBall2Position, &gBall2Scale);
        renderObject(&ball3, &gBall3Position, &gBall3Scale);
    }

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
    UnloadTexture(player1);
    UnloadTexture(player2);
    UnloadTexture(ball1);
    UnloadTexture(ball2);
    UnloadTexture(ball3);
    UnloadFont(defaultFont);
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
#include "Win.h"

Win::Win()                                      : Scene { {0.0f}, nullptr   } {}
Win::Win(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Win::~Win() { shutdown(); }

void Win::initialise()
{
   mGameState.adrian = new Entity();
}

void Win::update(float deltaTime)
{

}

void Win::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("You Win", -400, 100, 70, WHITE);
   DrawText("Press R to Restart", -250, 400, 50, WHITE);
}

void Win::shutdown()
{
   delete mGameState.adrian;
}
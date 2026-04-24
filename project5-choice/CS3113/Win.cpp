#include "Win.h"

Win::Win()                                      : Scene { {0.0f}, nullptr   } {}
Win::Win(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Win::~Win() { shutdown(); }

void Win::initialise()
{
   mGameState.character = new Entity();
}

void Win::update(float deltaTime)
{

}

void Win::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("You Win", -400, -200, 70, WHITE);
   DrawText("Sterling Will be Proud of You", -400, 0, 30, WHITE);
   DrawText("Press R to Restart", -400, 100, 30, WHITE);
}

void Win::shutdown()
{
   delete mGameState.character;
}
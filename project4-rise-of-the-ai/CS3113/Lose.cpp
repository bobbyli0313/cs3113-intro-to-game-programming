#include "Lose.h"

Lose::Lose()                                      : Scene { {0.0f}, nullptr   } {}
Lose::Lose(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Lose::~Lose() { shutdown(); }

void Lose::initialise()
{
   mGameState.adrian = new Entity();
}

void Lose::update(float deltaTime)
{

}

void Lose::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("You Lose", -400, 100, 70, WHITE);
   DrawText("Press R to Restart", -250, 400, 50, WHITE);
}

void Lose::shutdown()
{
   delete mGameState.adrian;
}
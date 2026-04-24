#include "Lose.h"

Lose::Lose()                                      : Scene { {0.0f}, nullptr   } {}
Lose::Lose(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Lose::~Lose() { shutdown(); }

void Lose::initialise()
{
   mGameState.character = new Entity();
}

void Lose::update(float deltaTime)
{

}

void Lose::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("You Lose", -400, -200, 70, WHITE);
   DrawText("The F Students are Inventors", -400, 0, 30, WHITE);
   DrawText("Press R to Restart", -400, 100, 30, WHITE);
}

void Lose::shutdown()
{
   delete mGameState.character;
}
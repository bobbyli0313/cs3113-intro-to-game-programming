#include "Menu.h"

Menu::Menu()                                      : Scene { {0.0f}, nullptr   } {}
Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
   // Need a target for the camera
   mGameState.adrian = new Entity();
}

void Menu::update(float deltaTime)
{

}

void Menu::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("2067: A Space Odyssey", -400, 100, 70, WHITE);
   DrawText("Press Enter to Start", -250, 400, 50, WHITE);
}

void Menu::shutdown()
{
   delete mGameState.adrian;
}
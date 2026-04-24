#include "Menu.h"

Menu::Menu()                                      : Scene { {0.0f}, nullptr   } {}
Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
   mGameState.character = new Entity();
}

void Menu::update(float deltaTime)
{

}

void Menu::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText("Introduction to Maze Escaping", -400, -200, 40, WHITE);
   DrawText("You entered an abandoned school", -400, -100, 20, WHITE);
   DrawText("They were prisoned for 670 years", -400, -30, 20, WHITE);
   DrawText("Find your way out before you become one of them", -400, 40, 20, WHITE);
   DrawText("WASD - Move", -400, 110, 20, WHITE);
   DrawText("EFG - Select           Press Enter to Start", -400, 180, 20, WHITE);
}

void Menu::shutdown()
{
   delete mGameState.character;
}
#include "LevelB.h"

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
   mGameState.nextSceneID = 0;

   // Music from https://incompetech.com/music/royalty-free/music.html
   mGameState.bgm = LoadMusicStream("assets/Vibing Over Venus.mp3");
   SetMusicVolume(mGameState.bgm, 0.1f);
   PlayMusicStream(mGameState.bgm);

   // Sounds from Runaway - Kanye West
   mGameState.jumpSound = LoadSound("assets/jump.wav");
   mGameState.nextLevelSound = LoadSound("assets/nextLevel.wav");
   mGameState.fallSound = LoadSound("assets/fall.wav");

   // Assets from https://kenney-assets.itch.io/pixel-platformer
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tileset.png",        // texture filepath
      TILE_DIMENSION,              // tile size
      20, 9,                       // texture cols & rows
      mOrigin                      // in-game origin
   );

   std::map<Direction, std::vector<int>> adrianAnimationAtlas = {
      {DOWN,  {  8, 9, 10, 11, 12, 13, 14, 15 }},
      {LEFT,  {  8, 9, 10, 11, 12, 13, 14, 15 }},
      {UP,    {  8, 9, 10, 11, 12, 13, 14, 15 }},
      {RIGHT, {  8, 9, 10, 11, 12, 13, 14, 15 }},
   };

   float sizeRatio  = 48.0f / 64.0f;

   // Assets from https://floatingkites.itch.io/cute-astronaut
   mGameState.adrian = new Entity(
      {mOrigin.x - 500.0f, mOrigin.y - 200.0f}, // position
      {150.0f * sizeRatio, 150.0f},             // scale
      "assets/adrian.png",                      // texture file address
      ATLAS,                                    // single image or atlas?
      { 3, 8 },                                 // atlas dimensions
      adrianAnimationAtlas,                     // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.adrian->setJumpingPower(600.0f);
   mGameState.adrian->setColliderDimensions({
      mGameState.adrian->getScale().x / 1.8f,
      mGameState.adrian->getScale().y / 1.8f
   });
   mGameState.adrian->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   // Assets from https://kenney-assets.itch.io/pixel-platformer
   mGameState.ai = new Entity(
      {mOrigin.x - 120.0f, mOrigin.y - 200.0f}, // position
      {75.0f, 75.0f},                           // scale
      "assets/blueAI.png",                      // texture file address
      NPC                                       // entity type
   );
   mGameState.ai->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
   mGameState.ai->setAIType(WANDERER);
   mGameState.ai->setSpeed(50);
}

void LevelB::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.adrian->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      mGameState.ai,  // collidable entities
      1               // col. entity count
   );

   mGameState.ai->update(
      deltaTime,         // delta time / fixed timestep
      nullptr,           // player
      mGameState.map,    // map
      nullptr,           // collidable entities
      0                  // col. entity count
   );

   if (mGameState.adrian->getPosition().x > 900.0f) {
      mGameState.nextSceneID = 3;
      PlaySound(mGameState.nextLevelSound);
   }

   if (mGameState.adrian->getPosition().y > 800.0f) {
      PlaySound(mGameState.fallSound);
      if (mGameState.adrian->getLives() == 1) {
         mGameState.nextSceneID = 4;
      } else {
         mGameState.adrian->setLives(mGameState.adrian->getLives() - 1);
         mGameState.adrian->setPosition({mOrigin.x - 500.0f, mOrigin.y - 200.0f});
      }
   }
   if (mGameState.adrian->getLives() == 0) mGameState.nextSceneID = 4;

}

void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.adrian->render();
   mGameState.ai->render();
   mGameState.map->render();
}

void LevelB::shutdown()
{
   delete mGameState.adrian;
   delete mGameState.ai;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
   UnloadSound(mGameState.nextLevelSound);
   UnloadSound(mGameState.fallSound);
}
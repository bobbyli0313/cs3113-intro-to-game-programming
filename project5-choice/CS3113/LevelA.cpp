#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
   mGameState.nextSceneID = 0;

   // Music from https://incompetech.com/music/royalty-free/music.html
   mGameState.bgm = LoadMusicStream("assets/SCP-x5x.mp3");
   SetMusicVolume(mGameState.bgm, 0.2f);
   PlayMusicStream(mGameState.bgm);

   // From https://placeholder-assets.itch.io/50-free-sounds-pack
   mGameState.collectSound = LoadSound("assets/collect.wav");
   mGameState.nextLevelSound = LoadSound("assets/nextLevel.wav");

   // From https://www.youtube.com/watch?v=KK_EBl36DYA
   mGameState.rightAnswerSound = LoadSound("assets/rightAnswer.mp3");
   mGameState.wrongAnswerSound = LoadSound("assets/wrongAnswer.mp3");

   // Assets from https://kenney.nl/assets/pattern-pack-pixel
   mGameState.map = new Map(
      21, 21,                      // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tilemap.png",        // texture filepath
      TILE_DIMENSION,              // tile size
      8, 8,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   std::map<Direction, std::vector<int>> characterAnimationAtlas = {
      {DOWN,  {  0, 3, 6,  9 }},
      {LEFT,  {  2, 5, 8, 11 }},
      {UP,    {  1, 4, 7, 10 }},
      {RIGHT, {  2, 5, 8, 11 }},
   };

   // Assets from https://totuslotus.itch.io/characterpack
   mGameState.character = new Entity(
      {-1000.0f, -1075.0f},                         // position
      {150.0f, 150.0f},                         // scale
      "assets/character.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 3 },                                 // atlas dimensions
      characterAnimationAtlas,                  // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.character->setColliderDimensions({
      mGameState.character->getScale().x / 1.2f,
      mGameState.character->getScale().y / 1.2f
   });

   mGameState.character->setFrameSpeed(8);
   mGameState.character->setShaderStatus(1);

   std::map<Direction, std::vector<int>> aiAnimationAtlas = {
      {DOWN,  {  0, 3, 6,  9 }},
      {LEFT,  {  2, 5, 8, 11 }},
      {UP,    {  1, 4, 7, 10 }},
      {RIGHT, {  2, 5, 8, 11 }},
   };

   // Assets from https://totuslotus.itch.io/characterpack
   mGameState.ai = new Entity(
      {-550.0f, 1650.0f},                       // position
      {150.0f, 150.0f},                         // scale
      "assets/mobA.png",                        // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 3 },                                 // atlas dimensions
      aiAnimationAtlas,                         // actual atlas
      NPC                                       // entity type
   );

   mGameState.ai->setColliderDimensions({
      mGameState.ai->getScale().x / 1.2f,
      mGameState.ai->getScale().y / 1.2f
   });

   mGameState.ai->setFrameSpeed(4);
   mGameState.ai->setAIType(FOLLOWER);
   mGameState.ai->setSpeed(70);
   mGameState.ai->setAIState(WALKING);

   // Assets from https://clockworkraven.itch.io/raven-fantasy-icons
   mGameState.carrot = new Entity(
      {-550.0f, 130.0f},   // position
      {100.0f, 100.0f},    // scale
      "assets/carrot.png", // texture file address
      CARROT               // entity type
   );

   mGameState.potion = new Entity(
      {950.0f, 700.0f},    // position
      {100.0f, 100.0f},    // scale
      "assets/potion.png", // texture file address
      POTION               // entity type
   );

   mGameState.isCarrotCollected = false;
   mGameState.isPotionCollected = false;

   mGameState.question = "Who created C++?";
   mGameState.answerE = "e) Guido van Rossum";
   mGameState.answerF = "f) Dennis Ritchie";
   mGameState.answerG = "g) Bjarne Stroustrup";
}

void LevelA::update(float deltaTime)
{
   Entity* collidableEntities[] = { mGameState.ai, mGameState.carrot, mGameState.potion };
   UpdateMusicStream(mGameState.bgm);

   mGameState.character->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      collidableEntities,  // collidable entities
      3               // col. entity count
   );

   mGameState.ai->update(
      deltaTime,      // delta time / fixed timestep
      mGameState.character,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   if (!mGameState.carrot->isActive() && !mGameState.isCarrotCollected) {
      PlaySound(mGameState.collectSound);
      mGameState.isCarrotCollected = true;
   }

   if (!mGameState.potion->isActive() && !mGameState.isPotionCollected) {
      PlaySound(mGameState.collectSound);
      mGameState.isPotionCollected = true;
   }

   if (mGameState.character->getPosition().x > 2000.0f) {
      mGameState.nextSceneID = 2;
   }

   if (mGameState.character->getLives() == 0) mGameState.nextSceneID = 4;
}

void LevelA::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.character->render();
   mGameState.ai->render();
   mGameState.carrot->render();
   mGameState.potion->render();
   mGameState.map->render();
}

void LevelA::shutdown()
{
   delete mGameState.character;
   delete mGameState.ai;
   delete mGameState.map;
   delete mGameState.carrot;
   delete mGameState.potion;
   delete mGameState.question;
   delete mGameState.answerE;
   delete mGameState.answerF;
   delete mGameState.answerG;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.nextLevelSound);
   UnloadSound(mGameState.collectSound);
   UnloadSound(mGameState.wrongAnswerSound);
   UnloadSound(mGameState.rightAnswerSound);
}
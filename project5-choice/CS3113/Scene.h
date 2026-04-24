#include "Entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *character;
    Map *map;

    Entity *ai;

    Entity *carrot;
    Entity *potion;

    char *question;
    char *answerE;
    char *answerF;
    char *answerG;

    Music bgm;
    Sound nextLevelSound;
    Sound collectSound;
    Sound rightAnswerSound;
    Sound wrongAnswerSound;

    bool isCarrotCollected;
    bool isPotionCollected;

    int nextSceneID;
};

class Scene 
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif
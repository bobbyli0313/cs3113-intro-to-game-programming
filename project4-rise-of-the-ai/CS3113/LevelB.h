#include "LevelA.h"

#ifndef LEVELB_H
#define LEVELB_H

class LevelB : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,   0,   0,   0, 0, 7,   7,   7,   0,   0, 0, 0, 0, 0,
        0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0, 0, 0,
        63,  63,  63,  0, 0, 0,   0,   0,   0,   0, 0, 63,  63,  63,
        105, 105, 105, 0, 0, 63,  63,  63,  63,  0, 0, 105, 105, 105,
        105, 105, 105, 0, 0, 105, 105, 105, 105, 0, 0, 105, 105, 105,
        105, 105, 105, 0, 0, 105, 105, 105, 105, 0, 0, 105, 105, 105
    };

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
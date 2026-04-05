#include "LevelB.h"

#ifndef LEVELC_H
#define LEVELC_H

class LevelC : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 0,
        0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 11,
        0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 0,
        0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 27, 0, 0,
        103, 103, 103, 0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 0,
        105, 105, 105, 0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 27,
        105, 105, 105, 0, 0, 0,   0,   0,   0,   0, 0, 0,  0, 0,
        105, 105, 105, 0, 0, 103, 103, 103, 103, 0, 0, 27, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
#include "Scene.h"

constexpr int LEVEL_WIDTH = 14,
              LEVEL_HEIGHT = 8;

class LevelA : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        23,  23,  23,  0, 23,  23,  23,  0, 0, 23,  23,  23,  23,  23,
        105, 105, 105, 0, 105, 105, 105, 0, 0, 105, 105, 105, 105, 105,
        105, 105, 105, 0, 105, 105, 105, 0, 0, 105, 105, 105, 105, 105
    };

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
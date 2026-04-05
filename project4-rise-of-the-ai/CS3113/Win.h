#include "Lose.h"

#ifndef WIN_H
#define WIN_H

class Win : public Scene {

public:
    Win();
    Win(Vector2 origin, const char *bgHexCode);
    ~Win();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
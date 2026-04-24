#include "Menu.h"

#ifndef LOSE_H
#define LOSE_H

class Lose : public Scene {

public:
    Lose();
    Lose(Vector2 origin, const char *bgHexCode);
    ~Lose();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
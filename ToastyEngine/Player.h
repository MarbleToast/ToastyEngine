#pragma once
#include "GameObject.h"

class Player :
    public GameObject
{
public:
    double movementSpeed = 100.0;

    Player(Vec3 pos) : GameObject("Player", pos) {};

    void handleInput() override;
};


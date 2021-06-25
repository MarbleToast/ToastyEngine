#pragma once
#include "GameObject.h"
#include "Keyboard.h"

class Player :
    public GameObject
{
public:
    double baseMovementSpeed = 1000.0;
    double movementSpeed = baseMovementSpeed;

    Player(Vec3 pos) : GameObject("Player") {
        transform.position = pos;
        Keyboard::addListener(*this);
    };

    void handleInput() override;
    void update() override;
};


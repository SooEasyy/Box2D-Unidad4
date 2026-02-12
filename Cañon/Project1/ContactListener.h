#pragma once
#include <box2d/box2d.h>
#include <functional>

class ContactListener : public b2ContactListener
{
public:
    // Callback para avisar al Game que se tocó el objetivo
    std::function<void()> onGoalReached;

    // Método de Box2D que se ejecuta al comenzar un contacto
    void BeginContact(b2Contact* contact) override;

    bool levelAlreadyWon = false;
};

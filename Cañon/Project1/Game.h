#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Cannon.h"
#include "Ragdoll.h"
#include "Objeto.h"
using namespace sf;
using namespace std;

class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(Mouse::Button button, bool isPressed);
    void createBorders();
    void createObstacles(); 

    RenderWindow window;
    b2World world;
    Cannon cannon;
    bool isFiring;
    ContactListener contactListener; 

    vector<RectangleShape> borders;
    vector<Ragdoll> ragdolls;
    vector<Objeto> objetos; 
};

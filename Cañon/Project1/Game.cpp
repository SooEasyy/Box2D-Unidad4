#include "Game.h"
#include <cmath>
using namespace sf;
using namespace std;

void ContactListener::BeginContact(b2Contact* contact) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    if (bodyA->GetType() == b2_dynamicBody && !bodyA->IsAwake()) {
        bodyA->SetAwake(true);
    }
    if (bodyB->GetType() == b2_dynamicBody && !bodyB->IsAwake()) {
        bodyB->SetAwake(true);
    }
}

Game::Game() :
    window(VideoMode(800, 600), "Cannon Game"),
    world(b2Vec2(0.0f, 0.1f)),
    cannon(Vector2f(100, 500)),
    isFiring(false),
    contactListener() { 

        world.SetContactListener(&contactListener); 

        createBorders();
        createObstacles(); 
    }

void Game::run() {
    Clock clock;
    while (window.isOpen()) {
        processEvents();
        Time deltaTime = clock.restart();
        update();
        render();
    }
}

void Game::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            window.close();
            break;
        case Event::MouseButtonPressed:
            handlePlayerInput(event.mouseButton.button, true);
            break;
        case Event::MouseButtonReleased:
            handlePlayerInput(event.mouseButton.button, false);
            break;
        }
    }
}

void Game::update() {
    cannon.update(window);

    if (isFiring) {
        Vector2i mousePosition = Mouse::getPosition(window);
        Vector2f cannonPosition = cannon.getPosition();

        Vector2f direction = Vector2f(mousePosition.x, mousePosition.y) - cannonPosition;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= distance; 

        float power = distance / 200.0f; 

        ragdolls.emplace_back(world, cannonPosition, power * direction);

        isFiring = false;
    }

    for (auto& ragdoll : ragdolls) {
        ragdoll.update();
    }

    for (auto& objeto : objetos) {
        objeto.update();
    }

    world.Step(1 / 60.f, 8, 3);
}

void Game::render() {
    window.clear();

    for (const auto& border : borders) {
        window.draw(border);
    }

    for (auto& ragdoll : ragdolls) {
        ragdoll.draw(window);
    }

    for (auto& objeto : objetos) {
        objeto.draw(window);
    }

    cannon.draw(window);

    window.display();
}

void Game::handlePlayerInput(Mouse::Button button, bool isPressed) {
    if (button == Mouse::Left) {
        isFiring = isPressed;
    }
}

void Game::createBorders() {
    float thickness = 5.0f;
    Vector2u windowSize = window.getSize();

    RectangleShape topBorder(Vector2f(windowSize.x, thickness));
    topBorder.setPosition(0, 0);

    RectangleShape bottomBorder(Vector2f(windowSize.x, thickness));
    bottomBorder.setPosition(0, windowSize.y - thickness);

    RectangleShape leftBorder(Vector2f(thickness, windowSize.y));
    leftBorder.setPosition(0, 0);

    RectangleShape rightBorder(Vector2f(thickness, windowSize.y));
    rightBorder.setPosition(windowSize.x - thickness, 0);

    borders.push_back(topBorder);
    borders.push_back(bottomBorder);
    borders.push_back(leftBorder);
    borders.push_back(rightBorder);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2EdgeShape groundBox;

    groundBox.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(windowSize.x / 30.0f, 0.0f));
    groundBody->CreateFixture(&groundBox, 0.0f);

    groundBox.SetTwoSided(b2Vec2(0.0f, windowSize.y / 30.0f), b2Vec2(windowSize.x / 30.0f, windowSize.y / 30.0f));
    groundBody->CreateFixture(&groundBox, 0.0f);

    groundBox.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, windowSize.y / 30.0f));
    groundBody->CreateFixture(&groundBox, 0.0f);

    groundBox.SetTwoSided(b2Vec2(windowSize.x / 30.0f, 0.0f), b2Vec2(windowSize.x / 30.0f, windowSize.y / 30.0f));
    groundBody->CreateFixture(&groundBox, 0.0f);
}

void Game::createObstacles() {
    // Estáticos
    objetos.emplace_back(world, Vector2f(400, 300), Vector2f(50, 50), true, Color::Magenta); 
    objetos.emplace_back(world, Vector2f(600, 400), Vector2f(50, 50), true, Color::Magenta); 

    // Dinámicos
    objetos.emplace_back(world, Vector2f(400, 200), Vector2f(30, 100), false, Color::Cyan); 
    objetos.emplace_back(world, Vector2f(600, 300), Vector2f(30, 100), false, Color::Cyan); 
}
#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>

class Goal
{
public:
    Goal(b2World* world, float x, float y);
    ~Goal();

    void Update();
    void Draw(sf::RenderWindow& window);

    b2Body* body;

private:
    sf::Sprite sprite;
};

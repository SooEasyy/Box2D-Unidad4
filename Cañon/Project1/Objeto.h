#ifndef OBJETO_H
#define OBJETO_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

class Objeto {
public:
    Objeto(b2World& world, Vector2f position, Vector2f size, bool isStatic, Color color);

    void update();
    void draw(RenderWindow& window);

private:
    b2Body* body;
    RectangleShape shape;
};

#endif // OBJETO_H

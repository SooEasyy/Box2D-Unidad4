#ifndef OBJETO_H
#define OBJETO_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

class Objeto {
public:
    Objeto(b2World& world, Vector2f position, Vector2f size, bool isStatic, const std::string& textureName);
    ~Objeto();

    void update();
    void draw(RenderWindow& window);
    b2Body* getBody() { return body; }

private:
    b2Body* body;
    sf::Sprite sprite;
    sf::Texture texture;
};

#endif // OBJETO_H

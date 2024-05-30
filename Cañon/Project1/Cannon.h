#ifndef CANNON_H
#define CANNON_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Cannon {
public:
    Cannon(Vector2f position);
    void update(const RenderWindow& window);
    void draw(RenderWindow& window);
    Vector2f getFireDirection() const;
    float getFirePower() const;
    Vector2f getPosition() const;

private:
    Vector2f position;
    float angle;
    float power;
    Texture cannonTexture;
    Sprite cannonSprite;
};

#endif // CANNON_H

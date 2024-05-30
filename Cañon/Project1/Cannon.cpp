#include "Cannon.h"
#include <cmath>

using namespace sf;
using namespace std;

Cannon::Cannon(Vector2f position) : position(position), angle(0), power(0) {
    if (!cannonTexture.loadFromFile("cannon.png")) {
    }
    
    cannonSprite.setTexture(cannonTexture);
    cannonSprite.setOrigin(cannonTexture.getSize().x / 2, cannonTexture.getSize().y / 2);
    cannonSprite.setPosition(position);
    cannonSprite.setScale(0.5f, 0.5f); 
}

void Cannon::update(const RenderWindow& window) {
    Vector2i mousePosition = Mouse::getPosition(window);
    Vector2f aimDir = Vector2f(mousePosition) - position;
    angle = atan2(aimDir.y, aimDir.x) * 180 / 3.14;
    cannonSprite.setRotation(angle);
    power = sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y) / 0.2; 
}

void Cannon::draw(RenderWindow& window) {
    window.draw(cannonSprite);
}

Vector2f Cannon::getFireDirection() const {
    return Vector2f(cos(angle * 3.14 / 180), sin(angle * 3.14 / 180));
}

float Cannon::getFirePower() const {
    return power;
}

Vector2f Cannon::getPosition() const {
    return position;
}

#include "Objeto.h"
using namespace sf;

Objeto::Objeto(b2World& world, Vector2f position, Vector2f size, bool isStatic, Color color) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(position.x / 30.0f, position.y / 30.0f); 
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody; 

    body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(size.x / 60.0f, size.y / 60.0f); 

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = isStatic ? 0.0f : 1.0f; 
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    shape.setSize(size); 
    shape.setFillColor(color);
    shape.setOrigin(size / 2.0f);
    shape.setPosition(position);
}

void Objeto::update() {
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    shape.setPosition(position.x * 30.0f, position.y * 30.0f); 
    shape.setRotation(angle * 180.0f / 3.14159f); 
}

void Objeto::draw(RenderWindow& window) {
    window.draw(shape);
}

#include "Ragdoll.h"
using namespace sf;

Ragdoll::Ragdoll(b2World& world, Vector2f position, Vector2f velocity) {
    b2Body* torso = createLimb(world, position, Vector2f(1.0f, 2.0f), velocity, Color::Green);

    b2Body* head = createLimb(world, position + Vector2f(25.0f, 0.0f), Vector2f(0.8f, 0.8f), velocity, Color::Red);
    createJoint(world, torso, head, b2Vec2((position.x + 0.0f) / 30.0f, (position.y + 20.0f) / 30.0f));

    b2Body* leftArm = createLimb(world, position + Vector2f(-2.0f, -1.0f), Vector2f(0.5f, 1.2f), velocity, Color::Blue);
    createJoint(world, torso, leftArm, b2Vec2((position.x - 5.0f) / 30.0f, (position.y + 15.0f) / 30.0f));

    b2Body* rightArm = createLimb(world, position + Vector2f(2.0f, -1.0f), Vector2f(0.5f, 1.2f), velocity, Color::Blue);
    createJoint(world, torso, rightArm, b2Vec2((position.x + 5.0f) / 30.0f, (position.y + 15.0f) / 30.0f));

    b2Body* leftLeg = createLimb(world, position + Vector2f(-25.0f, 3.0f), Vector2f(0.5f, 2.0f), velocity, Color::Yellow);
    createJoint(world, torso, leftLeg, b2Vec2((position.x + 5.0f) / 30.0f, (position.y - 20.0f) / 30.0f));

    b2Body* rightLeg = createLimb(world, position + Vector2f(-25.0f, 3.0f), Vector2f(0.5f, 2.0f), velocity, Color::Yellow);
    createJoint(world, torso, rightLeg, b2Vec2((position.x - 5.0f) / 30.0f, (position.y - 20.0f) / 30.0f));
}

b2Body* Ragdoll::createLimb(b2World& world, Vector2f position, Vector2f size, Vector2f velocity, Color color) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / 30.0f, position.y / 30.0f); 
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(size.x / 2.0f, size.y / 2.0f); 

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));

    RectangleShape shape;
    shape.setSize(size * 30.0f);
    shape.setFillColor(color);
    shape.setOrigin(shape.getSize() / 2.0f);

    bodies.push_back(body);
    shapes.push_back(shape);

    return body;
}

void Ragdoll::createJoint(b2World& world, b2Body* bodyA, b2Body* bodyB, b2Vec2 anchor) {
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(bodyA, bodyB, anchor);
    b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);
    joints.push_back(joint);
}

void Ragdoll::update() {
    for (size_t i = 0; i < bodies.size(); ++i) {
        b2Vec2 position = bodies[i]->GetPosition();
        float angle = bodies[i]->GetAngle();
        shapes[i].setPosition(position.x * 30.0f, position.y * 30.0f); 
        shapes[i].setRotation(angle * 180.0f / b2_pi);
    }
}

void Ragdoll::draw(RenderWindow& window) {
    for (const auto& shape : shapes) {
        window.draw(shape);
    }
}

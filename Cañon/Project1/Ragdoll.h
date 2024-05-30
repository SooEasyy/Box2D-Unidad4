#ifndef RAGDOLL_H
#define RAGDOLL_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
using namespace sf;
using namespace std;

class Ragdoll {
public:
    Ragdoll(b2World& world, Vector2f position, Vector2f velocity);
    void update();
    void draw(RenderWindow& window);

private:
    vector<b2Body*> bodies; 
    vector<RectangleShape> shapes;  
    vector<b2RevoluteJoint*> joints; 

    b2Body* createLimb(b2World& world, Vector2f position, Vector2f size, Vector2f velocity, Color color);
    void createJoint(b2World& world, b2Body* bodyA, b2Body* bodyB, b2Vec2 anchor);
};

#endif

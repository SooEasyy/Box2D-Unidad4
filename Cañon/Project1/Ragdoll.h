#ifndef RAGDOLL_H
#define RAGDOLL_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
using namespace sf;
using namespace std;

class Ragdoll {
public:
    Ragdoll(b2World* world, sf::Vector2f positionPixels, sf::Vector2f velocityPixels);
    void update();
    void draw(RenderWindow& window);
    sf::Vector2f getPosition() const;
    void destroy(b2World* world);
    bool IsSleeping() const;

private:

    static constexpr float SCALE = 30.f;
    static constexpr float LAUNCH_FORCE = 2.0f;

    vector<b2Body*> bodies; 
    vector<RectangleShape> shapes;  
    vector<b2RevoluteJoint*> joints; 

    b2Body* createLimb(
        b2World& world,
        b2Vec2 positionMeters,
        sf::Vector2f sizeMeters,
        sf::Vector2f velocityPixels,
        sf::Color color
    );

    void createJoint(
        b2World& world,
        b2Body* bodyA,
        b2Body* bodyB,
        b2Vec2 anchorMeters
    );
};

#endif

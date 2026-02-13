#include "Ragdoll.h"

using namespace sf;

Ragdoll::Ragdoll(b2World* world, Vector2f positionPixels, Vector2f velocityPixels)
{
    // Convertimos UNA VEZ a metros
    b2Vec2 basePos(
        positionPixels.x / SCALE,
        positionPixels.y / SCALE
    );

    // TORSO
    b2Body* torso = createLimb(
        *world,
        basePos,
        { 1.0f, 2.0f },
        velocityPixels,
        Color::Green
    );

    // CABEZA
    b2Body* head = createLimb(
        *world,
        basePos + b2Vec2(0.f, -1.5f),
        { 0.8f, 0.8f },
        velocityPixels,
        Color::Red
    );
    createJoint(*world, torso, head, torso->GetWorldCenter() + b2Vec2(0.f, -1.0f));

    // BRAZO IZQUIERDO
    b2Body* leftArm = createLimb(
        *world,
        basePos + b2Vec2(-0.9f, -0.5f),
        { 0.5f, 1.2f },
        velocityPixels,
        Color::Blue
    );
    createJoint(*world, torso, leftArm, torso->GetWorldCenter() + b2Vec2(-0.6f, -0.4f));

    // BRAZO DERECHO
    b2Body* rightArm = createLimb(
        *world,
        basePos + b2Vec2(0.9f, -0.5f),
        { 0.5f, 1.2f },
        velocityPixels,
        Color::Blue
    );
    createJoint(*world, torso, rightArm, torso->GetWorldCenter() + b2Vec2(0.6f, -0.4f));

    // PIERNA IZQUIERDA
    b2Body* leftLeg = createLimb(
        *world,
        basePos + b2Vec2(-0.4f, 1.8f),
        { 0.5f, 2.0f },
        velocityPixels,
        Color::Yellow
    );
    createJoint(*world, torso, leftLeg, torso->GetWorldCenter() + b2Vec2(-0.3f, 1.0f));

    // PIERNA DERECHA
    b2Body* rightLeg = createLimb(
        *world,
        basePos + b2Vec2(0.4f, 1.8f),
        { 0.5f, 2.0f },
        velocityPixels,
        Color::Yellow
    );
    createJoint(*world, torso, rightLeg, torso->GetWorldCenter() + b2Vec2(0.3f, 1.0f));
}

// ----------------------------------------------------
// Crear extremidad
// ----------------------------------------------------
b2Body* Ragdoll::createLimb(
    b2World& world,
    b2Vec2 positionMeters,
    Vector2f sizeMeters,
    Vector2f velocityPixels,
    Color color
)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = positionMeters;

    b2Body* body = world.CreateBody(&bodyDef);

    body->GetUserData().pointer = 1; // BODY_RAGDOLL

    b2PolygonShape box;
    box.SetAsBox(sizeMeters.x / 2.f, sizeMeters.y / 2.f);

    b2FixtureDef fix;
    fix.shape = &box;
    fix.density = 9.8f;
    fix.friction = 0.3f;

    body->CreateFixture(&fix);

    b2Vec2 dir(velocityPixels.x, velocityPixels.y);
    float len = dir.Length();

    if (len > 0.f)
    {
        dir.x /= len;
        dir.y /= len;
    }

    body->SetLinearVelocity(
        b2Vec2(
            dir.x * LAUNCH_FORCE,
            dir.y * LAUNCH_FORCE
        )
    );

    body->SetAwake(true);


    RectangleShape shape(sizeMeters * SCALE);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setFillColor(color);

    bodies.push_back(body);
    shapes.push_back(shape);

    return body;

}

// ----------------------------------------------------
// Crear joint
// ----------------------------------------------------
void Ragdoll::createJoint(b2World& world, b2Body* bodyA, b2Body* bodyB, b2Vec2 anchorMeters)
{
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(bodyA, bodyB, anchorMeters);
    world.CreateJoint(&jointDef);
}

// ----------------------------------------------------
// Update
// ----------------------------------------------------
void Ragdoll::update()
{
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        b2Vec2 pos = bodies[i]->GetPosition();
        float angle = bodies[i]->GetAngle();

        shapes[i].setPosition(pos.x * SCALE, pos.y * SCALE);
        shapes[i].setRotation(angle * 180.f / b2_pi);
    }
}

// ----------------------------------------------------
// Draw
// ----------------------------------------------------
void Ragdoll::draw(RenderWindow& window)
{
    for (auto& s : shapes)
        window.draw(s);
}

// ----------------------------------------------------
// Posición principal (torso)
// ----------------------------------------------------
Vector2f Ragdoll::getPosition() const
{
    if (bodies.empty())
        return { 0.f, 0.f };

    b2Vec2 pos = bodies[0]->GetPosition();
    return { pos.x * SCALE, pos.y * SCALE };
}

void Ragdoll::destroy(b2World* world)
{
    for (b2Body* b : bodies)
        world->DestroyBody(b);

    bodies.clear();
    shapes.clear();
    joints.clear();
}

bool Ragdoll::IsSleeping() const
{
    for (b2Body* b : bodies)
    {
        if (b->GetLinearVelocity().Length() > 0.2f)
            return false;
    }
    return true;
}

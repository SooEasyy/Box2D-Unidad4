#include "Goal.h"
#include "AssetManager.h"
#include "Game.h"

static const float SCALE = 30.f;

Goal::Goal(b2World* world, float x, float y)
{
    // --- BODY ---
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x, y);

    body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = 0.5f;

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.isSensor = true;

    body->CreateFixture(&fixture);

    body->GetUserData().pointer = 2;

    // --- SPRITE ---
    sf::Texture& tex = AssetManager::get().getTexture("goal");

    sprite.setTexture(tex);
    sprite.setOrigin(
        tex.getSize().x / 2.f,
        tex.getSize().y / 2.f
    );

    sprite.setScale(
        (1.f * SCALE) / tex.getSize().x,
        (1.f * SCALE) / tex.getSize().y
    );
}

Goal::~Goal()
{
    // body se destruye desde Level::Unload()
}

void Goal::Update()
{
    b2Vec2 pos = body->GetPosition();
    sprite.setPosition(pos.x * SCALE, pos.y * SCALE);
}

void Goal::Draw(sf::RenderWindow& window)
{
    Update();
    window.draw(sprite);
}

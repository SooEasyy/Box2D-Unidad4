#include "Objeto.h"
#include <iostream>
#include "AssetManager.h"
using namespace sf;

static const float SCALE = 30.f;

Objeto::Objeto(b2World& world, Vector2f position, Vector2f size, bool isStatic, const std::string& textureName)
{
    b2BodyDef bodyDef;
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;

    // ⚠ position en METROS
    bodyDef.position.Set(position.x, position.y);

    body = world.CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(size.x / 2.f, size.y / 2.f);

    b2FixtureDef fixture;
    fixture.shape = &box;
    fixture.density = isStatic ? 0.f : 1.f;
    fixture.friction = 0.4f;

    body->CreateFixture(&fixture);

    // 🔵 VISUAL
    sf::Texture& tex = AssetManager::get().getTexture(textureName);

    sprite.setTexture(tex);
    sprite.setOrigin(
        tex.getSize().x / 2.f,
        tex.getSize().y / 2.f
    );

    sprite.setScale(
        (size.x * 30.f) / tex.getSize().x,
        (size.y * 30.f) / tex.getSize().y
    );
}

Objeto::~Objeto()
{
    if (body)
    {
        body->GetWorld()->DestroyBody(body);
        body = nullptr;
    }
}

void Objeto::update()
{
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle();

    sprite.setPosition(pos.x * 30.f, pos.y * 30.f);
    sprite.setRotation(angle * 180.f / b2_pi);
}

void Objeto::draw(RenderWindow& window) {
    window.draw(sprite);
}

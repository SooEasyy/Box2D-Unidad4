#include "Level.h"
#include <iostream>
#include "Goal.h"

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
Level::Level(int id, b2World* world)
    : levelId(id), world(world), goal(nullptr)
{
}

// ------------------------------------------------------
// Load(): crea todos los objetos del nivel
// ------------------------------------------------------
void Level::Load()
{
    Unload(); // Limpia nivel anterior

    std::cout << "Cargando nivel " << levelId << std::endl;

    // --------------------------
    // NIVEL 1 — Fácil
    // --------------------------
    if (levelId == 1)
    {
        // Suelo     
        AddStaticBlock(13.f, 20.f, 30.f, 2.f, "cesped"); 

        // Bloque
        AddStaticBlock(12.f, 16.f, 5.f, 6.f, "muro_verde");   

        // Goal accesible
        goal = new Goal(world, 23.f, 17.f);
    }

    // --------------------------
    // NIVEL 2 — Intermedio
    // --------------------------
    else if (levelId == 2)
    {

        // Suelo
        AddStaticBlock(13.f, 20.f, 30.f, 2.f, "cesped");

        // Plataforma derecha superior
        AddStaticBlock(23.f, 8.f, 6.f, 2.f, "piedra1");
        
        // Rectangulo rotatorio izquierda
        AddRotatingBlock(12.f, 10.f, 12.f, 22.f, 2.f, 12.f);

        // Rectangulo rotatorio derecha
        AddRotatingBlock(18.f, 10.f, 18.f, 22.f, 2.f, 12.f);

        // GOAL 
        goal = new Goal(world, 23.f, 6.f);       
    }

    // --------------------------
    // NIVEL 3 — Difícil
    // --------------------------
    else if (levelId == 3)
    {
        // Suelo
        AddStaticBlock(13.f, 20.f, 30.f, 2.f, "cesped");

        // Bloque flotante arriba izquierda
        AddStaticBlock(6.f, 4.f, 4.f, 4.f, "piedra2");

        // Rectangulo flotante arriba derecha
        AddStaticBlock(23.f, 5.f, 7.f, 2.f, "piedra2");

        // Bloque medio izquierda
        AddStaticBlock(13.f, 15.f, 4.f, 3.f, "piedra1");

        // Bloque inferior izquierda
        AddStaticBlock(12.f, 18.f, 4.f, 3.f, "piedra1");

        // Bloque medio derecha
        AddStaticBlock(24.f, 13.f, 4.f, 4.f, "piedra1");

        // Bloque inferior derecha
        AddStaticBlock(26.f, 17.f, 4.f, 4.f, "piedra1");

        // Dinamicos izquierda
        AddDynamicBlock(13.f, 3.f, 3.f, 3.f, "colores");
        AddDynamicBlock(13.f, 6.f, 3.f, 3.f, "colores");
        AddDynamicBlock(13.f, 9.f, 3.f, 3.f, "colores");
        AddDynamicBlock(13.f, 12.f, 3.f, 3.f, "colores");

        // Dinamicos derecha
        AddDynamicBlock(17.f, 3.f, 3.f, 3.f, "colores");
        AddDynamicBlock(17.f, 6.f, 3.f, 3.f, "colores");
        AddDynamicBlock(17.f, 9.f, 3.f, 3.f, "colores");
        AddDynamicBlock(17.f, 12.f, 3.f, 3.f, "colores");

        // Cadenas 
        AddHangingBlock(6.f, 4.f, 6.f, 6.f, 3.f, 3.f);

        // GOAL 
        goal = new Goal(world, 23.f, 2.f);
    }
}

// ------------------------------------------------------
// Unload(): destruye bodies y limpia memoria
// ------------------------------------------------------
void Level::Unload()
{
    // Destruir bloques
    for (auto& o : objetos)
        delete o;
    objetos.clear();

    // Destruir goal si existe
    if (goal != nullptr)
    {
        world->DestroyBody(goal->body);
        delete goal;
        goal = nullptr;
    }
}

// ------------------------------------------------------
// Update(): sincroniza Box2D → SFML
// ------------------------------------------------------
void Level::Update()
{
    for (auto& o : objetos)
        o->update();
}

// ------------------------------------------------------
// Draw(): dibuja bloques + goal
// ------------------------------------------------------
void Level::Draw(sf::RenderWindow& window)
{
    for (auto& o : objetos)
        o->draw(window);

    if (goal)
        goal->Draw(window);
}

// ------------------------------------------------------
// Agregar bloque estático
// ------------------------------------------------------
void Level::AddStaticBlock(float x, float y, float w, float h, const std::string& textureName)
{
    objetos.push_back(
        new Objeto(
            *world,
            { x, y},
            { w, h},
            true,
            textureName
        )
    );
}

// ------------------------------------------------------
// Bloque dinámico
// ------------------------------------------------------
void Level::AddDynamicBlock(float x, float y, float w, float h, const std::string& textureName)
{
    objetos.push_back(
        new Objeto(
            *world,
            { x, y},
            { w, h},
            false,
            textureName
        )
    );
}

void Level::AddHangingBlock(
    float anchorX, float anchorY,
    float blockX, float blockY,
    float w, float h
)
{
    // Punto fijo (invisible)
    b2BodyDef anchorDef;
    anchorDef.type = b2_staticBody;
    anchorDef.position.Set(anchorX, anchorY);

    b2Body* anchor = world->CreateBody(&anchorDef);

    // Bloque dinámico
    Objeto* block = new Objeto(
        *world,
        { blockX, blockY },
        { w, h },
        false,
        "cadenas"
    );

    objetos.push_back(block);

    // Joint tipo cuerda
    b2DistanceJointDef jointDef;
    jointDef.bodyA = anchor;
    jointDef.bodyB = block->getBody();

    jointDef.localAnchorA.Set(0.f, 0.f);
    jointDef.localAnchorB.Set(0.f, 0.f);

    jointDef.length = std::abs(blockY - anchorY);
    jointDef.stiffness = 2.0f;   // rigidez
    jointDef.damping = 0.2f;   // amortiguación

    b2Joint* joint = world->CreateJoint(&jointDef);
    joints.push_back(joint);
}
void Level::AddRotatingBlock(float pivotX, float pivotY,
                             float blockX, float blockY,
                             float w, float h)
{
    // Pivot fijo
    b2BodyDef pivotDef;
    pivotDef.type = b2_staticBody;
    pivotDef.position.Set(pivotX, pivotY);

    b2Body* pivot = world->CreateBody(&pivotDef);

    // Bloque dinámico
    Objeto* block = new Objeto(
        *world,
        { blockX, blockY },
        { w, h },
        false,
        "muro_verde"
    );

    objetos.push_back(block);

    // Joint revoluta
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = pivot;
    jointDef.bodyB = block->getBody();

    jointDef.localAnchorA.Set(0, 0);
    jointDef.localAnchorB.Set(0, 0);

    // Límites de giro (recomendado)
    jointDef.enableLimit = true;
    jointDef.lowerAngle = -0.5f;
    jointDef.upperAngle = 0.5f;

    world->CreateJoint(&jointDef);
}
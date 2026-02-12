#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Goal.h"
#include "Objeto.h"
#include "AssetManager.h"
#include "Game.h"

/*
 Level
 Clase que encapsula los objetos (Bodies + shapes) de un nivel.
 - Cada Level sabe crear (Load) y destruir (Unload) sus objetos del world de Box2D.
 - Provee Draw para renderizar las formas SFML ligadas a los bodies.
 - Aquí definimos dos tipos básicos: static y dynamic.
 - Comentarios incluidos en cada función según la consigna del TP.
*/

class Level
{
public:
    Level(int id, b2World* world);
    void Load();
    void Unload();

    void Update();
    void Draw(sf::RenderWindow& window);

    Goal* GetGoal() { return goal; }
    int GetId() const { return levelId; }

private:
    int levelId;         // Identificador del nivel (1,2,3,...)
    b2World* world;      // Puntero al mundo de Box2D (no es dueño)

    std::vector<Objeto*> objetos; // Lista de objetos creados por este nivel
    std::vector<b2Joint*> joints;
    Goal* goal;

    // Funciones helper para crear bloques estáticos y dinámicos
    void AddStaticBlock(float x, float y, float w, float h, const std::string& textureName);
    void AddDynamicBlock(float x, float y, float w, float h, const std::string& textureName);
    void AddHangingBlock(float anchorX, float anchorY, float blockX, float blockY, float w, float h);
    void AddRotatingBlock(float pivotX, float pivotY, float blockX, float blockY, float w, float h);
};

#endif // LEVEL_H


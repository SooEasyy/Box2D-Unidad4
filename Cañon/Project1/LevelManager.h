#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "Level.h"
#include "Game.h"
#include "ContactListener.h"

/*
 LevelManager
 - Encargado de crear y destruir niveles.
 - Permite avanzar (NextLevel) y dibujar el nivel actual.
 - Mantiene el tracking del id actual.
 - Comentarios incluidos.
*/

class LevelManager
{
public:
    LevelManager(b2World* world);

    void LoadLevel(int id);
    void NextLevel();

    void Update();
    void Draw(sf::RenderWindow& window);
    bool IsGameWon() const { return gameWon; }
    bool IsGameLost() const { return gameLost; }

private:
    Level* currentLevel;
    b2World* world;
    int currentLevelId;
    bool gameWon = false;
    bool gameLost = false;
};

#endif // LEVEL_MANAGER_H


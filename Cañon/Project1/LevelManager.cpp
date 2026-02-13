#include "LevelManager.h"

// Constructor: inicializa variables 
LevelManager::LevelManager(b2World* world)
    : world(world), currentLevel(nullptr), currentLevelId(1), gameWon(false)
{
    LoadLevel(currentLevelId);
}

// LoadLevel: destruye el nivel previo y crea uno nuevo
void LevelManager::LoadLevel(int id)
{
    gameWon = false;

    // Limpiar nivel anterior
    if (currentLevel)
    {
        currentLevel->Unload();
        delete currentLevel;
    }

    // Actualizar id y crear nuevo nivel
    currentLevel = new Level(id, world);
    currentLevel->Load();
}

// NextLevel: carga el siguiente nivel (puedes agregar tope si querés) 
void LevelManager::NextLevel()
{
    currentLevelId++;

    // Si pasamos del nivel 3 → VICTORIA
    if (currentLevelId > 3)
    {
        gameWon = true;
        return;
    }

    LoadLevel(currentLevelId);
}

// Draw: re-direcciona al nivel actual 
void LevelManager::Draw(sf::RenderWindow& window)
{
    if (!gameWon && currentLevel)
        currentLevel->Draw(window);
}

void LevelManager::Update()
{
    if (currentLevel)
        currentLevel->Update();
}


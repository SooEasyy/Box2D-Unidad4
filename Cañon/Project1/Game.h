#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include "Menu.h"
#include "InfoScreen.h"
#include "Cannon.h"
#include "Ragdoll.h"
#include "Objeto.h"      
#include "ContactListener.h"
#include "AssetManager.h"


/*
    Clase Game (versión limpia con niveles)
    - Mantiene la ventana, Box2D y la lógica general del juego.
    - El LevelManager ahora controla todos los obstáculos y estructuras.
    - Game sólo controla:
        * Menú / InfoScreen
        * Disparo de ragdolls
        * Contador
        * Borders físicos
        * Render general
*/
class LevelManager;

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    // --- VENTANA + BOX2D ---
    sf::RenderWindow window;
    b2World world;

    // --- SISTEMA DE CONTACTOS ---
    ContactListener contactListener;

    // --- MENÚS ---
    Menu* menu;
    InfoScreen* infoScreen;
    bool inMenu;
    bool inInfoScreen;

    // --- FÍSICA / ESCENA ---
    std::vector<sf::RectangleShape> borders;
    void createBorders();

    // --- NIVELES ---
    LevelManager* levelManager;

    // --- RAGDOLL & DISPARO ---
    std::vector<Ragdoll> ragdolls;
    int ragdollCount = 5;
    bool isFiring;
    Cannon cannon;

    // UI
    sf::Font font;
    sf::Text ragdollCounter;

    // --- VENTANA VICTORIA ---
    bool showWinScreen = false;
    sf::Font winFont;
    sf::Text winText;

    // --- BUCLE ---
    void processEvents();
    void update();
    void render();

    bool pendingNextLevel = false;

    // --- INPUT ---
    void handlePlayerInput(sf::Mouse::Button button, bool isPressed);

    // --- VENTANA DERROTA ---
    bool showLoseScreen = false;
    sf::Text loseText;
    bool checkingLose = false;
    float loseTimer = 0.f;

};

#endif

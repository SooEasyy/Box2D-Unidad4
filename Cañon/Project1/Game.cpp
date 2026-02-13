#include "Game.h"
#include <cmath>
#include <iostream>
#include "LevelManager.h"

using namespace sf;
using namespace std;

/*=====================================================
Inicializa ventana, mundo físico Box2D y managers
Carga assets y configura UI del juego
Carga menú / infoScreen
Crea bordes físicos
Inicializa LevelManager
=====================================================*/
   
Game::Game() :
    window(VideoMode(800, 600), "Cannon Game"),
    world(b2Vec2(0.0f, 0.1f)),
    cannon(Vector2f(100, 500)),
    isFiring(false),
    inMenu(true),
    inInfoScreen(false)
{
    // Assets
    AssetManager::get().loadTexture("cesped", "assets/grass.png");
    AssetManager::get().loadTexture("muro_verde", "assets/grassed_brick.png");
    AssetManager::get().loadTexture("piedra1", "assets/stone_brick.jpg");
    AssetManager::get().loadTexture("piedra2", "assets/stone_brick2.jpg");
    AssetManager::get().loadTexture("madera", "assets/wood.jpg");
    AssetManager::get().loadTexture("goal", "assets/star.png");
    AssetManager::get().loadTexture("colores", "assets/colors.png");
    AssetManager::get().loadTexture("cadenas", "assets/chain.png");


    // Contactos
    world.SetContactListener(&contactListener);

    // Crear manejador de niveles
    levelManager = new LevelManager(&world);

    // Cuando el Goal se toca, avanzar al siguiente nivel
    contactListener.onGoalReached = [this]() {
        pendingNextLevel = true;
        };

    // Menús
    menu = new Menu(window);
    infoScreen = new InfoScreen(window);

    // Bordes físicos
    createBorders();

    // Fuente
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error cargando la fuente!" << endl;
    }

    ragdollCounter.setFont(font);
    ragdollCounter.setCharacterSize(20);
    ragdollCounter.setPosition(20, 20);
    ragdollCounter.setFillColor(Color::White);
    ragdollCounter.setString("Ragdolls restantes: " + to_string(ragdollCount));

    // Pantalla de Victoria
    winText.setFont(font);
    winText.setString("¡Has ganado el juego!");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition(120, 200);

    // Pantalla de Derrota
    loseText.setFont(font);
    loseText.setString("Has perdido");
    loseText.setCharacterSize(60);
    loseText.setFillColor(sf::Color::Red);
    loseText.setPosition(200, 200);

}

/* Destructor */
Game::~Game()
{
    delete menu;
    delete infoScreen;
    delete levelManager;
}


/* =====================================================
   LOOP PRINCIPAL
   ===================================================== */
void Game::run() {
    Clock clock;
    while (window.isOpen()) {

        processEvents();

        update();

        render();
    }
}

/* =====================================================
   EVENTOS
   ===================================================== */
void Game::processEvents() {
    Event event;
    while (window.pollEvent(event)) {

        if (event.type == Event::Closed)
            window.close();

        // --- Clics del menú ---
        if (event.type == Event::MouseButtonPressed) {

            if (inMenu) {
                Vector2i mousePos = Mouse::getPosition(window);
                int action = menu->handleClick(mousePos);

                if (action == 1)      inMenu = false;
                else if (action == 2) { inInfoScreen = true; inMenu = false; }
            }

            else if (inInfoScreen) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (infoScreen->handleClick(mousePos)) {
                    inInfoScreen = false;
                    inMenu = true;
                }
            }

            // --- Disparo de ragdolls ---
            else {
                if (event.mouseButton.button == Mouse::Left && ragdollCount > 0)
                    isFiring = true;
            }
        }

        if (event.type == Event::MouseButtonReleased) {
            if (event.mouseButton.button == Mouse::Left)
                isFiring = false;
        }
    }
}

/* =====================================================
   UPDATE — LÓGICA DEL JUEGO
   ===================================================== */
void Game::update() {

    if (showLoseScreen)
        return;

    if (inMenu || inInfoScreen) return;

    cannon.update(window);

    // --- Disparo ---
    if (isFiring && ragdollCount > 0) {

        Vector2i mousePosition = Mouse::getPosition(window);
        Vector2f cannonPosition = cannon.getPosition();

        Vector2f direction = Vector2f(mousePosition) - cannonPosition;
        float dist = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= dist;

        float power = dist / 200.0f;

        ragdolls.emplace_back(&world, cannonPosition, power * direction);

        ragdollCount--;
        ragdollCounter.setString("Ragdolls restantes: " + to_string(ragdollCount));

        // --- DERROTA ---
        if (ragdollCount == 0)
        {
            checkingLose = true;   // ⬅️ empezamos a observar
            loseTimer = 0.f;
        }

        isFiring = false;
    }

    world.Step(1.f / 60.f, 8, 3);

    // --- Actualizar objetos ---
    if (levelManager)
        levelManager->Update();

    // --- Actualizar ragdolls ---
    for (auto& rag : ragdolls)
        rag.update();

    // Si el Goal fue tocado durante la colisión → cambiar de nivel después del Step
    if (pendingNextLevel)
    {
        for (auto& rag : ragdolls)
            rag.destroy(&world);

        ragdolls.clear();

        pendingNextLevel = false;
        contactListener.levelAlreadyWon = false;
        world.Step(0, 0, 0);
        levelManager->NextLevel();

        // Reiniciar ragdolls para el nuevo nivel
        ragdolls.clear();
        ragdollCount = 5;
        ragdollCounter.setString("Ragdolls restantes: " + to_string(ragdollCount));
    }

    // --- VICTORIA ---
    if (levelManager->IsGameWon())
    {
        showWinScreen = true;
    }

    // --- DERROTA ---
    if (checkingLose && !pendingNextLevel && !showLoseScreen)
    {
        bool allStopped = true;

        for (auto& rag : ragdolls)
        {
            if (!rag.IsSleeping())
            {
                allStopped = false;
                break;
            }
        }

        if (allStopped)
        {
            loseTimer += 1.f / 60.f;

            // Espera pequeña para evitar falsos positivos
            if (loseTimer > 1.0f)
            {
                showLoseScreen = true;
                checkingLose = false;
            }
        }
        else
        {
            loseTimer = 0.f; // si se vuelve a mover, reinicia
        }
    }

}

/* =====================================================
   RENDER — DIBUJA TODO
   ===================================================== */
void Game::render() {

    window.clear();

    if (inMenu)
        menu->draw();

    else if (inInfoScreen)
        infoScreen->draw();


    else {

        // Bordes
        for (const auto& border : borders)
            window.draw(border);

        // Ragdolls
        for (auto& rag : ragdolls)
            rag.draw(window);

        // Level Manager
        levelManager->Draw(window);

        // Cannon
        cannon.draw(window);

        // UI
        window.draw(ragdollCounter);

        if (showWinScreen)
        {
            window.clear(sf::Color::Black);
            window.draw(winText);
        }

        if (showLoseScreen)
        {
            window.clear(sf::Color::Black);
            window.draw(loseText);
            window.display();
            return;
        }
    }

    window.display();
}

/* =====================================================
   bordes estáticos (físicos + gráficos)
   ===================================================== */
void Game::createBorders() {

    float thickness = 5.0f;
    Vector2u size = window.getSize();

    RectangleShape top({ float(size.x), thickness });
    top.setPosition(0, 0);

    RectangleShape bottom({ float(size.x), thickness });
    bottom.setPosition(0, size.y - thickness);

    RectangleShape left({ thickness, float(size.y) });
    left.setPosition(0, 0);

    RectangleShape right({ thickness, float(size.y) });
    right.setPosition(size.x - thickness, 0);

    borders = { top, bottom, left, right };

    // === Box2D bordes reales ===
    b2BodyDef def;
    b2Body* ground = world.CreateBody(&def);

    b2EdgeShape edge;

    float w = size.x / 30.f;
    float h = size.y / 30.f;

    // top
    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(w, 0));
    ground->CreateFixture(&edge, 0);

    // bottom
    edge.SetTwoSided(b2Vec2(0, h), b2Vec2(w, h));
    ground->CreateFixture(&edge, 0);

    // left
    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(0, h));
    ground->CreateFixture(&edge, 0);

    // right
    edge.SetTwoSided(b2Vec2(w, 0), b2Vec2(w, h));
    ground->CreateFixture(&edge, 0);
}


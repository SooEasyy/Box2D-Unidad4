#include "Menu.h"

// Pantalla del Menu del juego
Menu::Menu(sf::RenderWindow& win) : window(win) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error cargando la fuente!" << std::endl;
    }

    title.setFont(font);
    title.setString("Ragdoll Cannon");
    title.setCharacterSize(50);
    title.setPosition(150, 50);
    title.setFillColor(sf::Color::White);

    playButton.setFont(font);
    playButton.setString("Jugar");
    playButton.setCharacterSize(30);
    playButton.setPosition(180, 200);
    playButton.setFillColor(sf::Color::Black);

    infoButton.setFont(font);
    infoButton.setString("Informacion");
    infoButton.setCharacterSize(30);
    infoButton.setPosition(180, 300);
    infoButton.setFillColor(sf::Color::Black);

    playBox.setSize(sf::Vector2f(200, 50));
    playBox.setPosition(170, 190);
    playBox.setFillColor(sf::Color::White);

    infoBox.setSize(sf::Vector2f(200, 50));
    infoBox.setPosition(170, 290);
    infoBox.setFillColor(sf::Color::White);
}

void Menu::draw() {
    window.draw(title);
    window.draw(playBox);
    window.draw(playButton);
    window.draw(infoBox);
    window.draw(infoButton);
}

int Menu::handleClick(sf::Vector2i mousePos) {
    if (playBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        return 1; // Jugar
    }
    if (infoBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        return 2; // Información
    }
    return 0; // Nada
}

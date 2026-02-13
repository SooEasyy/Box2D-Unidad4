#include "InfoScreen.h"

//Pantalla de informacion del juego
InfoScreen::InfoScreen(sf::RenderWindow& win) : window(win) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error cargando la fuente!" << std::endl;
    }

    infoText.setFont(font);
    infoText.setString("Instrucciones:\n\n- Apunta y dispara los ragdolls.\n- Golpea el objetivo para ganar.\n- Evita obstáculos y usa la física a tu favor.");
    infoText.setCharacterSize(20);
    infoText.setPosition(50, 50);
    infoText.setFillColor(sf::Color::White);

    backButton.setFont(font);
    backButton.setString("Volver");
    backButton.setCharacterSize(30);
    backButton.setPosition(220, 300);
    backButton.setFillColor(sf::Color::Black);

    backBox.setSize(sf::Vector2f(150, 50));
    backBox.setPosition(210, 290);
    backBox.setFillColor(sf::Color::White);
}

void InfoScreen::draw() {
    window.draw(infoText);
    window.draw(backBox);
    window.draw(backButton);
}

bool InfoScreen::handleClick(sf::Vector2i mousePos) {
    return backBox.getGlobalBounds().contains(mousePos.x, mousePos.y);
}
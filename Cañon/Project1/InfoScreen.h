#ifndef INFOSCREEN_H
#define INFOSCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>

class InfoScreen {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text infoText;
    sf::Text backButton;
    sf::RectangleShape backBox;

public:
    InfoScreen(sf::RenderWindow& win);
    void draw();
    bool handleClick(sf::Vector2i mousePos);
};

#endif // INFOSCREEN_H
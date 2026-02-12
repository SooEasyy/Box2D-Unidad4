#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
    sf::Text playButton;
    sf::Text infoButton;
    sf::RectangleShape playBox;
    sf::RectangleShape infoBox;

public:
    Menu(sf::RenderWindow& win);
    void draw();
    int handleClick(sf::Vector2i mousePos);
};

#endif // MENU_H
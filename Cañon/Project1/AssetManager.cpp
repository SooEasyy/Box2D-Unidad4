#include "AssetManager.h"
#include <iostream>

AssetManager& AssetManager::get()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::loadTexture(const std::string& name, const std::string& path)
{
    sf::Texture tex;
    if (!tex.loadFromFile(path))
        std::cout << "Error cargando textura: " << path << std::endl;

    textures[name] = tex;
}

sf::Texture& AssetManager::getTexture(const std::string& name)
{
    return textures.at(name);
}

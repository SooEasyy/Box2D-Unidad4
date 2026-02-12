#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
public:
    static AssetManager& get();

    void loadTexture(const std::string& name, const std::string& path);
    sf::Texture& getTexture(const std::string& name);

private:
    AssetManager() {}
    std::map<std::string, sf::Texture> textures;
};


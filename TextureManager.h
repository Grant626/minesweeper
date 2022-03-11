#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager  
{
	static std::unordered_map<std::string, sf::Texture> textures;

public:
	static void LoadTexture(const char* filename);
	static sf::Texture& GetTexture(const char* textureKey);
	static void Clear();


};


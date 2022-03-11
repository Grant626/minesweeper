#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const char* fileName)
{
	string filePath = "images/";
	filePath += fileName;
	filePath += ".png";

	textures[fileName].loadFromFile(filePath);
}

sf::Texture& TextureManager::GetTexture(const char* textureKey)
{
	if (textures.find(textureKey) == textures.end())
	{
		LoadTexture(textureKey);

	}
	return textures[textureKey];
}

void TextureManager::Clear()
{
	textures.clear();
}
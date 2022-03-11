#pragma once
#include <vector>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureManager.h"
#include <unordered_map>
class Board
{
public:
	std::vector<Tile> board;
	static void initializeBoard(sf::RenderWindow* window);
};
  

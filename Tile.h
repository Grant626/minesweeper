#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Tile
{
	

public:
	std::vector<Tile*> neighbors;
	bool hasMines;
	float posX;
	float posY;
	sf::Sprite number;
	int numMines;  
	bool mine;
	bool flag;
	bool revealed;
	bool isMine();
	Tile();
	void revealEmptyTiles();

};


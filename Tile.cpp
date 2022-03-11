#include "Tile.h"
#include <iostream>

bool Tile::isMine()
{
	return this->mine;
}
Tile::Tile()
{
	mine = false;
	flag = false;
	revealed = false;
	hasMines = false;
	posX = 0;  
	posY = 0;
	numMines = 0;
}
void Tile::revealEmptyTiles()
{
	for (int i = 0; i < 8; i++) {
		if (this->neighbors[i] != nullptr && !this->neighbors[i]->hasMines && !this->neighbors[i]->isMine() && !this->neighbors[i]->revealed && !this->neighbors[i]->flag) {
			this->neighbors[i]->revealed = true;
			this->neighbors[i]->revealEmptyTiles();
		}
		if(this->neighbors[i] != nullptr && !this->neighbors[i]->flag) 
			this->neighbors[i]->revealed = true;
	}
}
#include "Board.h"

unordered_map<string, sf::Texture> textures;

 void Board::initializeBoard(sf::RenderWindow* window)
{
    std::vector<sf::Sprite> revealedTiles;
    //draw board of just revealed tiles 
    if (!textures["tile_revealed"].loadFromFile("images/tile_revealed.png"))
        std::cout << "Error loading texture";
    for (int i = 0; i < 16; i++)
    {
        revealedTiles.push_back(sf::Sprite());
        revealedTiles[i].setTexture(TextureManager::GetTexture("tile_revealed"));
        for (int j = 0; j < 25; j++) {
            revealedTiles[i].setPosition(sf::Vector2f((float)(j * 32), (float)(i * 32)));
            //window.draw(revealedTiles[i]);
        }   
    }
}
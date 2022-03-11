#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Random.h"
#include "TextureManager.h"
#include "Board.h"
using namespace std;

void resetBoard(vector<Tile> &randBoard, vector<int> &mineTiles, int &columns, int &rows, int &numMines) {

    ifstream configFile;
    configFile.open("boards/config.cfg");

    configFile >> columns;
    configFile >> rows;
    configFile >> numMines;

    cout << columns << endl;
    cout << rows << endl;
    cout << numMines << endl;

    int boardWidth = columns * 32;
    int boardHeight = rows * 32;

    //generate random numbers 0-(colums * rows), numMines times in a vector to know what tiles have mines
    for (unsigned int i = 0; i < numMines; i++)
    {
        int temp = Random::Int(0, columns * rows);
        for (unsigned int j = 0; j < mineTiles.size(); j++)
        {
            if (temp == mineTiles[j]) {
                temp = Random::Int(0, columns * rows);
            }
        }
        mineTiles.push_back(temp);
    }

    //pushback board vector with tiles and if tile matches random number, make mine=true;
    int count = 0;
    for (unsigned int i = 0; i < columns * rows; i++)
    {
        randBoard.push_back(Tile());
        for (unsigned int j = 0; j < mineTiles.size(); j++)
        {
            if (mineTiles[j] == i) {
                randBoard[i].mine = true;
                count++;
            }
        }
    }

    //intialize position variables for tiles in randBoard
    int countX = 0;
    int countY = 0;
    for (unsigned int i = 1; i <= randBoard.size(); i++)
    {
        if (countX == columns) {
            countX = 0;
            countY++;
        }
        randBoard[i - 1].posX = (float)(countX * 32);
        randBoard[i - 1].posY = (float)(countY * 32);
        //cout << randBoard[i-1].posX << " " << randBoard[i-1].posY << endl;
        countX++;
    }

    //initialize neighbors for each tile
    for (unsigned int i = 0; i < randBoard.size(); i++)
    {
        if (randBoard[i].posY == 0) {
            for (int j = 0; j < 3; j++)
                randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posX == 0) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(&randBoard[i - columns]);
            randBoard[i].neighbors.push_back(&randBoard[i - (columns - 1)]);
        }
        else if (randBoard[i].posX == boardWidth - 32) {
            randBoard[i].neighbors.push_back(&randBoard[i - (columns + 1)]);
            randBoard[i].neighbors.push_back(&randBoard[i - columns]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i - (columns + 1)]);
            randBoard[i].neighbors.push_back(&randBoard[i - columns]);
            randBoard[i].neighbors.push_back(&randBoard[i - (columns - 1)]);
        }
        if (randBoard[i].posX == boardWidth - 32) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posY == boardHeight - 32) {
            randBoard[i].neighbors.push_back(&randBoard[i + 1]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i + 1]);
            randBoard[i].neighbors.push_back(&randBoard[i + (columns + 1)]);
        }
        if (randBoard[i].posY == boardHeight - 32) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posX == 0) {
            randBoard[i].neighbors.push_back(&randBoard[i + columns]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i + columns]);
            randBoard[i].neighbors.push_back(&randBoard[i + (columns - 1)]);
        }
        if (randBoard[i].posX == 0)
            randBoard[i].neighbors.push_back(nullptr);
        else
            randBoard[i].neighbors.push_back(&randBoard[i - 1]);
    }

    //initialize number sprite for each tiles based on number of neighbors
    for (unsigned int i = 0; i < randBoard.size(); i++)
    {
        int count = 0;
        if (!randBoard[i].isMine()) {
            for (int j = 0; j < 8; j++)
            {
                if (randBoard[i].neighbors[j] != nullptr) {
                    if (randBoard[i].neighbors[j]->mine == true)
                        count++;
                }
            }
            if (count > 0) {
                randBoard[i].hasMines = true;
            }
            if (count == 1) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_1"));
                randBoard[i].numMines = 1;
            }
            else if (count == 2) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_2"));
                randBoard[i].numMines = 2;
            }
            else if (count == 3) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_3"));
                randBoard[i].numMines = 3;
            }
            else if (count == 4) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_4"));
                randBoard[i].numMines = 4;
            }
            else if (count == 5) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_5"));
                randBoard[i].numMines = 5;
            }
            else if (count == 6) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_6"));
                randBoard[i].numMines = 6;
            }
            else if (count == 7) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_7"));
                randBoard[i].numMines = 7;
            }
            else if (count == 8) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_8"));
                randBoard[i].numMines = 8;
            }
        }
    }
}

void setTestBoard(vector<Tile>& randBoard, string boardName, int& minecount, int &columns, int &rows) {
    string file = "boards/" + boardName + ".brd";
    ifstream inFile;
    inFile.open(file);
    minecount = 0;
    
    columns = 25;
    rows = 16;

    vector<char> board;
    char mine;
    for (int i = 0; i < 400; i++) {
        inFile >> mine;
        board.push_back(mine);
        randBoard.push_back(Tile());
        if (mine == '1') {
            minecount++;
            randBoard[i].mine = true;
        }
    }
    //intialize position variables for tiles in randBoard
    int countX = 0;
    int countY = 0;
    for (unsigned int i = 1; i <= randBoard.size(); i++)
    {
        if (countX == 25) {
            countX = 0;
            countY++;
        }
        randBoard[i - 1].posX = (float)(countX * 32);
        randBoard[i - 1].posY = (float)(countY * 32);
        //cout << randBoard[i-1].posX << " " << randBoard[i-1].posY << endl;
        countX++;
    }

    //initialize neighbors for each tile
    for (unsigned int i = 0; i < randBoard.size(); i++)
    {
        if (randBoard[i].posY == 0) {
            for (int j = 0; j < 3; j++)
                randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posX == 0) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(&randBoard[i - 25]);
            randBoard[i].neighbors.push_back(&randBoard[i - 24]);
        }
        else if (randBoard[i].posX == 768) {
            randBoard[i].neighbors.push_back(&randBoard[i - 26]);
            randBoard[i].neighbors.push_back(&randBoard[i - 25]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i - 26]);
            randBoard[i].neighbors.push_back(&randBoard[i - 25]);
            randBoard[i].neighbors.push_back(&randBoard[i - 24]);
        }
        if (randBoard[i].posX == 768) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posY == 480) {
            randBoard[i].neighbors.push_back(&randBoard[i + 1]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i + 1]);
            randBoard[i].neighbors.push_back(&randBoard[i + 26]);
        }
        if (randBoard[i].posY == 480) {
            randBoard[i].neighbors.push_back(nullptr);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else if (randBoard[i].posX == 0) {
            randBoard[i].neighbors.push_back(&randBoard[i + 25]);
            randBoard[i].neighbors.push_back(nullptr);
        }
        else {
            randBoard[i].neighbors.push_back(&randBoard[i + 25]);
            randBoard[i].neighbors.push_back(&randBoard[i + 24]);
        }
        if (randBoard[i].posX == 0)
            randBoard[i].neighbors.push_back(nullptr);
        else
            randBoard[i].neighbors.push_back(&randBoard[i - 1]);
    }

    //initialize number sprite for each tiles based on number of neighbors
    for (unsigned int i = 0; i < randBoard.size(); i++)
    {
        int count = 0;
        if (!randBoard[i].isMine()) {
            for (int j = 0; j < 8; j++)
            {
                if (randBoard[i].neighbors[j] != nullptr) {
                    if (randBoard[i].neighbors[j]->mine == true)
                        count++;
                }
            }
            if (count > 0) {
                randBoard[i].hasMines = true;
            }
            if (count == 1) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_1"));
                randBoard[i].numMines = 1;
            }
            else if (count == 2) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_2"));
                randBoard[i].numMines = 2;
            }
            else if (count == 3) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_3"));
                randBoard[i].numMines = 3;
            }
            else if (count == 4) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_4"));
                randBoard[i].numMines = 4;
            }
            else if (count == 5) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_5"));
                randBoard[i].numMines = 5;
            }
            else if (count == 6) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_6"));
                randBoard[i].numMines = 6;
            }
            else if (count == 7) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_7"));
                randBoard[i].numMines = 7;
            }
            else if (count == 8) {
                randBoard[i].number.setTexture(TextureManager::GetTexture("number_8"));
                randBoard[i].numMines = 8;
            }
        }
    }
}

int main()
{
    //board data from config file
    int columns = 0;
    int rows = 0;
    int numMines = 50;

    vector<Tile> randBoard;
    vector<int> mineTiles;
    resetBoard(randBoard, mineTiles, columns, rows, numMines);
    int boardWidth = columns * 32;
    int boardHeight = rows * 32;
    
    cout << numMines << endl;

    unordered_map<string, sf::Texture> textures;
    /* PROGRAM START */
    //Load sprites 
    sf::Sprite mineSprite(TextureManager::GetTexture("mine"));
    sf::Sprite flagSprite(TextureManager::GetTexture("flag"));
    sf::Sprite tileHiddenSprite(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite tileRevealedSprite(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite numOneSprite(TextureManager::GetTexture("number_1"));
    sf::Sprite numTwoSprite(TextureManager::GetTexture("number_2"));
    sf::Sprite numThreeSprite(TextureManager::GetTexture("number_3"));
    sf::Sprite numFourSprite(TextureManager::GetTexture("number_4"));
    sf::Sprite numFiveSprite(TextureManager::GetTexture("number_5"));
    sf::Sprite numSixSprite(TextureManager::GetTexture("number_6"));
    sf::Sprite numSevenSprite(TextureManager::GetTexture("number_7"));
    sf::Sprite numEightSprite(TextureManager::GetTexture("number_8")); 
    sf::Sprite smileFace(TextureManager::GetTexture("face_happy"));
    sf::Sprite loseFace(TextureManager::GetTexture("face_lose"));
    sf::Sprite winFace(TextureManager::GetTexture("face_win"));
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    sf::Sprite digit0(TextureManager::GetTexture("digits"));
    sf::Sprite digit1(TextureManager::GetTexture("digits"));
    sf::Sprite digit2(TextureManager::GetTexture("digits"));
    sf::Sprite digit3(TextureManager::GetTexture("digits"));
    sf::Sprite digit4(TextureManager::GetTexture("digits"));
    sf::Sprite digit5(TextureManager::GetTexture("digits"));
    sf::Sprite digit6(TextureManager::GetTexture("digits"));
    sf::Sprite digit7(TextureManager::GetTexture("digits"));
    sf::Sprite digit8(TextureManager::GetTexture("digits"));
    sf::Sprite digit9(TextureManager::GetTexture("digits"));
    
    digit0.setTextureRect(sf::IntRect(0, 0, 21, 32));
    digit1.setTextureRect(sf::IntRect(21, 0, 21, 32));
    digit2.setTextureRect(sf::IntRect(42, 0, 21, 32));
    digit3.setTextureRect(sf::IntRect(63, 0, 21, 32));
    digit4.setTextureRect(sf::IntRect(84, 0, 21, 32));
    digit5.setTextureRect(sf::IntRect(105, 0, 21, 32));
    digit6.setTextureRect(sf::IntRect(126, 0, 21, 32));
    digit7.setTextureRect(sf::IntRect(147, 0, 21, 32));
    digit8.setTextureRect(sf::IntRect(168, 0, 21, 32));
    digit9.setTextureRect(sf::IntRect(189, 0, 21, 32));


    smileFace.setPosition(sf::Vector2f(((columns * 32)/2) - 32, rows * 32));
    loseFace.setPosition(sf::Vector2f(((columns * 32) / 2) - 32, rows * 32));
    winFace.setPosition(sf::Vector2f(((columns * 32) / 2) - 32, rows * 32));

    cout << columns << " " << rows;
    //render window and create vector to store random tiles
    sf::RenderWindow window(sf::VideoMode(columns * 32, (rows * 32) + 88), "Minesweeper");

    vector<sf::Sprite> revealedTiles;
    vector<sf::Sprite> hiddenTiles;
    vector<sf::Sprite> flags;
    vector<sf::Sprite> mines;
    //int mineCount = 50;

    //create sprite vector for mines
    for (int i = 0; i < numMines; i++)
    {
        mines.push_back(sf::Sprite());
        mines[i].setTexture(TextureManager::GetTexture("mine"));
    }

    for (int i = 0; i < randBoard.size(); i++)
    {
        hiddenTiles.push_back(sf::Sprite());
        revealedTiles.push_back(sf::Sprite());
    }
    for (int i = 0; i <= numMines; i++) {
        flags.push_back(sf::Sprite());
    }
    int flagCount = 0;
    bool gameEnd = false;
    bool gameWin = false;
    bool debugToggle = false;
    int digit = numMines;
    string digits;
    bool test = false;

    while (true) {
        if (test) {
            sf::RenderWindow window(sf::VideoMode(columns * 32, (rows * 32) + 88), "Minesweeper");
            test = false;
        }

        while (window.isOpen())
        {
            window.clear();

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (sf::Mouse::getPosition(window).x > ((boardWidth / 2) - 32) + 192 && sf::Mouse::getPosition(window).x < ((boardWidth / 2) - 32) + 256 && sf::Mouse::getPosition(window).y > boardHeight && sf::Mouse::getPosition(window).y < (boardHeight + 64)) {
                            randBoard.clear();
                            mineTiles.clear();
                            gameEnd = false;
                            gameWin = false;
                            flagCount = 0;
                            setTestBoard(randBoard, "testboard1", numMines, columns, rows);
                            int boardWidth = columns * 32;
                            int boardHeight = rows * 32;
                            for (int i = 0; i < randBoard.size(); i++)
                            {
                                hiddenTiles.push_back(sf::Sprite());
                                revealedTiles.push_back(sf::Sprite());
                            }
                            test = true;
                            window.setSize(sf::Vector2u(columns * 32, (rows * 32)+88));
                        }
                        if (sf::Mouse::getPosition(window).x > ((boardWidth / 2) - 32) + 256 && sf::Mouse::getPosition(window).x < ((boardWidth / 2) - 32) + 320 && sf::Mouse::getPosition(window).y > boardHeight && sf::Mouse::getPosition(window).y < (boardHeight + 64)) {
                            randBoard.clear();
                            mineTiles.clear();
                            gameEnd = false;
                            gameWin = false;
                            flagCount = 0;
                            setTestBoard(randBoard, "testboard2", numMines, columns, rows);
                            int boardWidth = columns * 32;
                            int boardHeight = rows * 32;
                            for (int i = 0; i < randBoard.size(); i++)
                            {
                                hiddenTiles.push_back(sf::Sprite());
                                revealedTiles.push_back(sf::Sprite());
                            }
                            test = true;
                            window.setSize(sf::Vector2u(columns * 32, (rows * 32) + 88));
                        }
                        if (sf::Mouse::getPosition(window).x > ((boardWidth / 2) - 32) + 320 && sf::Mouse::getPosition(window).x < ((boardWidth / 2) - 32) + 384 && sf::Mouse::getPosition(window).y > boardHeight && sf::Mouse::getPosition(window).y < (boardHeight + 64)) {
                            randBoard.clear();
                            mineTiles.clear();
                            gameEnd = false;
                            gameWin = false;
                            flagCount = 0;
                            setTestBoard(randBoard, "testboard3", numMines, columns, rows);
                            int boardWidth = columns * 32;
                            int boardHeight = rows * 32;
                            for (int i = 0; i < randBoard.size(); i++)
                            {
                                hiddenTiles.push_back(sf::Sprite());
                                revealedTiles.push_back(sf::Sprite());
                            }
                            test = true;
                            window.setSize(sf::Vector2u(columns * 32, (rows * 32) + 88));
                        }
                        if (sf::Mouse::getPosition(window).x > (boardWidth / 2) - 32 && sf::Mouse::getPosition(window).x < (boardWidth / 2) + 32 && sf::Mouse::getPosition(window).y > boardHeight && sf::Mouse::getPosition(window).y < (boardHeight + 64)) {
                            randBoard.clear();
                            mineTiles.clear();
                            gameEnd = false;
                            gameWin = false;
                            flagCount = 0;
                            //numMines = 0;
                            resetBoard(randBoard, mineTiles, columns, rows, numMines);
                            int boardWidth = columns * 32;
                            int boardHeight = rows * 32;
                            for (int i = 0; i < randBoard.size(); i++)
                            {
                                hiddenTiles.push_back(sf::Sprite());
                                revealedTiles.push_back(sf::Sprite());
                            }
                            test = true;
                            window.setSize(sf::Vector2u(columns * 32, (rows * 32) + 88));
                        }

                        if (sf::Mouse::getPosition(window).x > (boardWidth / 2) + 96 && sf::Mouse::getPosition(window).x < (boardWidth / 2) + 160 && sf::Mouse::getPosition(window).y > boardHeight && sf::Mouse::getPosition(window).y < (boardHeight + 64)) {
                            if (!debugToggle) {
                                debugToggle = true;
                            }
                            else {
                                debugToggle = false;
                            }
                        }

                        if (sf::Mouse::getPosition(window).x < boardWidth && sf::Mouse::getPosition(window).y < boardHeight && !gameEnd && !gameWin) {
                            for (unsigned int i = 0; i < randBoard.size(); i++) {
                                if (sf::Mouse::getPosition(window).x > randBoard[i].posX && sf::Mouse::getPosition(window).x < (randBoard[i].posX + 32) && sf::Mouse::getPosition(window).y > randBoard[i].posY && sf::Mouse::getPosition(window).y < randBoard[i].posY + 32) {
                                    if (!randBoard[i].revealed && !randBoard[i].flag) {
                                        randBoard[i].revealed = true;
                                        if (!randBoard[i].hasMines && !randBoard[i].mine) {
                                            randBoard[i].revealEmptyTiles();
                                        }
                                    }
                                    if (randBoard[i].mine && !randBoard[i].flag)
                                        gameEnd = true;
                                }
                            }
                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        if (sf::Mouse::getPosition(window).x < boardWidth && sf::Mouse::getPosition(window).y < boardHeight && !gameEnd && !gameWin) {
                            for (unsigned int i = 0; i < randBoard.size(); i++) {
                                if (sf::Mouse::getPosition(window).x > randBoard[i].posX && sf::Mouse::getPosition(window).x < (randBoard[i].posX + 32) && sf::Mouse::getPosition(window).y > randBoard[i].posY && sf::Mouse::getPosition(window).y < randBoard[i].posY + 32) {
                                    if (randBoard[i].flag) {
                                        randBoard[i].flag = false;
                                        flagCount--;
                                        if (randBoard[i].mine) {
                                            numMines++;
                                        }
                                    }
                                    else {
                                        randBoard[i].flag = true;
                                        flagCount++;
                                        if (randBoard[i].mine) {
                                            numMines--;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //draw board of just revealed tiles 
            if (!textures["tile_revealed"].loadFromFile("images/tile_revealed.png"))
                cout << "Error loading texture tile_revealed.png";

            for (int i = 0; i < rows; i++)
            {
                revealedTiles[i].setTexture(TextureManager::GetTexture("tile_revealed"));
                for (int j = 0; j < columns; j++) {
                    revealedTiles[i].setPosition(sf::Vector2f((float)(j * 32), (float)(i * 32)));
                    window.draw(revealedTiles[i]);
                }
            }

            //draw mines on top
            for (int i = 0; i < randBoard.size(); i++)
            {
                int count = 0;
                if (randBoard[i].mine == true) {
                    mines[count].setPosition(sf::Vector2f(randBoard[i].posX, randBoard[i].posY));
                    window.draw(mines[count]);
                    count++;
                }
            }

            //draw numbers for non mine tiles
            for (int i = 0; i < randBoard.size(); i++)
            {
                int count = 0;
                if (!randBoard[i].isMine()) {
                    if (randBoard[i].hasMines) {
                        randBoard[i].number.setPosition(sf::Vector2f(randBoard[i].posX, randBoard[i].posY));
                        window.draw(randBoard[i].number);
                    }
                }
            }

            //draw hidden tiles over everything
            for (int i = 0; i < randBoard.size(); i++)
            {
                if (gameEnd) {
                    for (int i = 0; i < randBoard.size(); i++) {
                        if (randBoard[i].isMine())
                            randBoard[i].revealed = true;
                    }
                }
                if (!randBoard[i].revealed) {
                    hiddenTiles[i].setTexture(TextureManager::GetTexture("tile_hidden"));
                    for (int j = 0; j < columns; j++) {
                        hiddenTiles[i].setPosition(sf::Vector2f(randBoard[i].posX, randBoard[i].posY));
                        window.draw(hiddenTiles[i]);
                    }
                }
            }

            //draw digits
            //digit = flagCount;
            digits = to_string(numMines);
            for (int i = 0; i < digits.size(); i++) {
                if (digits.substr(i, 1) == "0") {
                    digit0.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit0);
                }
                else if (digits[i] == '1') {
                    digit1.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit1);
                }
                else if (digits[i] == '2') {
                    digit2.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit2);
                }
                else if (digits[i] == '3') {
                    digit3.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit3);
                }
                else if (digits[i] == '4') {
                    digit4.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit4);
                }
                else if (digits[i] == '5') {
                    digit5.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit5);
                }
                else if (digits[i] == '6') {
                    digit6.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit6);
                }
                else if (digits[i] == '7') {
                    digit7.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit7);
                }
                else if (digits[i] == '8') {
                    digit8.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit8);
                }
                else if (digits[i] == '9') {
                    digit9.setPosition(sf::Vector2f(i * 21, boardHeight));
                    window.draw(digit9);
                }
            }


            //draw buttons
            if (!gameEnd && !gameWin) {
                window.draw(smileFace);
            }
            else if (gameWin) {
                window.draw(winFace);
            }
            else {
                window.draw(loseFace);
            }
            debug.setPosition(sf::Vector2f((boardWidth / 2) + 96, boardHeight));
            window.draw(debug);
            test1.setPosition(sf::Vector2f((boardWidth / 2) + 160, boardHeight));
            window.draw(test1);
            test2.setPosition(sf::Vector2f((boardWidth / 2) + 224, boardHeight));
            window.draw(test2);
            test3.setPosition(sf::Vector2f((boardWidth / 2) + 288, boardHeight));
            window.draw(test3);

            //draw the flags if marked for flag and not revealed
            for (int i = 0; i < randBoard.size(); i++) {
                if (randBoard[i].flag && !randBoard[i].revealed) {
                    flags[flagCount].setPosition(randBoard[i].posX, randBoard[i].posY);
                    flags[flagCount].setTexture(TextureManager::GetTexture("flag"));
                    window.draw(flags[flagCount]);
                }
            }

            //draw mines ontop of everything if debug is toggled;
            if (debugToggle) {
                for (int i = 0; i < randBoard.size(); i++)
                {
                    int count = 0;
                    if (randBoard[i].mine == true) {
                        mines[count].setPosition(sf::Vector2f(randBoard[i].posX, randBoard[i].posY));
                        window.draw(mines[count]);
                        count++;
                    }
                }
            }
            int winCount = 0;
            for (unsigned int i = 0; i < randBoard.size(); i++) {
                if (randBoard[i].revealed && !randBoard[i].mine) {
                    winCount++;
                }
            }
            if (winCount == randBoard.size() - numMines) {
                gameWin = true;
            }
            if (numMines == 0) {
                gameWin = true;
            }
            if (gameWin) {
                for (unsigned int i = 0; i < randBoard.size(); i++) { //sets all mines to flags when you win
                    if (randBoard[i].mine)
                        randBoard[i].flag = true;
                }
            }

            window.display();
        }
    }
    //delete& randBoard;
    TextureManager::Clear();

    return 0;
}
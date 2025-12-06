#include "io.h"
#include "ColoredCell.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

GameOfLife *loadFromFile(const string &filename)
{
    ifstream f(filename);
    if (!f.is_open())
    {
        cerr << "Impossible d'ouvrir le fichier: " << filename << endl;
        return nullptr;
    }

    int rows, cols;
    f >> rows >> cols;

    GameOfLife *game = new GameOfLife(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int v;
            f >> v;
            game->cells[i][j]->setAlive(v == 1);
        }
    }
    return game;
}

void runConsole(GameOfLife *game, int iterations, int delayMs)
{
    for (int k = 0; k < iterations; k++)
    {
        system("clear");

        for (int i = 0; i < game->rows; i++)
        {
            for (int j = 0; j < game->cols; j++)
            {
                cout << (game->cells[i][j]->isAlive() ? "#" : ".");
            }
            cout << "\n";
        }

        game->update();
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

void runGUI(GameOfLife *game, int cellSize, int delayMs)
{
    int speed = delayMs;

    sf::RenderWindow window(
        sf::VideoMode(game->cols * cellSize, game->rows * cellSize),
        "Game of Life");

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Add || e.key.code == sf::Keyboard::Equal)
                {
                    if (speed > 10)
                        speed -= 10;
                }
                if (e.key.code == sf::Keyboard::Dash)
                {
                    speed += 10;
                }
            }
        }

        window.clear();

        for (int i = 0; i < game->rows; i++)
        {
            for (int j = 0; j < game->cols; j++)
            {
                ColoredCell *c = dynamic_cast<ColoredCell *>(game->cells[i][j]);

                sf::RectangleShape rect(sf::Vector2f(cellSize - 1, cellSize - 1));
                rect.setPosition(j * cellSize, i * cellSize);
                rect.setFillColor(c ? c->getColor() : sf::Color::White);

                window.draw(rect);
            }
        }

        window.display();
        this_thread::sleep_for(chrono::milliseconds(speed));
        game->update();
    }
}

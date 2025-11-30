#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

//---------------------------------------------
// Cell class
//---------------------------------------------
class Cell
{
    bool alive;

public:
    Cell() : alive(false) {}
    bool isAlive() const { return alive; }
    void setAlive(bool v) { alive = v; }
};

//---------------------------------------------
// GameOfLife class
//---------------------------------------------
class GameOfLife
{
public:
    int rows, cols;
    vector<vector<Cell>> cells;

    GameOfLife(int r, int c) : rows(r), cols(c), cells(r, vector<Cell>(c)) {}

    int countAliveNeighbors(int i, int j)
    {
        int count = 0;
        for (int di = -1; di <= 1; di++)
        {
            for (int dj = -1; dj <= 1; dj++)
            {
                if (di == 0 && dj == 0)
                    continue;
                int ni = i + di;
                int nj = j + dj;
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                {
                    if (cells[ni][nj].isAlive())
                        count++;
                }
            }
        }
        return count;
    }

    void update()
    {
        vector<vector<bool>> next(rows, vector<bool>(cols, false));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int aliveNeighbors = countAliveNeighbors(i, j);
                if (cells[i][j].isAlive())
                    next[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3);
                else
                    next[i][j] = (aliveNeighbors == 3);
            }
        }

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j].setAlive(next[i][j]);
    }
};

//---------------------------------------------
// Load grid from file
//---------------------------------------------
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
            game->cells[i][j].setAlive(v == 1);
        }
    }
    return game;
}

//---------------------------------------------
// Console mode
//---------------------------------------------
void runConsole(GameOfLife *game, int iterations, int delayMs)
{
    for (int k = 0; k < iterations; k++)
    {
        system("clear");
        for (int i = 0; i < game->rows; i++)
        {
            for (int j = 0; j < game->cols; j++)
            {
                cout << (game->cells[i][j].isAlive() ? "#" : ".");
            }
            cout << "\n"; // ← FIX !!
        }
        game->update();
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

//---------------------------------------------
// GUI mode with adjustable speed
//---------------------------------------------
void runGUI(GameOfLife *game, int cellSize, int delayMs)
{
    int speed = delayMs;
    sf::RenderWindow window(sf::VideoMode(game->cols * cellSize, game->rows * cellSize), "Game of Life");
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
                if (e.key.code == sf::Keyboard::Subtract || e.key.code == sf::Keyboard::Dash)
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
                sf::RectangleShape rect(sf::Vector2f(cellSize - 1, cellSize - 1));
                rect.setPosition(j * cellSize, i * cellSize);
                rect.setFillColor(game->cells[i][j].isAlive() ? sf::Color::White : sf::Color::Black);
                window.draw(rect);
            }
        }

        window.display();
        this_thread::sleep_for(chrono::milliseconds(speed));
        game->update();
    }
}

//---------------------------------------------
// Main
//---------------------------------------------
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "Usage: ./gameOfLife <input_file> <console|gui> [iterations] [cellSize] [delayMs]\n"; // ← FIX !!
        return 1;
    }

    string filename = argv[1];
    string mode = argv[2];

    GameOfLife *game = loadFromFile(filename);
    if (!game)
        return 1;

    if (mode == "console")
    {
        int iterations = (argc >= 4 ? stoi(argv[3]) : 100);
        int delayMs = (argc >= 6 ? stoi(argv[5]) : 150);
        runConsole(game, iterations, delayMs);
    }
    else if (mode == "gui")
    {
        int cellSize = (argc >= 5 ? stoi(argv[4]) : 15);
        int delayMs = (argc >= 6 ? stoi(argv[5]) : 150);
        runGUI(game, cellSize, delayMs);
    }
    else
    {
        cout << "Mode inconnu: " << mode << endl;
    }

    delete game;
    return 0;
}

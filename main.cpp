#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
//
// ---------------------
// Classe Cell
// ---------------------
class Cell
{
protected:
    bool alive;
    sf::RectangleShape shape;

public:
    Cell(float x, float y, float size) : alive(false)
    {
        shape.setSize({size, size});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color(50, 50, 50));
        shape.setOutlineThickness(1.f);
    }

    virtual void setAlive(bool state)
    {
        alive = state;
        shape.setFillColor(alive ? sf::Color::Green : sf::Color::Black);
    }

    virtual bool isAlive() const { return alive; }

    virtual void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

    virtual void toggle()
    {
        setAlive(!alive);
    }
};

// ---------------------
// Classe Grid
// ---------------------
class Grid
{
protected:
    int rows, cols;
    float cellSize;
    vector<vector<Cell>> cells;

public:
    Grid(int r, int c, float size) : rows(r), cols(c), cellSize(size)
    {
        cells.resize(rows, vector<Cell>(cols, Cell(0, 0, cellSize)));
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cells[i][j] = Cell(j * cellSize, i * cellSize, cellSize);
            }
        }
    }

    virtual void draw(sf::RenderWindow &window)
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j].draw(window);
    }

    virtual void toggleCellAt(float x, float y)
    {
        int j = x / cellSize;
        int i = y / cellSize;
        if (i >= 0 && i < rows && j >= 0 && j < cols)
            cells[i][j].toggle();
    }

    virtual int countAliveNeighbors(int i, int j)
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
                    if (cells[ni][nj].isAlive())
                        count++;
            }
        }
        return count;
    }

    virtual void update()
    {
        vector<vector<bool>> next(rows, vector<bool>(cols, false));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int aliveNeighbors = countAliveNeighbors(i, j);
                if (cells[i][j].isAlive())
                {
                    next[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3);
                }
                else
                {
                    next[i][j] = (aliveNeighbors == 3);
                }
            }
        }

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j].setAlive(next[i][j]);
    }

    virtual void setRandomAlive(int n)
    {
        srand(time(nullptr));
        int count = 0;
        while (count < n)
        {
            int i = rand() % rows;
            int j = rand() % cols;
            if (!cells[i][j].isAlive())
            {
                cells[i][j].setAlive(true);
                count++;
            }
        }
    }
};

// ---------------------
// Classe Game
// ---------------------
class Game : public Grid
{
private:
    bool running;

public:
    Game(int r, int c, float size) : Grid(r, c, size), running(false)
    {
        setRandomAlive(50); // démarre avec 10 cellules vivantes
    }

    void toggleRunning() { running = !running; }

    void step()
    {
        if (running)
            update();
    }

    void handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            toggleCellAt(event.mouseButton.x, event.mouseButton.y);
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
                toggleRunning();
            if (event.key.code == sf::Keyboard::Enter)
                update();
        }
    }
};

// ---------------------
// Main
// ---------------------
int main()
{
    const int rows = 40, cols = 60;
    const float cellSize = 15.f;

    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "Conway Game of Life - SFML");
    window.setFramerateLimit(10);

    Game game(rows, cols, cellSize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            game.handleEvent(event, window);
        }

        game.step();

        window.clear();
        game.draw(window);
        window.display();
    }

    return 0;
}

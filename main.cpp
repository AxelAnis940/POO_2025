#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// --------------------------------------------------
// Classe Cell
// --------------------------------------------------
class Cell
{
protected:
    bool alive;
    sf::RectangleShape shape;

public:
    Cell(float x = 0, float y = 0, float size = 10) : alive(false)
    {
        shape.setSize({size, size});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color(50, 50, 50));
        shape.setOutlineThickness(1.f);
    }

    void setAlive(bool state)
    {
        alive = state;
        shape.setFillColor(alive ? sf::Color::Green : sf::Color::Black);
    }

    bool isAlive() const { return alive; }

    void draw(sf::RenderWindow &window) { window.draw(shape); }

    void toggle() { setAlive(!alive); }
};

// --------------------------------------------------
// Classe Grid
// --------------------------------------------------
class Grid
{
protected:
    int rows, cols;
    float cellSize;
    vector<vector<Cell>> cells;

public:
    Grid(int r, int c, float size) : rows(r), cols(c), cellSize(size)
    {
        cells.resize(rows, vector<Cell>(cols, Cell(0, 0, size)));

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j] = Cell(j * size, i * size, size);
    }

    void draw(sf::RenderWindow &window)
    {
        for (auto &row : cells)
            for (auto &cell : row)
                cell.draw(window);
    }

    void toggleCellAt(float x, float y)
    {
        int j = x / cellSize;
        int i = y / cellSize;
        if (i >= 0 && i < rows && j >= 0 && j < cols)
            cells[i][j].toggle();
    }

    int countAliveNeighbors(int i, int j)
    {
        int cnt = 0;
        for (int di = -1; di <= 1; di++)
            for (int dj = -1; dj <= 1; dj++)
            {
                if (di == 0 && dj == 0)
                    continue;
                int ni = i + di, nj = j + dj;
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                    if (cells[ni][nj].isAlive())
                        cnt++;
            }
        return cnt;
    }

    void update()
    {
        vector<vector<bool>> next(rows, vector<bool>(cols, false));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int n = countAliveNeighbors(i, j);
                if (cells[i][j].isAlive())
                    next[i][j] = (n == 2 || n == 3);
                else
                    next[i][j] = (n == 3);
            }
        }

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j].setAlive(next[i][j]);
    }

    void setRandomAlive(int n)
    {
        srand(time(nullptr));
        int count = 0;

        while (count < n)
        {
            int i = rand() % rows, j = rand() % cols;
            if (!cells[i][j].isAlive())
            {
                cells[i][j].setAlive(true);
                count++;
            }
        }
    }

    // Lecture depuis matrice (console)
    void setStateFromMatrix(const vector<vector<int>> &mat)
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cells[i][j].setAlive(mat[i][j] == 1);
    }

    vector<string> exportState() const
    {
        vector<string> out(rows, string(cols, '0'));
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                out[i][j] = cells[i][j].isAlive() ? '1' : '0';
        return out;
    }
};

// --------------------------------------------------
// Classe Game graphique
// --------------------------------------------------
class Game : public Grid
{
    bool running;

public:
    Game(int r, int c, float s) : Grid(r, c, s), running(false)
    {
        setRandomAlive(50);
    }

    void toggleRunning() { running = !running; }

    void step()
    {
        if (running)
            update();
    }

    void handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
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

// --------------------------------------------------
// MODE CONSOLE
// --------------------------------------------------
void runConsoleMode(const string &filename, int iterations)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Erreur : impossible d'ouvrir " << filename << endl;
        return;
    }

    string line;

    // Sauter les lignes vides avant "10 10"
    do
    {
        if (!getline(file, line))
        {
            cout << "Erreur : fichier vide.\n";
            return;
        }
    } while (line.find_first_not_of(" \t\r") == string::npos);

    stringstream header(line);
    int rows, cols;

    if (!(header >> rows >> cols))
    {
        cout << "Erreur : format incorrect dans la première ligne.\n";
        return;
    }

    vector<vector<int>> matrix(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++)
    {
        if (!getline(file, line))
        {
            cout << "Erreur : ligne manquante " << i << endl;
            return;
        }

        stringstream row(line);
        for (int j = 0; j < cols; j++)
        {
            if (!(row >> matrix[i][j]))
            {
                cout << "Erreur : données insuffisantes dans la ligne " << i << endl;
                return;
            }
        }
    }

    Grid grid(rows, cols, 10);
    grid.setStateFromMatrix(matrix);

    string outFolder = filename + "_out";
    system(("mkdir -p \"" + outFolder + "\"").c_str());

    for (int it = 0; it < iterations; it++)
    {
        string outName = outFolder + "/it_" + to_string(it) + ".txt";
        ofstream out(outName);
        auto state = grid.exportState();
        for (auto &row : state)
            out << row << "\n";
        grid.update();
    }

    cout << "Terminé : fichiers générés dans " << outFolder << endl;
}

// --------------------------------------------------
// MODE GRAPHIQUE
// --------------------------------------------------
void runGraphicMode()
{
    const int rows = 40, cols = 60;
    const float size = 15.f;

    sf::RenderWindow window(sf::VideoMode(cols * size, rows * size), "Game of Life");
    window.setFramerateLimit(10);

    Game game(rows, cols, size);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
            game.handleEvent(e, window);
        }
        game.step();
        window.clear();
        game.draw(window);
        window.display();
    }
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(int argc, char **argv)
{
    if (argc == 3)
    {
        runConsoleMode(argv[1], stoi(argv[2]));
    }
    else
    {
        runGraphicMode();
    }
    return 0;
}

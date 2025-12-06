#include <iostream>
#include "io.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "Usage: ./gameOfLife <input_file> <console|gui> [iterations] [cellSize] [delayMs]\n";
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

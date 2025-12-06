#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <string>
#include "Cell.h"

class GameOfLife
{
public:
    int rows, cols;
    std::vector<std::vector<Cell *>> cells;

    GameOfLife(int r, int c);
    virtual ~GameOfLife();

    int countAliveNeighbors(int i, int j);
    void update();
};

#endif

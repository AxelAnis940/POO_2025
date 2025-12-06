#include "GameOfLife.h"
#include "ColoredCell.h"

GameOfLife::GameOfLife(int r, int c) : rows(r), cols(c), cells(r, std::vector<Cell *>(c))
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cells[i][j] = new ColoredCell();
}

GameOfLife::~GameOfLife()
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            delete cells[i][j];
}

int GameOfLife::countAliveNeighbors(int i, int j)
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
                if (cells[ni][nj]->isAlive())
                    count++;
        }
    }
    return count;
}

void GameOfLife::update()
{
    std::vector<std::vector<bool>> next(rows, std::vector<bool>(cols, false));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int aliveNeighbors = countAliveNeighbors(i, j);

            if (cells[i][j]->isAlive())
                next[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3);
            else
                next[i][j] = (aliveNeighbors == 3);
        }
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cells[i][j]->setAlive(next[i][j]);
}

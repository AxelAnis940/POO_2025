#ifndef IO_H
#define IO_H

#include <string>
#include "GameOfLife.h"

GameOfLife *loadFromFile(const std::string &filename);
void runConsole(GameOfLife *game, int iterations, int delayMs);
void runGUI(GameOfLife *game, int cellSize, int delayMs);

#endif

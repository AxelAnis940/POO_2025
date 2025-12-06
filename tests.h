#ifndef TESTS_H
#define TESTS_H

#include "GameOfLife.h"

// Teste qu'après t itérations, la grille reste cohérente (pas de valeurs invalides)
bool testGridValidity(GameOfLife &g, int iterations);

// Teste les 4 règles du Game of Life sur une cellule
bool testGameOfLifeRules();

#endif

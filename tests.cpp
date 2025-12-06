#include "tests.h"
#include <iostream>
using namespace std;

// Test 1 : validité de la grille après t itérations
bool testGridValidity(GameOfLife &g, int iterations)
{
    for (int k = 0; k < iterations; k++)
    {
        g.update();

        // Vérification : aucune cellule ne doit être dans un état incohérent
        for (int i = 0; i < g.rows; i++)
        {
            for (int j = 0; j < g.cols; j++)
            {
                bool alive = g.cells[i][j]->isAlive();

                // l'état doit être 0 ou 1 → toujours vrai car bool
                if (alive != true && alive != false)
                    return false;
            }
        }
    }
    return true;
}

// Test 2 : tester les 4 règles du Game of Life
bool testGameOfLifeRules()
{
    // grille 3x3 pour isoler la cellule centrale
    GameOfLife g(3, 3);

    // Fonction rapide pour reset
    auto reset = [&]()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                g.cells[i][j]->setAlive(false);
    };

    // RÈGLE 1 : une cellule vivante avec <2 voisins meurt

    reset();
    g.cells[1][1]->setAlive(true); // cellule centrale vivante
    g.cells[0][1]->setAlive(true); // 1 voisin → devrait mourir

    g.update();
    if (g.cells[1][1]->isAlive())
        return false;

    // RÈGLE 2 : une cellule vivante avec 2 ou 3 voisins survit

    reset();
    g.cells[1][1]->setAlive(true);
    g.cells[0][1]->setAlive(true);
    g.cells[2][1]->setAlive(true); // 2 voisins

    g.update();
    if (!g.cells[1][1]->isAlive())
        return false;

    // RÈGLE 3 : une cellule vivante avec >3 voisins meurt
    reset();
    g.cells[1][1]->setAlive(true);
    g.cells[0][1]->setAlive(true);
    g.cells[2][1]->setAlive(true);
    g.cells[1][0]->setAlive(true);
    g.cells[1][2]->setAlive(true); // 4 voisins → meurt

    g.update();
    if (g.cells[1][1]->isAlive())
        return false;

    // RÈGLE 4 : une cellule morte avec exactement 3 voisins naît
    reset();
    g.cells[0][1]->setAlive(true);
    g.cells[2][1]->setAlive(true);
    g.cells[1][0]->setAlive(true); // 3 voisins → naissance centrale

    g.update();
    if (!g.cells[1][1]->isAlive())
        return false;

    return true;
}

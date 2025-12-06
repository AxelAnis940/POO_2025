#include "Cell.h"

Cell::Cell() : alive(false) {}

Cell::~Cell() {}

bool Cell::isAlive() const
{
    return alive;
}

void Cell::setAlive(bool v)
{
    alive = v;
}

#ifndef COLOREDCELL_H
#define COLOREDCELL_H

#include "Cell.h"
#include <SFML/Graphics.hpp>

class ColoredCell : public Cell
{
    sf::Color color;

public:
    ColoredCell();

    sf::Color getColor() const;
    void setColor(const sf::Color &c);
};

#endif

#include "ColoredCell.h"

ColoredCell::ColoredCell() : color(sf::Color::Green) {}

sf::Color ColoredCell::getColor() const
{
    return alive ? color : sf::Color::Black;
}

void ColoredCell::setColor(const sf::Color &c)
{
    color = c;
}

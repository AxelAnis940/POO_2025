#ifndef CELL_H
#define CELL_H

class Cell
{
protected:
    bool alive;

public:
    Cell();
    virtual ~Cell();

    virtual bool isAlive() const;
    virtual void setAlive(bool v);
};

#endif

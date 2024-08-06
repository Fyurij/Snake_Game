#include "Strategy.h"



bool StandartStrategy::MoveUp(Koordinates& head)
{
    --head.y;
    if (head.y > -1)
    {
        return true;
    }
    return false;
}

bool StandartStrategy::MoveDown(Koordinates& head)
{
    ++head.y;
    if (head.y < height)
    {
        return true;
    }
    return false;
}

bool StandartStrategy::MoveLeft(Koordinates& head)
{
    --head.x;
    if (head.x > -1)
    {
        return true;
    }
    return false;
}

bool StandartStrategy::MoveRight(Koordinates& head)
{
    ++head.x;
    if (head.x < width)
    {
        return true;
    }
    return false;
}



bool EndlessStrategy::MoveUp(Koordinates& head)
{
    --head.y;
    if (head.y == -1)
    {
        head.y = height - 1;
    }
    return true;
}

bool EndlessStrategy::MoveDown(Koordinates& head)
{
    ++head.y;
    if (head.y == height)
    {
        head.y = 0;
    }
    return true;
}

bool EndlessStrategy::MoveLeft(Koordinates& head)
{
    --head.x;
    if (head.x == -1)
    {
        head.x = width - 1;
    }
    return true;
}

bool EndlessStrategy::MoveRight(Koordinates& head)
{
    ++head.x;
    if (head.x == width)
    {
        head.x = 0;
    }
    return true;
}
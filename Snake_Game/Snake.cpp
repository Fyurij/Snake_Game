#include "Snake.h"
#include "Strategy.h"

#include <algorithm>
#include <vector>



void Snake::UpdateBody(const Koordinates& newHead, int foodPosX, int foodPosY)
{
    if (!EatingFood(foodPosX, foodPosY, newHead))
    {
        body.pop_back();
    }
    body.push_front(newHead);
}

bool Snake::EatingFood(int foodPosX, int foodPosY, const Koordinates& newHead) const
{
    if (newHead.x == foodPosX && newHead.y == foodPosY)
    {
        return true;
    }
    return false;
}

bool Snake::IsSnakeBody(const Koordinates& koordinates) const
{
    /*for (std::deque<Koordinates>::const_iterator i = body.cbegin(); i != body.cend() - 1; ++i)
    {
        if (*i == koordinates)
        {
            return true;
        }
    }
    return false;*/
    return std::any_of(body.cbegin(), body.cend(), [&koordinates/*or just '&' for all arguments*/](const Koordinates& bodyPart) {return bodyPart == koordinates; });
}

bool Snake::MoveUp(int foodPosX, int foodPosY)
{
    Koordinates newHead = body.front();
    if (strategy->MoveUp(newHead) && !IsSnakeBody(newHead))
    {
        UpdateBody(newHead, foodPosX, foodPosY);
        return true;
    }
    return false;
}

bool Snake::MoveDown(int foodPosX, int foodPosY)
{
    Koordinates newHead = body.front();
    if (strategy->MoveDown(newHead) && !IsSnakeBody(newHead))
    {
        UpdateBody(newHead, foodPosX, foodPosY);
        return true;
    }
    return false;
}

bool Snake::MoveLeft(int foodPosX, int foodPosY)
{
    Koordinates newHead = body.front();
    if (strategy->MoveLeft(newHead) && !IsSnakeBody(newHead))
    {
        UpdateBody(newHead, foodPosX, foodPosY);
        return true;
    }
    return false;
}

bool Snake::MoveRight(int foodPosX, int foodPosY)
{
    Koordinates newHead = body.front();
    if (strategy->MoveRight(newHead) && !IsSnakeBody(newHead))
    {
        UpdateBody(newHead, foodPosX, foodPosY);
        return true;
    }
    return false;
}

void Snake::Restart()
{
    body.clear();
    body.push_front({ 1,1 });
}

void Snake::FillField(std::vector<std::vector<FieldStatus>>& field) const
{
    field[body[0].y][body[0].x] = FieldStatus::Head;
    for (int i = 1; i < body.size(); ++i)
    {
        field[body[i].y][body[i].x] = FieldStatus::Snake;
    }
}
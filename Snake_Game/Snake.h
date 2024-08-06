#pragma once
#include <deque>
#include <memory>
#include <vector>

#include "Structs.h"
#include "Strategy.h"



class Snake
{
private:
    std::deque<Koordinates> body;
    std::unique_ptr<Strategy> strategy;
private:
    void UpdateBody(const Koordinates& newHead, int foodPosX, int foodPosY);

public:
    Snake(std::unique_ptr<Strategy> strategy_)
        :body({ {1,1} })
        , strategy(std::move(strategy_))
    {}

    bool EatingFood(int foodPosX, int foodPosY, const Koordinates& newHead) const;
    bool IsSnakeBody(const Koordinates& koordinates) const;
    bool MoveUp(int foodPosX, int foodPosY);
    bool MoveDown(int foodPosX, int foodPosY);
    bool MoveLeft(int foodPosX, int foodPosY);
    bool MoveRight(int foodPosX, int foodPosY);
    void Restart();
    void FillField(std::vector<std::vector<FieldStatus>>& field) const;
};
#pragma once
#include "Structs.h"



class Strategy
{
public:
    virtual ~Strategy() = default;
    virtual bool MoveUp(Koordinates& head) = 0;
    virtual bool MoveDown(Koordinates& head) = 0;
    virtual bool MoveLeft(Koordinates& head) = 0;
    virtual bool MoveRight(Koordinates& head) = 0;
};



class StandartStrategy : public Strategy
{
private:
    int height;
    int width;
public:
    StandartStrategy(int height_, int width_)
        :height(height_)
        , width(width_)
    {}

    bool MoveUp(Koordinates& head) override;
    bool MoveDown(Koordinates& head) override;
    bool MoveLeft(Koordinates& head) override;
    bool MoveRight(Koordinates& head) override;
};

class EndlessStrategy : public Strategy
{
private:
    int height;
    int width;
public:
    EndlessStrategy(int height_, int width_)
        :height(height_)
        , width(width_)
    {}

    bool MoveUp(Koordinates& head) override;
    bool MoveDown(Koordinates& head) override;
    bool MoveLeft(Koordinates& head) override;
    bool MoveRight(Koordinates& head) override;
};
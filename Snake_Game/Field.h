#pragma once
#include "Structs.h"
#include "Snake.h"
#include "View.h"

#include <memory>



class Field
{
private:
    int width;
    int height;
    Koordinates food;
    Snake snake;
    std::shared_ptr<View> view;
public:
    Field(int width_, int height_, const std::shared_ptr<View>& view_, std::unique_ptr<Strategy> strategy)
        :width(width_)
        , height(height_)
        , view(view_)
        , snake(std::move(strategy))
    {
        GenerateFood();
    }

    void GenerateFood();
    void Move(State& state, Moving& move);
    void Restart();
};
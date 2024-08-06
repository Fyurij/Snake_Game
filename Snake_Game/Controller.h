#pragma once
#include "Structs.h"
#include "View.h"

#include <memory>



class Controller
{
private:
    int height = 10;
    int width = 10;
    Moving move = Moving::Empty;
    State state = State::Game;
    std::shared_ptr<View> view;
public:
    Controller(const std::shared_ptr<View>& view_)
        :view(view_)
    {}

    void Run();
};
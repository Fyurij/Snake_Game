#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/input.hpp>

#include "Structs.h"
#include "Snake.h"
#include "Strategy.h"
#include "View.h"
#include "Controller.h"
#include "Memento.h"

#include <memory>
#include <iostream>

#include <string>

int main()
{
    try
    {
        std::shared_ptr<View> view;
        std::shared_ptr<Statistics> stats = std::make_shared<Statistics>();
        Controller game(view, stats);
        game.Run();
    }
    catch (std::exception& ex)
    {
        Term::cout << Term::clear_screen;
        Term::cout << Term::cursor_move(1,1) << ex.what() << std::endl;
    }
}
#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/input.hpp>

#include "Structs.h"
#include "Snake.h"
#include "Strategy.h"
#include "View.h"
#include "Controller.h"

#include <memory>
#include <iostream>



int main()
{
    try
    {
        std::shared_ptr<View> view;
        Controller game(view);
        game.Run();
    }
    catch (std::exception& ex)
    {
        Term::cout << Term::clear_screen;
        Term::cout << Term::cursor_move(1,1) << ex.what() << std::endl;
    }
}

//std::this_thread::sleep_for(std::chrono::milliseconds(100));

//const auto event = Term::Platform::read_raw();
//if (!event.empty())
#include <cpp-terminal/input.hpp>
#include <cpp-terminal/key.hpp>
#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/event.hpp>

#include "Controller.h"
#include "Strategy.h"
#include "Field.h"

#include <thread>
#include <chrono>
#include <vector>



void Controller::Run()
{
    //Term::Screen screen = Term::screen_size();
    //Field field(screen.columns() - 10, screen.rows() - 10);      
    std::unique_ptr<Strategy> strategy = std::make_unique<EndlessStrategy>(height, width);
    Field field(height, width, view, std::move(strategy));
    field.Restart();
    while (state != State::Quit)
    {
        const auto event = Term::Platform::read_raw();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        if (!event.empty())
        {
            Term::Key key(event);
            //Term::cout << key.name() << std::endl;
            if (key == Term::Key::q || key == Term::Key::Q)
            {
                state = State::Quit;
            }
            else if (key == Term::Key::r || key == Term::Key::R)
            {
                Term::cout << Term::clear_screen() << std::flush;
                field.Restart();
                move = Moving::Empty;
            }
            else if (key == Term::Key::ArrowDown)
            {
                if (move != Moving::Up)
                {
                    move = Moving::Down;
                }
            }
            else if (key == Term::Key::ArrowUp)
            {
                if (move != Moving::Down)
                {
                    move = Moving::Up;
                }
            }
            else if (key == Term::Key::ArrowRight)
            {
                if (move != Moving::Left)
                {
                    move = Moving::Right;
                }
            }
            else if (key == Term::Key::ArrowLeft)
            {
                if (move != Moving::Right)
                {
                    move = Moving::Left;
                }
            }
        }
        if (move != Moving::Empty)
        {
            field.Move(state, move);
        }
        while (state == State::Quit)
        {
            std::vector<std::vector<FieldStatus>> temp;
            view->Notify(temp, state);
            Term::Event eventQuit = Term::read_event();
            Term::Key keyQuit(eventQuit);
            if (keyQuit == Term::Key::r || keyQuit == Term::Key::R)
            {
                Term::cout << Term::clear_screen() << std::flush;
                field.Restart();
                move = Moving::Empty;
                state = State::Game;
            }
            else if (keyQuit == Term::Key::q || keyQuit == Term::Key::Q)
            {
                break;
            }
            else
            {
                Term::cout << "Incorrect input. Please try again..." << std::endl << std::endl;
            }
        }
    }
}
#include "View.h"

#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/color.hpp>

#include <string>



void View::GameOver()
{
    Term::cout << Term::clear_screen() << std::flush;
    Term::cout << Term::cursor_move(1, 1) << "GAME OVER" << std::endl << std::endl;
    Term::cout << "Press R to restart" << std::endl;
    Term::cout << "Press Q to quit" << std::endl;
}

void View::RenderField(const std::vector<std::vector<FieldStatus>>& field)
{
    std::string print;
    bool IsFoodOnField = false;
    print.append(Term::cursor_move(1, 1));
    for (int i = 0; i < field.size(); ++i)
    {
        for (int j = 0; j < field[i].size(); ++j)
        {
            if (field[i][j] == FieldStatus::Empty)
            {
                //Term::cout << Term::color_bg(Term::Color::Name::Gray) << "  ";
                print.append(Term::color_bg(Term::Color::Name::Gray) + "  ");
            }
            else if (field[i][j] == FieldStatus::Snake)
            {
                //Term::cout << Term::color_bg(Term::Color::Name::Green) << "  ";
                //print.append(Term::color_bg(Term::Color::Name::Green) + "  ");
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
                {
                    print.append(Term::color_bg(0, 170, 0) + "  ");
                }
                else
                {
                    print.append(Term::color_bg(0, 85, 0) + "  ");
                }
            }
            else if (field[i][j] == FieldStatus::Head)
            {
                print.append(Term::color_bg(0, 255, 0) + "  ");
            }
            else if (field[i][j] == FieldStatus::Food)
            {
                //Term::cout << Term::color_bg(Term::Color::Name::Red) << "  ";
                print.append(Term::color_bg(Term::Color::Name::Red) + "  ");
                IsFoodOnField = true;
            }
            else
            {
                throw std::runtime_error("Invalid status...");
            }
        }
        //Term::cout << Term::color_bg(Term::Color::Name::Default) << std::endl;
        print.append(Term::color_bg(Term::Color::Name::Default) + "\n");
    }
    Term::cout << print << std::flush;
    //Term::cout << Term::color_bg(Term::Color::Name::Default) << std::endl;
}

void View::Notify(const std::vector<std::vector<FieldStatus>>& field, State state)
{
    if (state == State::Quit)
    {
        GameOver();
    }
    else
    {
        RenderField(field);
    }
}
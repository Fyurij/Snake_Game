#include "View.h"
#include "Field.h"
#include "LevelConfig.h"

#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/cursor.hpp>
#include <cpp-terminal/color.hpp>

#include <string>
#include <memory>
#include <iostream>
#include <deque>



void View::GameOver(std::shared_ptr<Statistics> stats)
{
    Term::cout << Term::clear_screen() << std::flush;
    Term::cout << Term::cursor_move(1, 1) << "GAME OVER" << std::endl << std::endl;
    Term::cout << "Press R to restart" << std::endl;
    Term::cout << "Press Q to quit" << std::endl << std::endl;
    Term::cout << "Your final score: " << stats->GetTotalPoints() << std::endl << std::endl;
    if (stats->UpdatingScore())
    {
        Term::cout << "You beat your record. Congratulations!" << std::endl;
    }
}

void View::Pause()
{
    Term::cout << Term::clear_screen() << std::flush;
    Term::cout << Term::cursor_move(1, 1) << "PAUSE" << std::endl << std::endl;
    Term::cout << "Press R to restart" << std::endl;
    Term::cout << "Press S to save" << std::endl;
    Term::cout << "Press Q to quit" << std::endl;
    Term::cout << "Press Esc to continue" << std::endl << std::endl;
}

std::string LevelToString(Level& difficulty)
{
    switch (difficulty)
    {
    case Level::First:
        return "first";
    case Level::Second:
        return "second";
    case Level::Third:
        return "third";
    default:
        throw std::runtime_error("Unknown difficulty");
    }
}

void View::SwitchLevel(Level& difficulty)
{
    Term::cout << Term::clear_screen() << std::flush;
    if (difficulty == Level::Third)
    {
        Term::cout << Term::cursor_move(1, 1) << "Congratulations! You pass all levels" << std::endl << "Start from the first level ? (y / n)" << std::endl;
    }
    else
    {
        Term::cout << Term::cursor_move(1, 1) << "You pass " << LevelToString(difficulty) << " level" << std::endl << "Will you go next? (y/n)" << std::endl;
    }
}

void View::RenderField(const std::vector<std::vector<FieldStatus>>& field, std::shared_ptr<Statistics> stats, Level& difficulty, std::shared_ptr<LevelConfig> config)
{
    Term::cout << Term::clear_screen() << std::flush;
    std::string print;
    bool IsFoodOnField = false;
    print.append(Term::cursor_move(1, 1));
    for (int i = 0; i < field.size(); ++i)
    {
        for (int j = 0; j < field[i].size(); ++j)
        {
            if (field[i][j] == FieldStatus::Empty)
            {
                print.append(Term::color_bg(Term::Color::Name::Gray) + "  ");
            }
            else if (field[i][j] == FieldStatus::Snake)
            {
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
                print.append(Term::color_bg(Term::Color::Name::Red) + "  ");
                IsFoodOnField = true;
            }
            else if (field[i][j] == FieldStatus::Obstacle)
            {
                print.append(Term::color_bg(Term::Color::Name::BrightYellow) + "  ");
            }
            else
            {
                throw std::runtime_error("Invalid status...");
            }
        }
        print.append(Term::color_bg(Term::Color::Name::Default) + "\n");
    }
    Term::cout << print << std::flush << std::endl;
    Term::cout << "Need eat food for next level: " << config->GetGoal() - stats->GetScore() << std::endl << std::endl;
    Term::cout << "Current score: " << stats->GetPoints() << std::endl << std::endl;
    Term::cout << "Total score: " << stats->GetTotalPoints() << std::endl << std::endl;
    Term::cout << "Level: " << LevelToString(difficulty) << std::endl << std::endl;
    if (stats->GetBestScore() != 0)
    {
        Term::cout << "Your best score: " << stats->GetBestScore() << std::endl;
    }
    else
    {
        Term::cout << "Your first time. Good luck!" << std::endl;
    }
}

void View::Notify(const std::vector<std::vector<FieldStatus>>& field, State state, std::shared_ptr<Statistics> stats, Level& difficulty, std::shared_ptr<LevelConfig> config)
{
    if (state == State::Quit)
    {
        GameOver(stats);
    }
    else if (state == State::Pause)
    {
        Pause();
    }
    else
    {
        RenderField(field, stats, difficulty, config);
    }
}

void View::PregameMessage(bool error)
{
    Term::cout << Term::clear_screen() << std::flush;
    Term::cout << Term::cursor_move(1, 1);
    Term::cout << "Choose difficulty level (Press button):" << std::endl << "1 - First" << std::endl << "2 - Second" << std::endl << "3 - Third" << std::endl;
    if (error)
    {
        Term::cout << "Incorrect button... Try again..." << std::endl;
    }
}

void View::SnapshotMessage(bool error)
{
    Term::cout << Term::clear_screen() << std::flush;
    Term::cout << Term::cursor_move(1, 1);
    Term::cout << "Snapshot is loaded!" << std::endl << "Would you like to continue? (y/n)" << std::endl;
    if (error)
    {
        Term::cout << "Incorrect button... Try again..." << std::endl;
    }

}

int View::EnterSnapshotNumber(const std::deque<std::string>& snapshots)
{
    if (snapshots.empty())
    {
        Term::cout << "You don't have saves. Starting a new game..." << std::endl;
        return -1;
    }
    Term::cout << "Your saves: " << std::endl;
    for (const std::string& snapshot : snapshots)
    {
        Term::cout << snapshot << std::endl;
    }
    Term::cout << std::endl;
    Term::cout << "Download snapshot number: " << std::endl;
    int number;
    std::cin >> number;
    return number;
}
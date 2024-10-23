#include <cpp-terminal/input.hpp>
#include <cpp-terminal/key.hpp>
#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/event.hpp>

#include "Controller.h"
#include "Strategy.h"
#include "Field.h"
#include "Structs.h"
#include "Statistics.h"

#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <deque>
#include <utility>





std::unique_ptr<Strategy> SetStrategy(std::shared_ptr<LevelConfig> config, int height, int width)
{
    if (config->GetStrategy() == "Endless")
    {
        return std::make_unique<EndlessStrategy>(height, width);
    }
    else if (config->GetStrategy() == "Standart")
    {
        return std::make_unique<StandartStrategy>(height, width);
    }
    else
    {
        throw std::invalid_argument("Unknown strategy...");
    }
}

std::string AskUserName()
{
    Term::cout << "Enter your name: " << std::endl;
    std::string name;
    Term::cin >> name;
    return name;
}



void Controller::ProcessingInPregame(Term::Key& key, Memento& snapshot, std::string& userName)
{
    std::unique_ptr<Strategy> strategy;
    if (stats->GetName().empty())
    {
        stats->SetName(userName);
    }
    if (!IsSnapshot)
    {
        switch (key)
        {
        case Term::Key::One:
            config = std::make_shared<LevelConfig>(Level::First);
            difficulty = Level::First;
            break;
        case Term::Key::Two:
            config = std::make_shared<LevelConfig>(Level::Second);
            difficulty = Level::Second;
            break;
        case Term::Key::Three:
            config = std::make_shared<LevelConfig>(Level::Third);
            difficulty = Level::Third;
            break;
        default:
            error = true;
            view->PregameMessage(error);
            break;
        }
        stats->SetScoreZero();
        strategy = SetStrategy(config, height, width);
        field = std::make_unique<Field>(height, width, view, std::move(strategy), stats, config->GetNumObstacles(), difficulty);
        field->Restart(config);
    }
    else
    {
        config = std::make_shared<LevelConfig>(snapshot.GetDifficulty());
        strategy = SetStrategy(config, height, width);
        field = std::make_unique<Field>(width, height, snapshot, std::move(strategy), stats);
    }
    IsSnapshot = false;
    state = State::Game;
}



void Controller::ProcessingInGame(Term::Key& key, Memento& snapshot, std::string& userName)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(config->GetTime()));
    switch (key)
    {
    case Term::Key::Esc:
        stats->WriteToFile();
        state = State::Pause;
        break;
    case Term::Key::ArrowDown:
        if (move != Moving::Up)
        {
            move = Moving::Down;
        }
        break;
    case Term::Key::ArrowUp:
        if (move != Moving::Down)
        {
            move = Moving::Up;
        }
        break;
    case Term::Key::ArrowRight:
        if (move != Moving::Left)
        {
            move = Moving::Right;
        }
        break;
    case Term::Key::ArrowLeft:
        if (move != Moving::Right)
        {
            move = Moving::Left;
        }
        break;
    }
    if (config->GetGoal() - 1 == stats->GetScore())
    {
        checkNextLevel = true;
    }
    if (config->GetGoal() == stats->GetScore())
    {
        RenderField = false;
    }
    if (config->GetGoal() == stats->GetScore())
    {
        view->SwitchLevel(difficulty);
        move = Moving::Empty;
        switch (key)
        {
        case Term::Key::y:
        case Term::Key::Y:
            if (difficulty == Level::First)
            {
                Term::Key key(Term::Key::Two);
                ProcessingInPregame(key, snapshot, userName);
            }
            else if (difficulty == Level::Second)
            {
                Term::Key key(Term::Key::Three);
                ProcessingInPregame(key, snapshot, userName);
            }
            else if (difficulty == Level::Third)
            {
                Term::Key key(Term::Key::One);
                ProcessingInPregame(key, snapshot, userName);
            }
            checkNextLevel = false;
            RenderField = true;
            break;
        case Term::Key::n:
        case Term::Key::N:
            state = State::Quit;
            ProcessingInEnd(key);
            break;
        default:
            break;
        }
    }
    if (RenderField)
    {
        field->Move(state, move, config, checkNextLevel);
    }
}



void Controller::ProcessingInEnd(Term::Key& key)
{
    std::vector<std::vector<FieldStatus>> temp;
    view->Notify(temp, state, stats, difficulty, config);
    switch (key)
    {
    case Term::Key::Esc:
        state = State::Game;
        move = Moving::Empty;
        break;
    case Term::Key::s:
    case Term::Key::S:
    {
        Memento temp = field->CreateSnapshot(stats->GetName());
        snapshots->WriteToSnapshot(temp);
        break;
    }
    case Term::Key::r:
    case Term::Key::R:
        move = Moving::Empty;
        state = State::HelloMessage;
        stats->SetAllScoreZero();
        error = false;
        break;
    case Term::Key::q:
    case Term::Key::Q:
        stats->WriteToFile();
        stats->SetAllScoreZero();
        game = false;
        break;
    default:
        Term::cout << std::endl << "Incorrect input. Please try again..." << std::endl;
    }
}

void Controller::ProcessingWithSnapshot(Term::Key& key)
{
    switch (key)
    {
    case Term::Key::Y:
    case Term::Key::y:
        state = State::ChooseDiff;
        break;
    case Term::Key::N:
    case Term::Key::n:
        state = State::HelloMessage;
        IsSnapshot = false;
        break;
    default:
        error = true;
    }
}

std::pair<Memento, bool> Controller::ChoosingSnapshot()
{
    int number = view->EnterSnapshotNumber(snapshots->GetSnapshotsList());
    return snapshots->CreateMemento(number);
}

void Controller::Run()
{
    stats->ReadFromFile();
    std::string userName;
    if (state == State::AskName)
    {
        userName = AskUserName();
        state = State::ReadSnapshot;
    }
    GeneralConfig config;
    snapshots = std::make_unique<SnapshotManager>(userName, config.GetLimit());
    std::pair<Memento, bool> CheckSnapshot{Memento(userName),false};
    while (game)
    {
        const auto event = Term::Platform::read_raw();
        switch (state)
        {
        case State::ReadSnapshot:
            snapshots->FindAllSnapshots();
            if (!snapshots->IsEmpty())
            {
                CheckSnapshot = ChoosingSnapshot();
                IsSnapshot = CheckSnapshot.second;
                difficulty = CheckSnapshot.first.GetDifficulty();
            }
            if (IsSnapshot)
            {
                view->SnapshotMessage(error);
                state = State::WorkWithSnapshot;
            }
            else
            {
                state = State::HelloMessage;
            }
            break;
        case State::HelloMessage:
            view->PregameMessage(error);
            state = State::ChooseDiff;
            break;
        default:
            break;
        }
        if (event.type() != Term::Event::Type::Key && state != State::Game && !IsSnapshot)
        {
            continue;
        }
        Term::Key key(event);
        switch (state)
        {
        case State::WorkWithSnapshot:
            ProcessingWithSnapshot(key);
            break;
        case State::ChooseDiff:
        {
            ProcessingInPregame(key, CheckSnapshot.first, userName);
            break;
        }
        case State::Game:
        {
            ProcessingInGame(key, CheckSnapshot.first, userName);
            break;
        }
        case State::Pause:
        case State::Quit:
        {
            ProcessingInEnd(key);
            break;
        }
        }
    }
}
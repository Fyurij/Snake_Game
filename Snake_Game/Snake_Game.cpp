﻿#include <iostream>
#include <cpp-terminal/color.hpp>
#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/key.hpp>
#include <cpp-terminal/screen.hpp>
#include <cpp-terminal/event.hpp>
#include <cpp-terminal/input.hpp>
#include <vector>
#include <random>
#include <exception>
#include <string>
#include <thread>
#include <chrono>
#include <deque>



enum class FieldStatus
{
    Empty,
    Snake,
    Food
};

enum class Moving
{
    Left,
    Right,
    Up,
    Down,
    Empty
};

enum class State
{
    Game,
    Quit
};

struct Koordinates
{
    int x;
    int y;
};

bool operator==(const Koordinates& lhs, const Koordinates& rhs)
{
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}



class Snake
{
private:
    std::deque<Koordinates> body;
    int length;
    Moving move;
public:
    Snake()
        :length(1)
        ,move(Moving::Empty)
        , body({ {1,1} })
    {}

    void IncreasingLength(Koordinates koord)
    {
        body.push_front(koord);
        ++length;
    }

    bool EatingFood(int foodPosX, int foodPosY)
    {
        if (body[0].x == foodPosX && body[0].y == foodPosY)
        {
            return true;
        }
        return false;
    }

    void MoveUp(int foodPosX, int foodPosY)
    {
        body.push_front({ body[0].x, body[0].y - 1 });
        if (!EatingFood(foodPosX, foodPosY))
        {
            body.pop_back();
        }
    }

    void MoveDown(int foodPosX, int foodPosY)
    {
        body.push_front({ body[0].x, body[0].y + 1 });
        if (!EatingFood(foodPosX, foodPosY))
        {
            body.pop_back();
        }
    }

    void MoveLeft(int foodPosX, int foodPosY)
    {
        body.push_front({ body[0].x - 1, body[0].y});
        if (!EatingFood(foodPosX, foodPosY))
        {
            body.pop_back();
        }
    }

    void MoveRight(int foodPosX, int foodPosY)
    {
        body.push_front({ body[0].x + 1, body[0].y});
        if (!EatingFood(foodPosX, foodPosY))
        {
            body.pop_back();
        }
    }

    Koordinates GetHead()
    {
        return body[0];
    }

    void Restart()
    {
        body.clear();
        body.push_front({1,1});
    }

    bool IsSnakeBody(const Koordinates& square)
    {
        for (std::deque<Koordinates>::iterator i = body.begin(); i != body.end(); ++i)
        {
            if (*i == square)
            {
                return true;
            }
        }
        return false;
    }

    void FillField(std::vector<std::vector<FieldStatus>>& field)
    {
        for (int i = 0; i < body.size(); ++i)
        {
            field[body[i].y][body[i].x] = FieldStatus::Snake;
        }
    }
};



class Field
{
private:
    //std::vector<std::vector<FieldStatus>> field;
    int width;
    int height;
    Koordinates food;
    Snake snake;
public:
    Field(int width_, int height_)
        //:field(height_, std::vector<FieldStatus>(width_, FieldStatus::Empty))
        :width(width_)
        ,height(height_)
        , food({1,1})
    {
        //field[SnakePosY][SnakePosX] = FieldStatus::Snake;
    }

    void GameOver(State& state)
    {
        Term::cout << Term::clear_screen() << std::flush;
        Term::cout << Term::cursor_move(1, 1) << "GAME OVER" << std::endl << std::endl;
        state = State::Quit;
    }

    void GenerateFood()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribWidth(1, width - 1);
        std::uniform_int_distribution<> distribHeight(1, height - 1);
        while (snake.IsSnakeBody({ food.x, food.y }))
        {
            food.x = distribWidth(gen);
            food.y = distribHeight(gen);
        }
    }

    void MoveUp(State& state)
    {
        //int temp = head.y - 1;
        Koordinates newHead = snake.GetHead();
        --newHead.y;
        if (newHead.y > -1 && !snake.IsSnakeBody(newHead))
        {
            snake.MoveUp(food.x, food.y);
            //snake.MoveUp(food.x, food.y);
            //Koordinates head = snake.GetHead();
            if (snake.EatingFood(food.x, food.y))
            {
                GenerateFood();
            }
        }
        else
        {
            GameOver(state);
        }
    }

    void MoveRight(State& state)
    {
        //int temp = head.x + 1;
        Koordinates newHead = snake.GetHead();
        ++newHead.x;
        if (newHead.x < width && !snake.IsSnakeBody(newHead))
        {
            snake.MoveRight(food.x, food.y);
            //snake.MoveRight(food.x, food.y);
            //Koordinates head = snake.GetHead();
            if (snake.EatingFood(food.x, food.y))
            {
                GenerateFood();
            }
        }
        else
        {
            GameOver(state);
        }
    }

    void MoveDown(State& state)
    {
        //int temp = head.y + 1;
        Koordinates newHead = snake.GetHead();
        ++newHead.y;
        if (newHead.y < height && !snake.IsSnakeBody(newHead))
        {
            snake.MoveDown(food.x, food.y);
            //snake.MoveDown(food.x, food.y);
            //Koordinates head = snake.GetHead();
            if (snake.EatingFood(food.x, food.y))
            {
                GenerateFood();
            }
        }
        else
        {
            GameOver(state);
        }
    }

    void MoveLeft(State& state)
    {
        //int temp = head.x - 1;
        Koordinates newHead = snake.GetHead();
        --newHead.x;
        if (newHead.x > -1 && !snake.IsSnakeBody(newHead))
        {
            snake.MoveLeft(food.x, food.y);
            //snake.MoveLeft(food.x, food.y);
            //Koordinates head = snake.GetHead();
            if (snake.EatingFood(food.x, food.y))
            {
                GenerateFood();
            }
        }
        else
        {
            GameOver(state);
        }
    }

    void Restart()
    {
        snake.Restart();
        GenerateFood();
    }

    void PrintOut()
    {
        std::vector<std::vector<FieldStatus>> field (height, std::vector<FieldStatus>(width, FieldStatus::Empty));
        snake.FillField(field);
        field[food.y][food.x] = FieldStatus::Food;
        std::string print;
        bool IsFoodOnField = false;
        print.append(Term::cursor_move(1, 1));
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
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
                        print.append(Term::color_bg(0, 255, 0) + "  ");
                    }
                    else
                    {
                        print.append(Term::color_bg(0, 127, 0) + "  ");
                    }
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
        if (!IsFoodOnField)
        {
            GenerateFood();
        }
        //Term::cout << Term::color_bg(Term::Color::Name::Default) << std::endl;
    }
};



int main()
{
    try
    {
        Term::Screen screen = Term::screen_size();
        //Field field(screen.columns(), screen.rows());
        Field field(15, 15);
        Moving move = Moving::Empty;
        State state = State::Game;
        field.GenerateFood();
        while (state != State::Quit)
        {
            //field.PrintOut();
            const auto event = Term::Platform::read_raw();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            if (!event.empty())
            {
                Term::Key key(event);
                if (key == Term::Key::q || key == Term::Key::Q)
                {
                    field.GameOver(state);
                }
                else if (key == Term::Key::r || key == Term::Key::R)
                {
                    field.Restart();
                    move = Moving::Empty;
                    Term::cout << Term::clear_screen() << std::flush;
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
            switch (move)
            {
            case Moving::Left:
                field.MoveLeft(state);
                break;
            case Moving::Right:
                field.MoveRight(state);
                break;
            case Moving::Up:
                field.MoveUp(state);
                break;
            case Moving::Down:
                field.MoveDown(state);
                break;
            default:
                break;
            }
            if (state == State::Game)
            {
                field.PrintOut();
            }
            //Term::cout << Term::clear_screen() << std::flush;
            while (state == State::Quit)
            {
                Term::cout << "Press R to restart" << std::endl;
                Term::cout << "Press Q to quit" << std::endl;
                Term::Event eventQuit = Term::read_event();
                Term::Key keyQuit(eventQuit);
                if (keyQuit == Term::Key::r || keyQuit == Term::Key::R)
                {
                    field.Restart();
                    move = Moving::Empty;
                    Term::cout << Term::clear_screen() << std::flush;
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
    catch (std::exception& ex)
    {
        Term::cout << Term::clear_screen;
        Term::cout << Term::cursor_move(1,1) << ex.what() << std::endl;
    }
}

//std::this_thread::sleep_for(std::chrono::milliseconds(100));

//const auto event = Term::Platform::read_raw();
//if (!event.empty())
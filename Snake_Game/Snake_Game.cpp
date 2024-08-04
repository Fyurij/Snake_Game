#include <iostream>
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
#include <memory>
#include <algorithm>



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
        ,width(width_)
    {}

    bool MoveUp(Koordinates& head) override
    {
        --head.y;
        if (head.y > -1)
        {
            return true;
        }
        return false;
    }

    bool MoveDown(Koordinates& head) override
    {
        ++head.y;
        if (head.y < height)
        {
            return true;
        }
        return false;
    }

    bool MoveLeft(Koordinates& head) override
    {
        --head.x;
        if (head.x > -1)
        {
            return true;
        }
        return false;
    }

    bool MoveRight(Koordinates& head) override
    {
        ++head.x;
        if (head.x < width)
        {
            return true;
        }
        return false;
    }
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

    bool MoveUp(Koordinates& head) override
    {
        --head.y;
        if (head.y == -1)
        {
            head.y = height - 1;
        }
        return true;
    }

    bool MoveDown(Koordinates& head) override
    {
        ++head.y;
        if (head.y == height)
        {
            head.y = 0;
        }
        return true;
    }

    bool MoveLeft(Koordinates& head) override
    {
        --head.x;
        if (head.x == -1)
        {
            head.x = width - 1;
        }
        return true;
    }

    bool MoveRight(Koordinates& head) override
    {
        ++head.x;
        if (head.x == width)
        {
            head.x = 0;
        }
        return true;
    }
};



class Snake
{
private:
    std::deque<Koordinates> body;
    std::unique_ptr<Strategy> strategy;
private:
    void UpdateBody(const Koordinates& newHead, int foodPosX, int foodPosY)
    {
        if (!EatingFood(foodPosX, foodPosY, newHead))
        {
            body.pop_back();
        }
        body.push_front(newHead);
    }

public:
    Snake(std::unique_ptr<Strategy> strategy_)
        :body({ {1,1} })
        ,strategy(std::move(strategy_))
    {}

    bool EatingFood(int foodPosX, int foodPosY, const Koordinates& newHead) const
    {
        if (newHead.x == foodPosX && newHead.y == foodPosY)
        {
            return true;
        }
        return false;
    }

    bool IsSnakeBody(const Koordinates& koordinates) const
    {
        /*for (std::deque<Koordinates>::const_iterator i = body.cbegin(); i != body.cend() - 1; ++i)
        {
            if (*i == koordinates)
            {
                return true;
            }
        }
        return false;*/
        return std::any_of(body.cbegin(), body.cend() - 1, [&koordinates/*or just '&' for all arguments*/](const Koordinates& bodyPart) {return bodyPart == koordinates; });
    }

    bool MoveUp(int foodPosX, int foodPosY)
    {
        Koordinates newHead = body.front();
        if (strategy->MoveUp(newHead) && !IsSnakeBody(newHead))
        {
            UpdateBody(newHead, foodPosX, foodPosY);
            return true;
        }
        return false;
    }

    bool MoveDown(int foodPosX, int foodPosY)
    {
        Koordinates newHead = body.front();
        if (strategy->MoveDown(newHead) && !IsSnakeBody(newHead))
        {
            UpdateBody(newHead, foodPosX, foodPosY);
            return true;
        }
        return false;
    }

    bool MoveLeft(int foodPosX, int foodPosY)
    {
        Koordinates newHead = body.front();
        if (strategy->MoveLeft(newHead) && !IsSnakeBody(newHead))
        {
            UpdateBody(newHead, foodPosX, foodPosY);
            return true;
        }
        return false;
    }

    bool MoveRight(int foodPosX, int foodPosY)
    {
        Koordinates newHead = body.front();
        if (strategy->MoveRight(newHead) && !IsSnakeBody(newHead))
        {
            UpdateBody(newHead, foodPosX, foodPosY);
            return true;
        }
        return false;
    }

    void Restart()
    {
        body.clear();
        body.push_front({1,1});
    }

    void FillField(std::vector<std::vector<FieldStatus>>& field) const
    {
        for (int i = 0; i < body.size(); ++i)
        {
            field[body[i].y][body[i].x] = FieldStatus::Snake;
        }
    }
};



class View
{
private:
    void GameOver()
    {
        Term::cout << Term::clear_screen() << std::flush;
        Term::cout << Term::cursor_move(1, 1) << "GAME OVER" << std::endl << std::endl;
        Term::cout << "Press R to restart" << std::endl;
        Term::cout << "Press Q to quit" << std::endl;
    }

    void RenderField(const std::vector<std::vector<FieldStatus>>& field)
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
        //Term::cout << Term::color_bg(Term::Color::Name::Default) << std::endl;
    }
public:
    void Notify(const std::vector<std::vector<FieldStatus>>& field, State state)
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
};



class Field
{
private:
    //std::vector<std::vector<FieldStatus>> field;
    int width;
    int height;
    Koordinates food;
    Snake snake;
    std::shared_ptr<View> view;
public:
    Field(int width_, int height_, const std::shared_ptr<View>& view_, std::unique_ptr<Strategy> strategy)
        //:field(height_, std::vector<FieldStatus>(width_, FieldStatus::Empty))
        :width(width_)
        ,height(height_)
        ,view(view_)
        ,snake(std::move(strategy))
    {
        GenerateFood();
        //field[SnakePosY][SnakePosX] = FieldStatus::Snake;
    }

    void GenerateFood()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribWidth(1, width - 1);
        std::uniform_int_distribution<> distribHeight(1, height - 1);
        food.x = distribWidth(gen);
        food.y = distribHeight(gen);
        while (snake.IsSnakeBody({ food.x, food.y }))
        {
            food.x = distribWidth(gen);
            food.y = distribHeight(gen);
        }
    }

    void Move(State& state, Moving& move)
    {
        bool WasMoving = false;
        switch (move)
        {
        case Moving::Left:
            WasMoving = snake.MoveLeft(food.x, food.y);
            break;
        case Moving::Right:
            WasMoving = snake.MoveRight(food.x, food.y);
            break;
        case Moving::Up:
            WasMoving = snake.MoveUp(food.x, food.y);
            break;
        case Moving::Down:
            WasMoving = snake.MoveDown(food.x, food.y);
            break;
        default:
            break;
        }
        std::vector<std::vector<FieldStatus>> field(height, std::vector<FieldStatus>(width, FieldStatus::Empty));
        if (WasMoving)
        {
            if (snake.IsSnakeBody({ food.x, food.y }))
            {
                GenerateFood();
            }
            field[food.y][food.x] = FieldStatus::Food;
            snake.FillField(field);
        }
        else
        {
            state = State::Quit;
        }
        view->Notify(field, state);
    }

    void Restart()
    {
        std::vector<std::vector<FieldStatus>> field(height, std::vector<FieldStatus>(width, FieldStatus::Empty));
        snake.Restart();
        snake.FillField(field);
        GenerateFood();
        field[food.y][food.x] = FieldStatus::Food;
        view->Notify(field, State::Game);
    }
};



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

    void Run()
    {
        //Term::Screen screen = Term::screen_size();
        //Field field(screen.columns() - 10, screen.rows() - 10);      
        std::unique_ptr<Strategy> strategy = std::make_unique<EndlessStrategy>(height, width);
        Field field(height, width, view, std::move(strategy));
        field.Restart();
        while (state != State::Quit)
        {
            //field.PrintOut();
            const auto event = Term::Platform::read_raw();
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
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
                //field.Move(state, move)
            }
            if (move != Moving::Empty)
            {
                field.Move(state, move);
            }
            //Term::cout << Term::clear_screen() << std::flush;
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
};



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
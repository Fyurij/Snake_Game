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

struct Koordinates
{
    int x;
    int y;
};



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

    Moving MoveUp()
    {
        if (move != Moving::Down)
        {
            move = Moving::Up;
        }
        return move;
    }

    Moving MoveDown()
    {
        if (move != Moving::Up)
        {
            move = Moving::Down;
        }
        return move;
    }

    Moving MoveLeft()
    {
        if (move != Moving::Right)
        {
            move = Moving::Left;
        }
        return move;
    }

    Moving MoveRight()
    {
        if (move != Moving::Left)
        {
            move = Moving::Right;
        }
        return move;
    }
};



class Field
{
private:
    std::vector<std::vector<FieldStatus>> field;
    int width;
    int height;
    int SnakePosX;
    int SnakePosY;
    int FoodPosX;
    int FoodPosY;
public:
    Field(int width_, int height_)
        :field(height_, std::vector<FieldStatus>(width_, FieldStatus::Empty))
        ,width(width_)
        ,height(height_)
        ,SnakePosX(1)
        ,SnakePosY(1)
    {
        field[SnakePosY][SnakePosX] = FieldStatus::Snake;
    }

    void GameOver(int& quit)
    {
        Term::cout << Term::clear_screen() << std::flush;
        Term::cout << Term::cursor_move(1, 1) << "GAME OVER" << std::endl << std::endl;
        ++quit;
    }

    void MoveUp(int& quit, Snake& snake)
    {
        int temp = SnakePosY - 1;
        if (temp > -1)
        {
            field[SnakePosY][SnakePosX] = FieldStatus::Empty;
            SnakePosY = temp;
            if (field[SnakePosY][SnakePosX] == FieldStatus::Food)
            {
                snake.IncreasingLength({SnakePosY, SnakePosX});
            }
            field[SnakePosY][SnakePosX] = FieldStatus::Snake;
        }
        else
        {
            GameOver(quit);
        }
    }

    void MoveRight(int& quit, Snake& snake)
    {
        int temp = SnakePosX + 1;
        if (temp < width)
        {
            field[SnakePosY][SnakePosX] = FieldStatus::Empty;
            SnakePosX = temp;
            if (field[SnakePosY][SnakePosX] == FieldStatus::Food)
            {
                snake.IncreasingLength({ SnakePosY, SnakePosX });
            }
            field[SnakePosY][SnakePosX] = FieldStatus::Snake;
        }
        else
        {
            GameOver(quit);
        }
    }

    void MoveDown(int& quit, Snake& snake)
    {
        int temp = SnakePosY + 1;
        if (temp < height)
        {
            field[SnakePosY][SnakePosX] = FieldStatus::Empty;
            SnakePosY = temp;
            if (field[SnakePosY][SnakePosX] == FieldStatus::Food)
            {
                snake.IncreasingLength({ SnakePosY, SnakePosX });
            }
            field[SnakePosY][SnakePosX] = FieldStatus::Snake;
        }
        else
        {
            GameOver(quit);
        }
    }

    void MoveLeft(int& quit, Snake& snake)
    {
        int temp = SnakePosX - 1;
        if (temp > -1)
        {
            field[SnakePosY][SnakePosX] = FieldStatus::Empty;
            SnakePosX = temp;
            if (field[SnakePosY][SnakePosX] == FieldStatus::Food)
            {
                snake.IncreasingLength({ SnakePosY, SnakePosX });
            }
            field[SnakePosY][SnakePosX] = FieldStatus::Snake;
        }
        else
        {
            GameOver(quit);
        }
    }

    void GenerateFood()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribWidth(1, width - 1); 
        std::uniform_int_distribution<> distribHeight(1, height - 1);
        FoodPosX = distribWidth(gen);
        FoodPosY = distribHeight(gen);
        if (field[FoodPosY][FoodPosX] != FieldStatus::Snake)
        {
            field[FoodPosY][FoodPosX] = FieldStatus::Food;
        }
        else
        {
            while (field[FoodPosY][FoodPosX] == FieldStatus::Snake)
            {
                int FoodPosX = distribWidth(gen);
                int FoodPosY = distribHeight(gen);
            }
            field[FoodPosY][FoodPosX] = FieldStatus::Food;
        }
    }

    void Restart()
    {
        field[SnakePosY][SnakePosX] = FieldStatus::Empty;
        field[FoodPosY][FoodPosX] = FieldStatus::Empty;
        SnakePosX = 1;
        SnakePosY = 1;
        field[SnakePosY][SnakePosX] = FieldStatus::Snake;
        GenerateFood();
    }

    void PrintOut()
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
                    print.append(Term::color_bg(Term::Color::Name::Green) + "  ");
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
        Snake snake;
        Moving move = Moving::Empty;
        int quit = 0;
        field.GenerateFood();
        while (quit == 0)
        {
            //field.PrintOut();
            const auto event = Term::Platform::read_raw();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            if (!event.empty())
            {
                Term::Key key(event);
                if (key == Term::Key::q || key == Term::Key::Q)
                {
                    field.GameOver(quit);
                }
                else if (key == Term::Key::ArrowDown)
                {
                    move = snake.MoveDown();
                }
                else if (key == Term::Key::ArrowUp)
                {
                    move = snake.MoveUp();
                }
                else if (key == Term::Key::ArrowRight)
                {
                    move = snake.MoveRight();
                }
                else if (key == Term::Key::ArrowLeft)
                {
                    move = snake.MoveLeft();
                }
            }
            switch (move)
            {
            case Moving::Left:
                field.MoveLeft(quit, snake);
                break;
            case Moving::Right:
                field.MoveRight(quit, snake);
                break;
            case Moving::Up:
                field.MoveUp(quit, snake);
                break;
            case Moving::Down:
                field.MoveDown(quit, snake);
                break;
            case Moving::Empty:
                break;
            default:
                break;
            }
            if (quit == 0)
            {
                field.PrintOut();
            }
            //Term::cout << Term::clear_screen() << std::flush;
            while (quit == 1)
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
                    --quit;
                }
                else if (keyQuit == Term::Key::q || keyQuit == Term::Key::Q)
                {
                    ++quit;
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
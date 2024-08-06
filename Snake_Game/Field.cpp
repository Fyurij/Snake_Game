#include "Field.h"
#include "Structs.h"

#include <random>
#include <vector>



void Field::GenerateFood()
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

void Field::Move(State & state, Moving & move)
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

void Field::Restart()
{
    std::vector<std::vector<FieldStatus>> field(height, std::vector<FieldStatus>(width, FieldStatus::Empty));
    snake.Restart();
    snake.FillField(field);
    GenerateFood();
    field[food.y][food.x] = FieldStatus::Food;
    view->Notify(field, State::Game);
}
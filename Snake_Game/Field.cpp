#include "Field.h"
#include "Structs.h"
#include "LevelConfig.h"

#include <random>
#include <vector>



void Field::GenerateObstacles(int numObstacles)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(1, width - 1);
    std::uniform_int_distribution<> distribHeight(1, height - 1);
    while (obstacles.size() != numObstacles)
    {
        Koordinates obstacle{.x = distribWidth(gen), .y = distribHeight(gen) };
        if (!snake.IsSnakeBody({ obstacle.x, obstacle.y }))
        {
            obstacles.push_back(obstacle);
        }
    }
}

bool Field::IsObstacle()
{
    for (int i = 0; i < obstacles.size(); ++i)
    {
        if (snake.IsSnakeBody(obstacles[i]))
        {
            return true;
        }
    }
    return false;
}

bool Field::IsFoodOnObstacle()
{
    /*for (int i = 0; i < obstacles.size(); ++i)
    {
        if (food == obstacles[i])
        {
            return true;
        }
    }
    return false;*/

    return std::any_of(obstacles.cbegin(), obstacles.cend(), [this](const Koordinates& obstacle) {return food == obstacle; });
}

void Field::GenerateFood()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(1, width - 1);
    std::uniform_int_distribution<> distribHeight(1, height - 1);
    food.x = distribWidth(gen);
    food.y = distribHeight(gen);
    while (snake.IsSnakeBody({ food.x, food.y }) && IsFoodOnObstacle())
    {
        food.x = distribWidth(gen);
        food.y = distribHeight(gen);
    }
}

void Field::Move(State & state, Moving & move, std::shared_ptr<LevelConfig> config, bool checkNextLevel)
{
    bool WasMoving = true;
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
    for (int i = 0; i < obstacles.size(); ++i)
    {
        field[obstacles[i].y][obstacles[i].x] = FieldStatus::Obstacle;
    }
    if (WasMoving && !IsObstacle())
    {
        if (snake.IsSnakeBody({ food.x, food.y }))
        {
            stats->IncreasingScore();
            if (checkNextLevel == false)
            {
                GenerateFood();
            }
        }
        field[food.y][food.x] = FieldStatus::Food;
        snake.FillField(field);
    }
    else
    {
        state = State::Quit;
    }
    view->Notify(field, state, stats, difficulty, config);
}

void Field::Restart(std::shared_ptr<LevelConfig> config)
{
    int score = 0;
    std::vector<std::vector<FieldStatus>> field(height, std::vector<FieldStatus>(width, FieldStatus::Empty));
    for (int i = 0; i < obstacles.size(); ++i)
    {
        field[obstacles[i].y][obstacles[i].x] = FieldStatus::Obstacle;
    }
    snake.Restart();
    snake.FillField(field);
    GenerateFood();
    field[food.y][food.x] = FieldStatus::Food;
    view->Notify(field, State::Game, stats, difficulty, config);
}

Memento Field::CreateSnapshot(const std::string& name)
{
    return Memento(name, stats->GetScore(), stats->GetTotalScore(), difficulty, snake.GetSnakeKoordinates(), obstacles, food);
}

void Field::RestoreFromSnapshot(Memento& snapshot)
{
    difficulty = snapshot.difficulty;
    obstacles = snapshot.obstacles;
    food = snapshot.food;
    stats->SetScore(snapshot.eatenFood);
    stats->SetTotalScore(snapshot.totalScore);
    snake.SetSnakeKoordinates(snapshot.snake);
}
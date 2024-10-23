#pragma once
#include "Structs.h"
#include "Snake.h"
#include "View.h"
#include "Statistics.h"
#include "LevelConfig.h"
#include "Memento.h"

#include <memory>



class Field
{
private:
    int width;
    int height;
    Koordinates food;
    Snake snake;
    std::vector<Koordinates> obstacles;
    std::shared_ptr<View> view;
    std::shared_ptr<Statistics> stats;
    Level difficulty;

private:
    void RestoreFromSnapshot(Memento& snapshot);

public:
    Field(int width_, int height_, Memento& snapshot, std::unique_ptr<Strategy> strategy, std::shared_ptr<Statistics> stats_)
        :width(width_)
        , height(height_)
        , snake(snapshot.snake, std::move(strategy))
        , stats(stats_)
    {
        RestoreFromSnapshot(snapshot);
    }

    Field(int width_, int height_, const std::shared_ptr<View>& view_, std::unique_ptr<Strategy> strategy, std::shared_ptr<Statistics>& stats_, int numObstacles, Level difficulty_)
        :width(width_)
        , height(height_)
        , view(view_)
        , snake(std::move(strategy))
        , stats(stats_)
        , difficulty(difficulty_)
    {
        GenerateFood();
        GenerateObstacles(numObstacles);
    }

    void GenerateObstacles(int numObstacles);
    void GenerateFood();
    bool IsObstacle();
    bool IsFoodOnObstacle();
    void Move(State& state, Moving& move, std::shared_ptr<LevelConfig> config, bool checkNextLevel);
    void Restart(std::shared_ptr<LevelConfig> config);
    Memento CreateSnapshot(const std::string& name);
};
#pragma once
#include <string>
#include <map>
#include <fstream>

#include "Structs.h"

class GeneralConfig
{
private:
    int savesLimit;

public:
    GeneralConfig()
    {
        std::ifstream config("Config/Config.txt");
        if (!config.is_open())
        {
            throw std::runtime_error("General config is not opened...");
        }
        config >> savesLimit;
    }

    int GetLimit()
    {
        return savesLimit;
    }
};



class LevelConfig
{
private:
    int numObstacles;
    int time;
    std::string typeStrategy;
    int goal;
public:
    LevelConfig(Level difficulty)
    {
        const std::map<Level, const char*> ConfigFileNames = {
                                                                    {Level::First,   "Config/ConfigEasy.txt"},
                                                                    {Level::Second, "Config/ConfigNormal.txt"},
                                                                    {Level::Third,   "Config/ConfigHard.txt"}
        };
        auto path = ConfigFileNames.at(difficulty);
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Config is not opened...");
        }
        std::string temp;
        file >> temp >> time >> temp >> typeStrategy >> temp >> goal >> temp >> numObstacles;
    }

    int GetTime()
    {
        return time;
    }

    int GetGoal()
    {
        return goal;
    }

    std::string GetStrategy()
    {
        return typeStrategy;
    }

    int GetNumObstacles()
    {
        return numObstacles;
    }
};
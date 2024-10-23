#pragma once
#include <string>
#include <map>



class Statistics
{
private:
    std::map<std::string, int> leaderboard;
    std::string name;
    int totalScore;
    int currentScore;
    int koefficent;
public:
    Statistics()
        :totalScore(0)
        , currentScore(0)
        , koefficent(50)
    {}

    void SetName(std::string name_)
    {
        name = name_;
    }

    std::string GetName()
    {
        return name;
    }

    int GetScore()
    {
        return currentScore;
    }

    int GetTotalScore()
    {
        return totalScore;
    }

    int GetPoints()
    {
        return currentScore * koefficent;
    }

    int GetTotalPoints()
    {
        return totalScore * koefficent;
    }

    int GetBestScore()
    {
        if (leaderboard.contains(name))
        {
            return leaderboard[name];
        }
        return 0;
    }

    void IncreasingScore()
    {
        ++currentScore;
        ++totalScore;
    }

    void SetScoreZero()
    {
        currentScore = 0;
    }

    void SetAllScoreZero()
    {
        currentScore = 0;
        totalScore = 0;
    }

    void SetScore(int score)
    {
        currentScore = score;
    }

    void SetTotalScore(int totalpoints)
    {
        totalScore = totalpoints;
    }

    void ReadFromFile();
    void WriteToFile();
    bool UpdatingScore();
};
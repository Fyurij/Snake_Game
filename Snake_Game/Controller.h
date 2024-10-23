#pragma once
#include "Structs.h"
#include "View.h"
#include "Strategy.h"
#include "Field.h"
#include "Statistics.h"
#include "LevelConfig.h"

#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <exception>
#include <deque>
#include <utility>



class Controller
{
private:
    std::unique_ptr<Field> field;
    std::unique_ptr<SnapshotManager> snapshots;

    std::shared_ptr<View> view;
    std::shared_ptr<LevelConfig> config;
    std::shared_ptr<Statistics> stats;

    Moving move = Moving::Empty;
    State state = State::AskName;
    Level difficulty;

    int height = 15;
    int width = 15;

    bool error = false;
    bool game = true;
    bool checkNextLevel = false;
    bool IsSnapshot = false;
    bool RenderField = true;
public:
    Controller(const std::shared_ptr<View>& view_, const std::shared_ptr<Statistics>& stats_)
        :view(view_)
        ,stats(stats_)
    {}

    void ProcessingInPregame(Term::Key& key, Memento& snapshot, std::string& userName);
    void ProcessingInGame(Term::Key& key, Memento& snapshot, std::string& userName);
    void ProcessingInEnd(Term::Key& key);
    void ProcessingWithSnapshot(Term::Key& key);
    std::pair<Memento, bool> ChoosingSnapshot();
    void Run();
};
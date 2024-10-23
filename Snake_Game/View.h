#pragma once
#include <vector>
#include <memory>
#include <deque>

#include "Structs.h"
#include "Statistics.h"
#include "LevelConfig.h"



class View
{
private:
    void GameOver(std::shared_ptr<Statistics> stats);
    void RenderField(const std::vector<std::vector<FieldStatus>>& field, std::shared_ptr<Statistics> stats, Level& difficulty, std::shared_ptr<LevelConfig> config);
    void Pause();
public:
    void Notify(const std::vector<std::vector<FieldStatus>>& field, State state, std::shared_ptr<Statistics> stats, Level& difficulty, std::shared_ptr<LevelConfig> config);
    void PregameMessage(bool error);
    void SwitchLevel(Level& difficulty);
    void SnapshotMessage(bool error);
    int EnterSnapshotNumber(const std::deque<std::string>& snapshots);
};
#pragma once
#include <vector>

#include "Structs.h"


class View
{
private:
    void GameOver();
    void RenderField(const std::vector<std::vector<FieldStatus>>& field);
    
public:
    void Notify(const std::vector<std::vector<FieldStatus>>& field, State state);
};
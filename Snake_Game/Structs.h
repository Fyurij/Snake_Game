#pragma once



enum class FieldStatus
{
    Empty,
    Snake,
    Head,
    Food,
    Obstacle
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
    AskName,
    ReadSnapshot,
    WorkWithSnapshot,
    ChooseSnapshot,
    HelloMessage,
    ChooseDiff,
    Game,
    Pause,
    Quit
};

enum class Level
{
    First,
    Second,
    Third,
    Fourth,
    Fifth
};

struct Koordinates
{
    int x;
    int y;
};

inline bool operator==(const Koordinates& lhs, const Koordinates& rhs)
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}
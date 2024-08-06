#pragma once



enum class FieldStatus
{
    Empty,
    Snake,
    Head,
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

enum class State
{
    Game,
    Quit
};

struct Koordinates
{
    int x;
    int y;
    friend bool operator==(const Koordinates& lhs, const Koordinates& rhs)
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
};
#include <iomanip>
#include <ostream>

#include "ChessBroad.h"

Position operator+(Position left, Position right)
{
    return {left.row + right.row, left.column + right.column};
}
Position operator-(Position left, Position right)
{
    return {left.row - right.row, left.column - right.column};
}
Position operator*(int n, Position pos)
{
    return {n * pos.row, n * pos.column};
}
Position operator*(Position pos, int n)
{
    return n * pos;
}

bool operator==(Position left, Position right)
{
    return left.row == right.row && left.column == right.column;
}
bool operator!=(Position left, Position right)
{
    return !(left == right);
}

std::ostream &operator<<(std::ostream &os, Chess ch)
{
    switch (ch)
    {
    case Chess::empty:
        os << ' ';
        break;
    case Chess::first_player:
        os << 'O';
        break;
    case Chess::second_player:
        os << 'X';
        break;
    default:
        break;
    }
    return os;
}

ChessBroad::ChessBroad() : _broad{}
{
    for (auto &arr : _broad)
        arr.fill(Chess::empty);
}
void ChessBroad::emplace(Position pos, Chess ch)
{
    _broad[pos.row][pos.column] = ch;
}
bool ChessBroad::full() const
{
    for (int i = 0; i < ChessBroad::size; ++i)
        for (int j = 0; j < ChessBroad::size; ++j)
            if (_broad[i][j] == Chess::empty)
                return false;
    return true;
}
Chess ChessBroad::get(Position pos) const
{
    return _broad[pos.row][pos.column];
}
bool ChessBroad::in_range(Position pos)
{
    return pos.row >= 0 && pos.row < size && pos.column >= 0 &&
           pos.column < size;
}
std::ostream &operator<<(std::ostream &os, const ChessBroad &broad)
{

    os << R"(
   c 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
  r|-----------------------------------------------------------|
)";
    for (int i = 0; i < ChessBroad::size; ++i)
    {
        os << std::setw(3) << i + 1 << '|';
        for (int j = 0; j < ChessBroad::size; ++j)
            os << ' ' << broad.get({i, j}) << " |" << std::flush;
        os << "\n   "
              "|-----------------------------------------------------------|\n";
    }
    os << std::flush;
    return os;
}

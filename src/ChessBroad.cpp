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
std::ostream &operator<<(std::ostream &os, Chess ch)
{
    return os << static_cast<char>(ch);
}

void ChessBroad::emplace(Position pos, Chess ch)
{
    if (_broad[pos.row][pos.column] != Chess::empty)
        throw InvalidPosition{};
    _broad[pos.row][pos.column] = ch;
}
bool ChessBroad::full() const
{
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
            if (_broad[i][j] != Chess::empty)
                return false;
    return true;
}
bool ChessBroad::win_game(Position hint) const
{
    auto check = [this](Position delta, Position origin) {
        auto in_range = [](Position pos) { return pos.row >= 0 && pos.row < 15 && pos.column >= 0 && pos.column < 15; };
        Chess origin_ch = get(origin);
        int count = 0;
        for (Position pos = origin; in_range(pos) && get(pos) == origin_ch; pos = pos + delta)
            ++count;
        for (Position pos = origin - delta; in_range(pos) && get(pos) == origin_ch; pos = pos - delta)
            ++count;
        return count == 5;
    };
    return check({1, 0}, hint) || check({1, 1}, hint) || check({0, 1}, hint) || check({1, -1}, hint);
}
Chess ChessBroad::get(Position pos) const
{
    return _broad[pos.row][pos.column];
}
std::ostream &operator<<(std::ostream &os, const ChessBroad &broad)
{

    os << R"(
   c 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
  r|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
)";
    for (int i = 0; i < broad_size; ++i)
    {
        os << std::setw(3) << i + 1 << '|';
        for (int j = 0; j < broad_size; ++j)
            os << ' ' << broad.get({i, j}) << " |";
        os << "\n   |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|\n";
    }
    os << std::flush;
    return os;
}
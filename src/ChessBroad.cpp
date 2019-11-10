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
bool operator<(Position left, Position right)
{
    return left.row < right.row || (left.row == right.row && left.column < right.column);
}

ChessBroad::ChessBroad()
{
    for (auto &arr : _broad)
        arr.fill(Chess::empty);
}
void ChessBroad::emplace(Position pos, Chess ch)
{

    auto in_range = [](Position pos) {
        return pos.row >= 0 && pos.row < broad_size && pos.column >= 0 && pos.column < broad_size;
    };
    if (!in_range(pos) || (_broad[pos.row][pos.column] != Chess::empty && ch != Chess::empty))
        throw InvalidPosition{};
    _broad[pos.row][pos.column] = ch;
}
bool ChessBroad::full() const
{
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
            if (_broad[i][j] == Chess::empty)
                return false;
    return true;
}
bool ChessBroad::win_game(Position hint) const
{
    // Check with a certain direction.
    auto check = [this](Position delta, Position origin) {
        auto in_range = [](Position pos) {
            return pos.row >= 0 && pos.row < broad_size && pos.column >= 0 && pos.column < broad_size;
        };
        Chess origin_ch = get(origin);
        int count = 0;
        for (Position pos = origin; in_range(pos) && get(pos) == origin_ch; pos = pos + delta)
            ++count;
        for (Position pos = origin - delta; in_range(pos) && get(pos) == origin_ch; pos = pos - delta)
            ++count;
        return count == 5;
    };

    // Check with all four directions.
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
  r|-----------------------------------------------------------|
)";
    for (int i = 0; i < broad_size; ++i)
    {
        os << std::setw(3) << i + 1 << '|';
        for (int j = 0; j < broad_size; ++j)
            os << ' ' << broad.get({i, j}) << " |" << std::flush;
        os << "\n   |-----------------------------------------------------------|\n";
    }
    os << std::flush;
    return os;
}
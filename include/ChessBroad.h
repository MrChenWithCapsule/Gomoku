#pragma once
#include <array>
#include <exception>
#include <iosfwd>

constexpr int broad_size = 15;

struct Position
{
    int row;
    int column;
};

Position operator+(Position left, Position right);
Position operator-(Position left, Position right);
Position operator*(int n, Position pos);
bool operator<(Position left, Position right);

enum class Chess : char
{
    empty = ' ',
    first_player = 'O',
    second_player = 'X',
    any = 'A' // For evaluation
};

std::ostream &operator<<(std::ostream &os, Chess ch);

class InvalidPosition : public std::exception
{
};

class ChessBroad
{
  public:
    ChessBroad();
    void emplace(Position pos, Chess ch);
    bool full() const;
    bool win_game(Position hint) const;
    Chess get(Position pos) const;

  private:
    std::array<std::array<Chess, broad_size>, broad_size> _broad;
};

std::ostream &operator<<(std::ostream &os, const ChessBroad &broad);
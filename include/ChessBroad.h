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

enum class Chess
{
    empty,
    first_player,
    second_player,
    any // For evaluation
};

std::ostream &operator<<(std::ostream &os, Chess ch);

class InvalidPosition : public std::exception
{
};

class ChessBroad
{
  public:
    ChessBroad();

    // Place a chess at the specified position,
    // throw if it's not empty.
    void emplace(Position pos, Chess ch);

    // Find out if the broad is full.
    bool full() const;

    // Find out if somebody wins.
    bool win_game(Position hint) const;

    // Get the chess at the specified position.
    Chess get(Position pos) const;

  private:
    std::array<std::array<Chess, broad_size>, broad_size> _broad;
};

std::ostream &operator<<(std::ostream &os, const ChessBroad &broad);
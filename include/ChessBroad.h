#pragma once
#include <array>
#include <exception>
#include <iosfwd>

struct Position
{
    int row;
    int column;

    friend Position operator+(Position left, Position right);
    friend Position operator-(Position left, Position right);
    Position &operator+=(Position other)
    {
        return *this = *this + other;
    }
    Position &operator-=(Position other)
    {
        return *this = *this - other;
    }
    friend Position operator*(int n, Position pos);
    friend Position operator*(Position pos, int n);
    Position &operator*=(int other)
    {
        return *this = *this * other;
    }

    friend bool operator==(Position left, Position right);
    friend bool operator!=(Position left, Position right);
};

enum class Chess
{
    empty,
    first_player,
    second_player,
    any // For evaluation
};

std::ostream &operator<<(std::ostream &os, Chess ch);

class ChessBroad
{
  public:
    static constexpr int size = 15;

    ChessBroad();

    // Place a chess at the specified position.
    void emplace(Position pos, Chess ch);

    // Find out if the broad is full.
    bool full() const;

    // Get the chess at the specified position.
    Chess get(Position pos) const;

    // Check if a given position is in the range of the broad.
    static bool in_range(Position pos);

  private:
    std::array<std::array<Chess, size>, size> _broad;
};

std::ostream &operator<<(std::ostream &os, const ChessBroad &broad);
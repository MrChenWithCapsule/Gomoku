#pragma once
#include <array>
#include <iostream>
#include <iomanip>

constexpr int broad_size = 15;
using BroadType = std::array<std::array<char, broad_size>, broad_size>;
// broad[i][j]: char at row(i), column(j)

struct Position
{
    int row;
    int column;
};
Position operator*(int num, Position pos) { return Position{num * pos.row, num * pos.column}; }
Position operator+(Position left, Position right) { return Position{left.row + right.row, left.column + right.column}; }
Position operator-(Position left, Position right) { return Position{left.row - right.row, left.column - right.column}; }

constexpr char first_player_chess = 'O';
constexpr char second_player_chess = 'X';

#include "ChessBroad.h"
#include <climits>

constexpr int first_win = INT_MAX;
constexpr int second_win = INT_MIN;
int evaluate(const ChessBroad &broad);
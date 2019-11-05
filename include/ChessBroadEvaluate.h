#include "ChessBroad.h"
#include <climits>

/*
 * Bigger score means the first player has more advantage.
 * If the first player wins, the score is INT_MAX; INT_MIN is the second wins.
 */

constexpr int first_win = INT_MAX;
constexpr int second_win = INT_MIN;

// Evaluate the current score.
int evaluate(const ChessBroad &broad);
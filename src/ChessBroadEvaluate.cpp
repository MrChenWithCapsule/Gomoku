#include <array>
#include <numeric>
#include <string>
#include <utility>

#include "ChessBroadEvaluate.h"

using Pattern = std::array<Chess, 5>;

constexpr Pattern operator""_c(const char *str, size_t len)
{
    Pattern ret{Chess::any, Chess::any, Chess::any, Chess::any, Chess::any};
    for (int i = 0; i < 6 && i < len; ++i)
    {
        switch (str[i])
        {
        case ' ':
            ret[i] = Chess::empty;
            break;
        case 'o':
            ret[i] = Chess::first_player;
            break;
        case 'x':
            ret[i] = Chess::second_player;
            break;
        default:
            break;
        }
    }
    return ret;
}

std::pair<Pattern, int> line_score[] = {
    {" o"_c, 12},      {" x"_c, -12},     {"  o"_c, 12},      {"  x"_c, -12},         {"   o"_c, 12},
    {"   x"_c, -12},   {" o x"_c, 12},    {" x o"_c, -12},    {"  ox"_c, 10},         {"  xo"_c, -10},
    {" o  x"_c, 14},   {" x  o"_c, -14},  {"  o x"_c, 12},    {"  x o"_c, -12},       {"   ox"_c, 10},
    {"   xo"_c, -10},  {" o   "_c, 16},   {" x   "_c, -16},   {"  o  "_c, 14},        {"  x  "_c, -14},
    {"   o "_c, 12},   {"   x "_c, -12},  {"    o"_c, 10},    {"    x"_c, -10},       {" o o"_c, 60},
    {" x x"_c, -60},   {" oo "_c, 60},    {" xx "_c, -60},    {" oo"_c, 60},          {" xx"_c, -60},
    {"  oo"_c, 60},    {"  xx"_c, -60},   {" o ox"_c, 60},    {" x xo"_c, -60},       {" oo x"_c, 60},
    {" xx o"_c, -60},  {"  oox"_c, 60},   {"  xxo"_c, -60},   {" o o "_c, 70},        {" x x "_c, -70},
    {" oo  "_c, 70},   {" xx  "_c, -70},  {"  oo "_c, 70},    {"  xx "_c, -70},       {"   oo"_c, 70},
    {"   xx"_c, -70},  {" ooo"_c, 140},   {" xxx"_c, -140},   {" ooox"_c, 140},       {" xxxo"_c, -140},
    {" oo o"_c, 800},  {" xx x"_c, -800}, {" o oo"_c, 800},   {" x xx"_c, -800},      {" ooo "_c, 800},
    {" xxx "_c, -800}, {" oooo"_c, 3500}, {" xxxx"_c, -3500}, {"ooooo"_c, first_win}, {"xxxxx"_c, second_win}};

// Check if matches.
bool is_match(const ChessBroad &broad, Position pos, Position delta, int index)
{
    auto in_range = [](Position pos) {
        return pos.row >= 0 && pos.row < broad_size && pos.column >= 0 && pos.column < broad_size;
    };
    for (int i = 0; i < line_score[index].first.size(); ++i)
    {
        switch (line_score[index].first[i])
        {
        case Chess::any:
            break;
        default:
            if (!in_range(pos + i * delta) || broad.get(pos + i * delta) != line_score[index].first[i])
                return false;
            break;
        }
    }
    return true;
}

// Try to match corresponding chesses with the pattern in the table.
int match(const ChessBroad &broad, Position pos, Position delta, bool next_first)
{
    for (int i = std::size(line_score) - 1; i >= 0; --i)
        if (is_match(broad, pos, delta, i))
        {
            if (next_first)
                return i % 2 ? line_score[i].second + line_score[i].second / 4 : line_score[i].second;
            else
                return !(i % 2) ? line_score[i].second + line_score[i].second / 4 : line_score[i].second;
        }
    return 0;
}

// Evaluate the score of the current point.
int eval_point(const ChessBroad &broad, Position pos, bool next_first)
{
    std::array scores{match(broad, pos, {1, 0}, next_first),  match(broad, pos, {1, 1}, next_first),
                      match(broad, pos, {0, 1}, next_first),  match(broad, pos, {-1, 1}, next_first),
                      match(broad, pos, {-1, 0}, next_first), match(broad, pos, {-1, -1}, next_first),
                      match(broad, pos, {0, -1}, next_first), match(broad, pos, {1, -1}, next_first)};
    for (auto s : scores)
        if (s == first_win || s == second_win)
            return s;
    return std::accumulate(scores.begin(), scores.end(), 0);
}
int evaluate(const ChessBroad &broad, bool next_first)
{
    // TODO: Use the previous result for optimization.
    int score = 0;
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
        {
            int tmp = eval_point(broad, {i, j}, next_first);
            if (tmp == first_win || tmp == second_win)
                return tmp;
            else
                score += tmp;
        }
    return score;
}
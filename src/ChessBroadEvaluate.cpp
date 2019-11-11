#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>

#include "ChessBroadEvaluate.h"
#include "TrieMatcher.h"

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

constexpr std::pair<Pattern, int> line_score[] = {
    {" o"_c, 17},      {" x"_c, -12},     {"  o"_c, 17},      {"  x"_c, -12},         {"   o"_c, 17},
    {"   x"_c, -12},   {" o x"_c, 17},    {" x o"_c, -12},    {"  ox"_c, 15},         {"  xo"_c, -10},
    {" o  x"_c, 19},   {" x  o"_c, -14},  {"  o x"_c, 17},    {"  x o"_c, -12},       {"   ox"_c, 15},
    {"   xo"_c, -10},  {" o   "_c, 21},   {" x   "_c, -16},   {"  o  "_c, 19},        {"  x  "_c, -14},
    {"   o "_c, 17},   {"   x "_c, -12},  {"    o"_c, 15},    {"    x"_c, -10},       {" o o"_c, 65},
    {" x x"_c, -60},   {" oo "_c, 65},    {" xx "_c, -60},    {" oo"_c, 65},          {" xx"_c, -60},
    {"  oo"_c, 65},    {"  xx"_c, -60},   {" o ox"_c, 65},    {" x xo"_c, -60},       {" oo x"_c, 65},
    {" xx o"_c, -60},  {"  oox"_c, 65},   {"  xxo"_c, -60},   {" o o "_c, 75},        {" x x "_c, -70},
    {" oo  "_c, 75},   {" xx  "_c, -70},  {"  oo "_c, 75},    {"  xx "_c, -70},       {"   oo"_c, 75},
    {"   xx"_c, -70},  {" ooo"_c, 150},   {" xxx"_c, -140},   {" ooox"_c, 150},       {" xxxo"_c, -140},
    {" oo o"_c, 1000}, {" xx x"_c, -800}, {" o oo"_c, 1000},  {" x xx"_c, -800},      {" ooo "_c, 1000},
    {" xxx "_c, -800}, {" oooo"_c, 3000}, {" xxxx"_c, -3500}, {"ooooo"_c, first_win}, {"xxxxx"_c, second_win}};

// Check if matches.
bool is_match(const ChessBroad &broad, Position pos, Position delta, int index)
{
    for (int i = 0; i < line_score[index].first.size(); ++i)
    {
        switch (line_score[index].first[i])
        {
        case Chess::any:
            break;
        default:
            if (!ChessBroad::in_range(pos + i * delta) || broad.get(pos + i * delta) != line_score[index].first[i])
                return false;
            break;
        }
    }
    return true;
}

// Try to match corresponding chesses with the pattern in the table.
int match(const ChessBroad &broad, Position pos, Position delta)
{
    static TrieMatcher score_tree;

    // Initialize the trie tree.
    static int _dummy{[] {
        for (const auto &pr : line_score)
            score_tree.insert(pr.first, pr.second);
        return 0;
    }()};

    return score_tree.get_score(broad, pos, delta);
}

// Evaluate the score of the current point.
int eval_point(const ChessBroad &broad, Position pos)
{
    std::array scores{match(broad, pos, {1, 0}),  match(broad, pos, {1, 1}),  match(broad, pos, {0, 1}),
                      match(broad, pos, {-1, 1}), match(broad, pos, {-1, 0}), match(broad, pos, {-1, -1}),
                      match(broad, pos, {0, -1}), match(broad, pos, {1, -1})};
    for (auto s : scores)
        if (s == first_win || s == second_win)
            return s;
    return std::accumulate(scores.begin(), scores.end(), 0);
}
int evaluate(const ChessBroad &broad)
{
    int score = 0;
    for (int i = 0; i < ChessBroad::size; ++i)
        for (int j = 0; j < ChessBroad::size; ++j)
        {
            int tmp = eval_point(broad, {i, j});
            if (tmp == first_win || tmp == second_win)
                return tmp;
            else
                score += tmp;
        }
    return score;
}
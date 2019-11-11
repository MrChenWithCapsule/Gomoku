#include "TrieMatcher.h"

void TrieMatcher::insert(std::array<Chess, 5> pat, int score)
{
    TrieNode *current = _root.get();
    for (int i = 0; i < 5 && pat[i] != Chess::any; ++i)
    {
        auto &p = current->edges[static_cast<int>(pat[i])];
        if (!p)
        {
            p = std::make_unique<TrieNode>();
            p->score = current->score;
        }
        current = p.get();
    }
    current->score = score;
}

int TrieMatcher::get_score(const ChessBroad &broad, Position pos, Position delta)
{
    TrieNode *current = _root.get();
    while (ChessBroad::in_range(pos))
    {
        auto &p = current->edges[static_cast<int>(broad.get(pos))];
        if (!p)
            break;
        current = p.get();
        pos += delta;
    }
    return current->score;
}
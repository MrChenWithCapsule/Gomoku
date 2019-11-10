#pragma once
#include <array>
#include <memory>

#include "ChessBroad.h"

struct TrieNode
{
    int score = 0;
    std::unique_ptr<TrieNode> edges[3];
};
class TrieMatcher
{
  public:
    void insert(std::array<Chess, 5> pat, int score);
    int get_score(const ChessBroad &broad, Position pos, Position delta);

  private:
    std::unique_ptr<TrieNode> _root = std::make_unique<TrieNode>();
};
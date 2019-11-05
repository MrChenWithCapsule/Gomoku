#pragma once
#include <algorithm>
#include <memory>
#include <vector>

#include "ChessBroad.h"

struct Node
{
    int score = 0;
    int searched_depth = not_searched;
    Position pos;
    std::vector<Node *> childs;
    Node *parent = nullptr;

    Node() = default;
    Node(Node *p) : parent{p}
    {
    }

    // searched_depth:
    static constexpr int cut = -1;
    static constexpr int not_searched = 0;
};

class GomokuTree
{

  public:
    using NodeAllocator = std::allocator<Node>;
    using AllocatorTraits = std::allocator_traits<NodeAllocator>;

    GomokuTree();

    // Update the interal status when current player placed a new chess.
    void update(Position pos);

    // Try to find the best decision for the current player.
    Position decide();

  private:
    // Find all possible decisions for a node.
    void find_possible_position(Node *target, int target_depth);

    // Cut the childs of a node.
    void cut_childs(Node *target, Node *except);

    // Search the tree for the best decision.
    void search(Node *target, int target_depth, int depth_limit);
    void pruning_search(Node *target, int target_depth, int depth_limit);

    // Find out if it is the first player's turn.
    bool is_first();

    ChessBroad _current_broad;
    Node *_current;
    NodeAllocator _alloc;

    int _current_depth = 0;
    /*
     * _current_depth % 2 == 0: first player's tern
     * else: second's.
     */
};
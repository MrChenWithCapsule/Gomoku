#pragma once
#include <algorithm>
#include <memory>
#include <vector>

#include "ChessBroad.h"

struct Node
{
    enum NodeStatus
    {
        not_searched,
        cut,
        searched
    };
    int score = 0;
    NodeStatus status = not_searched;
    Position pos;
    std::vector<Node *> childs;
    Node *parent = nullptr;

    Node() = default;
    Node(Node *p) : parent{p}
    {
    }
    Node(Node *p, Position chess_pos) : parent{p}, pos{chess_pos}
    {
    }
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
    void search(int depth_limit);
    void pruning_search(Node *target, int target_depth, int limit_depth);

    // Find out if it is the first player's turn.
    bool is_first(int depth);

    ChessBroad _current_broad;

    // Before the current chess was put.
    Node *_current;
    NodeAllocator _alloc;

    int _current_depth = 0;
    /*
     * _current_depth % 2 == 0: first player's tern
     * else: second's.
     */
};
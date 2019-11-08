#pragma once

#include <vector>

#include "ChessBroad.h"

class Node;

struct Edge
{
    Node *ptr = nullptr;
    Position pos;
};

class Node
{
  public:
    Node() = default;
    Node(Node *parent);

    // Cut the subtree of current node and mark this node as cut.
    void cut_subtree();
    void cut_subtree(Position except_pos);

    // Do a minmax search with depth limit, return the best step.
    Position find_best_step(ChessBroad *broad, int depth_limit);

  private:
    // If the first player will take the next step.
    bool next_first();

    // Evaluate the score of current node statically, update _score.
    void static_evaluate();

    // Find the childs of current node.
    void find_childs();

    // Get next player's chess.
    Chess get_chess();

    enum NodeStatus
    {
        not_searched,
        searched, // Direct childs of this node are added.
        cut,
        leaf
    };

    const int _depth = 0; // The depth of root is 0.
    int _score = 0;
    NodeStatus _stat = not_searched;
    const Node *_parent = nullptr;
    std::vector<Edge> _edges;
};

class GomokuTree
{
  public:
    GomokuTree() : _root{new Node{}}
    {
        _current = _root;
    }
    ~GomokuTree()
    {
        _root->cut_subtree();
        delete _root;
    }

    // Update the internal status when a player takes a step.
    void update(Position pos);

    // Decide the next step for the current player.
    Position decide();

  private:
    ChessBroad _broad;
    Node *const _root = nullptr;
    Node *_current = nullptr;
};
#pragma once

#include <memory>
#include <vector>

#include "ChessBroad.h"

class Node;

struct Edge
{
    std::unique_ptr<Node> ptr = nullptr;
    const Position pos;

    Edge() = default;
    explicit Edge(Position position);
    Edge(std::unique_ptr<Node> p, Position position);
};

class Node
{
  public:
    Node() = default;
    Node(const Node *parent);

    // Cut the subtree of current node and mark this node as cut.
    void cut_subtree();
    void cut_subtree(Position except_pos);

    // Do a minmax search with depth limit, return the best step.
    Position find_best_step(ChessBroad &broad, int depth_limit);

    // Get the child which puts a chess at pos.
    Node *get_child(Position pos);

    // Get the next player's chess.
    Chess get_chess() const;

  private:
    // Minmax search with alpha-beta pruning.
    void search(ChessBroad &broad, int depth_limit);

    // If the first player will take the next step.
    bool next_first() const;

    // Evaluate the score of current node statically, update _score.
    void static_evaluate(ChessBroad &broad);

    // Find the childs of current node.
    void find_childs(ChessBroad &broad);

    enum NodeStatus
    {
        normal,
        cut,
        leaf
    };

    const int _depth = 0; // The depth of root is 0.
    int _score = 0;
    NodeStatus _status = normal;
    const Node *_parent = nullptr;
    std::vector<Edge> _edges;
};

class GomokuTree
{
  public:
    GomokuTree();

    // Update the internal status when a player takes a step.
    void update(Position pos);

    // Decide the next step for the current player.
    Position decide();

  private:
    ChessBroad _broad;
    const std::unique_ptr<Node> _root = std::make_unique<Node>();
    Node *_current = nullptr;
};
#include <algorithm>

#include "ChessBroadEvaluate.h"
#include "GomokuTree.h"

Edge::Edge(Position position) : pos{position}
{
}

Edge::Edge(std::unique_ptr<Node> p, Position position) : ptr{p.release()}, pos{position}
{
}

Node::Node(const Node *parent) : _depth{parent->_depth + 1}, _parent{parent}
{
}

void Node::cut_subtree()
{
    for (auto &e : _edges)
        e.ptr->cut_subtree();
    _edges.clear();
    _stat = cut;
}

void Node::cut_subtree(Position except_pos)
{
    for (auto &e : _edges)
    {
        if (e.pos.row == except_pos.row && e.pos.column == except_pos.column)
            continue;
        e.ptr->cut_subtree();
    }
    _edges.clear();
}

Position Node::find_best_step(ChessBroad &broad, int depth_limit)
{
    _score = next_first() ? second_win : first_win;
    auto better_score = [this](int score1, int score2) {
        return next_first() ? std::max(score1, score2) : std::min(score1, score2);
    };
    if (_edges.empty())
        find_childs(broad);
    for (auto &e : _edges)
    {
        broad.emplace(e.pos, get_chess());
        if (depth_limit == 1)
            e.ptr->static_evaluate(broad);
        else
            e.ptr->search(broad, depth_limit - 1);
        broad.emplace(e.pos, Chess::empty);
    }
    return std::max_element(_edges.begin(), _edges.end(),
                            [&, this](const Edge &e1, const Edge &e2) {
                                return better_score(e1.ptr->_score, e2.ptr->_score) == e2.ptr->_score;
                            })
        ->pos;
}

Node *Node::get_child(Position pos)
{
    if (_edges.empty())
        return _edges.emplace_back(std::make_unique<Node>(this), pos).ptr.get();
    else
        return std::lower_bound(_edges.begin(), _edges.end(), Edge{pos},
                                [](const Edge &e1, const Edge &e2) {
                                    return e1.pos.row < e2.pos.row ||
                                           (e1.pos.row == e2.pos.row && e1.pos.column < e2.pos.column);
                                })
            ->ptr.get();
}

Chess Node::get_chess() const
{
    return next_first() ? Chess::first_player : Chess::second_player;
}

void Node::search(ChessBroad &broad, int depth_limit)
{
    if (_stat == cut)
        return;
    _score = next_first() ? second_win : first_win;
    auto better_score = [this](int score1, int score2) {
        return next_first() ? std::max(score1, score2) : std::min(score1, score2);
    };
    if (_stat != cut && _stat != leaf && _edges.empty())
        find_childs(broad);
    for (auto &e : _edges)
    {
        broad.emplace(e.pos, get_chess());
        if (depth_limit == 1)
            e.ptr->static_evaluate(broad);
        else
            e.ptr->search(broad, depth_limit - 1);
        broad.emplace(e.pos, Chess::empty);
        _score = better_score(_score, e.ptr->_score);
        if (better_score(_score, _parent->_score) == _score)
            return;
    }
}

bool Node::next_first() const
{
    return !(_depth % 2);
}

void Node::static_evaluate(ChessBroad &broad)
{
    _score = evaluate(broad);
    if (_score == first_win || _score == second_win)
        _stat = leaf;
}

void Node::find_childs(ChessBroad &broad)
{
    _stat = normal;
    _edges.reserve(broad_size * broad_size - _depth);
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
            if (broad.get({i, j}) == Chess::empty)
                _edges.emplace_back(std::make_unique<Node>(this), Position{i, j});
}

GomokuTree::GomokuTree()
{
    _current = _root.get();
}

void GomokuTree::update(Position pos)
{
    _broad.emplace(pos, _current->get_chess());
    _current = _current->get_child(pos);
}

Position GomokuTree::decide()
{
    constexpr int depth_limit = 2;
    return _current->find_best_step(_broad, depth_limit);
}
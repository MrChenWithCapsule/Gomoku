#include <array>

#include "ChessBroadEvaluate.h"
#include "GomokuTree.h"

GomokuTree::GomokuTree()
{
    _current = AllocatorTraits::allocate(_alloc, 1);
    AllocatorTraits::construct(_alloc, _current);
}

void GomokuTree::update(Position pos)
{
    if (_current->status == Node::not_searched || _current->status == Node::cut)
        find_possible_position(_current, _current_depth);
    auto comp = [&](Node *const left, Node *const right) { return left->pos < right->pos; };
    static Node temp;
    temp.pos = pos;

    // Find which node to move to.
    Node *current_new = *std::lower_bound(_current->childs.begin(), _current->childs.end(), &temp, comp);

    // Release other nodes and the parent node, move to the new node.
    cut_childs(_current, current_new);
    AllocatorTraits::destroy(_alloc, _current);
    AllocatorTraits::deallocate(_alloc, _current, 1);
    _current = current_new;
    _current_broad.emplace(_current->pos, is_first(_current_depth) ? Chess::first_player : Chess::second_player);
    _current->parent = nullptr;
    ++_current_depth;
}

void GomokuTree::find_possible_position(Node *target, int target_depth)
{
    target->childs.reserve(broad_size * broad_size - target_depth);
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
            if (_current_broad.get({i, j}) == Chess::empty)
            {
                auto ptr = AllocatorTraits::allocate(_alloc, 1);
                AllocatorTraits::construct(_alloc, ptr, target, Position{i, j});
                target->childs.push_back(ptr);
            }
    target->status = Node::searched;
}

void GomokuTree::cut_childs(Node *target, Node *except)
{
    if (target->status == Node::cut || target->status == Node::not_searched)
        return;
    for (Node *ptr : target->childs)
    {
        if (ptr == except)
            continue;

        // Cut recursively.
        cut_childs(ptr, nullptr);

        AllocatorTraits::destroy(_alloc, ptr);
        AllocatorTraits::deallocate(_alloc, ptr, 1);
    }
    target->childs.clear();
    if (except)
    {
        target->childs.push_back(except);
        target->childs.shrink_to_fit();
        target->status = Node::searched;
    }
    else
        target->status = Node::cut;
}

bool GomokuTree::is_first(int depth)
{
    return !(depth % 2);
}

Position GomokuTree::decide()
{
    int limit = 2;
    search(limit);
    auto comp = [this](Node *left, Node *right) {
        return is_first(_current_depth) ? left->score < right->score : left->score > right->score;
    };

    // Find the best decision.
    auto it = std::max_element(_current->childs.begin(), _current->childs.end(), comp);
    return (*it)->pos;
}

// An utility to help update the broad.
class TemporaryChess
{
  private:
    ChessBroad &_broad;
    Position _pos;
    Chess _ch;

  public:
    TemporaryChess(ChessBroad &broad, Position pos, Chess ch) : _broad{broad}, _pos{pos}, _ch{ch}
    {
        _broad.emplace(_pos, _ch);
    }
    ~TemporaryChess()
    {
        _broad.emplace(_pos, Chess::empty);
    }
};

void GomokuTree::search(int depth_limit)
{
    auto worse = [this](int m, int n) { return is_first(_current_depth) ? std::min(m, n) : std::max(m, n); };

    // Find possible decisions if current node is not searched.
    if (_current->status == Node::not_searched)
        find_possible_position(_current, _current_depth);

    _current->score = is_first(_current_depth) ? first_win : second_win;
    for (auto ptr : _current->childs)
    {
        // Update the internal status for evaluation.
        TemporaryChess _{_current_broad, ptr->pos,
                         is_first(_current_depth) ? Chess::first_player : Chess::second_player};

        pruning_search(ptr, _current_depth + 1, _current_depth + depth_limit);

        // Assume that the other player will make the best decision possible.
        _current->score = worse(_current->score, ptr->score);
    }
}

void GomokuTree::pruning_search(Node *target, int target_depth, int limit_depth)
{
    if (target_depth == limit_depth)
        target->score = evaluate(_current_broad);
    else
    {
        auto worse = [this, target_depth](int m, int n) {
            return is_first(target_depth) ? std::min(m, n) : std::max(m, n);
        };

        // Find possible decisions if current node is not searched and isn't a leaf.
        if (target->status == Node::not_searched && target->score != first_win && target->score != second_win)
            find_possible_position(target, target_depth);

        target->score = is_first(target_depth) ? first_win : second_win;
        for (auto ptr : target->childs)
        {
            // Update the internal status for evaluation.
            TemporaryChess _{_current_broad, ptr->pos,
                             is_first(target_depth) ? Chess::first_player : Chess::second_player};

            pruning_search(ptr, target_depth + 1, limit_depth);

            // Assume that the other player will make the best decision possible.
            target->score = worse(target->score, ptr->score);

            // Pruning.
            if (worse(target->score, target->parent->score) == target->parent->score)
            {
                cut_childs(target, nullptr);
                return;
            }
        }
    }
}
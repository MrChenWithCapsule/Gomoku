#include <array>

#include "ChessBroadEvaluate.h"
#include "GomokuTree.h"

GomokuTree::GomokuTree()
{
    _current = AllocatorTraits::allocate(_alloc, 1);
    _current_depth = 0;
    AllocatorTraits::construct(_alloc, _current);
}

void GomokuTree::update(Position pos)
{
    if (_current->searched_depth == Node::not_searched)
        find_possible_position(_current, _current_depth);
    auto comp = [&](Node *const left, Node *const right) { return left->pos < right->pos; };
    static Node temp;
    temp.pos = pos;
    Node *current_new = *std::lower_bound(_current->childs.begin(), _current->childs.end(), &temp, comp);
    cut_childs(_current, current_new);
    if (_current_depth)
        _current_broad.emplace(_current->pos, _current_depth % 2 ? Chess::first_player : Chess::second_player);
    AllocatorTraits::destroy(_alloc, _current);
    AllocatorTraits::deallocate(_alloc, _current, 1);
    _current = current_new;
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
                AllocatorTraits::construct(_alloc, ptr);
                ptr->pos = Position{i, j};
                target->childs.push_back(ptr);
            }
    target->searched_depth = 1;
}

void GomokuTree::cut_childs(Node *target, Node *except)
{
    if (target->searched_depth == Node::cut || target->searched_depth == Node::not_searched)
        return;
    for (Node *ptr : target->childs)
    {
        if (ptr == except)
            continue;
        cut_childs(ptr, nullptr);
        AllocatorTraits::destroy(_alloc, ptr);
        AllocatorTraits::deallocate(_alloc, ptr, 1);
    }
    target->childs.clear();
    if (except)
    {
        target->childs.push_back(except);
        target->childs.shrink_to_fit();
        target->searched_depth = except->searched_depth + 1;
    }
    else
        target->searched_depth = Node::cut;
}

Position GomokuTree::decide()
{
    static constexpr int search_depth_limit = 1;
    search(_current, _current_depth, _current_depth + search_depth_limit);
    auto comp = [d = _current_depth](Node *left, Node *right) {
        return d % 2 ? left->score > right->score : left->score < right->score;
    };
    auto it = std::max_element(_current->childs.begin(), _current->childs.end(), comp);
    return (*it)->pos;
}

void GomokuTree::search(Node *target, int target_depth, int depth_limit)
{
    _current_broad.emplace(target->pos, target_depth % 2 ? Chess::first_player : Chess::second_player);
    if (target_depth == depth_limit)
        target->score = evaluate(_current_broad);
    else
    {
        auto worse = [d = target_depth](int m, int n) { return d % 2 ? std::max(m, n) : std::min(m, n); };
        if (target->searched_depth == Node::not_searched && target->score != first_win && target->score != second_win)
            find_possible_position(target, target_depth);
        target->score = target_depth % 2 ? first_win : second_win;
        for (auto ptr : target->childs)
        {
            search(ptr, target_depth + 1, depth_limit);
            target->score = worse(target->score, ptr->score);
        }
    }
    _current_broad.emplace(target->pos, Chess::empty);
}
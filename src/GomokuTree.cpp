#include <numeric>

#include "GomokuTree.h"

GomokuTree::GomokuTree()
{
    _root = _current = _alloc.allocate(1);
    _current_depth = 1;
    std::allocator_traits<NodeAllocator>::construct(_alloc, _root);
    find_possible_position(_current, _current_depth);
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
    _current = current_new;
}

void GomokuTree::find_possible_position(Node *target, int target_depth)
{
    target->childs.reserve(broad_size * broad_size - target_depth);
    for (int i = 0; i < broad_size; ++i)
        for (int j = 0; j < broad_size; ++j)
            if (_current_broad.get({i, j}) != Chess::empty)
            {
                auto ptr = _alloc.allocate(1);
                std::allocator_traits<NodeAllocator>::construct(_alloc, ptr);
                ptr->searched_depth = target->searched_depth + 1;
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
        std::allocator_traits<NodeAllocator>::destroy(_alloc, ptr);
        _alloc.deallocate(ptr, 1);
    }
    target->childs.clear();
    target->childs.push_back(except);
    target->childs.shrink_to_fit();
    target->searched_depth = except ? except->searched_depth + 1 : Node::cut;
}

constexpr int first_win = std::numeric_limits<int>::max();
constexpr int second_win = std::numeric_limits<int>::min();
int evaluate(const ChessBroad &broad);
// first player: max
// second player: min

Position GomokuTree::decide()
{
    static constexpr int search_depth_limit = 5;
    search(_current, _current_depth, _current_depth + search_depth_limit);
    auto comp = [d = _current_depth](Node *left, Node *right) {
        return d % 2 ? left->score < right->score : left->score > right->score;
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
        auto worse = [d = target_depth](int m, int n) { return d % 2 ? std::min(m, n) : std::max(m, n); };
        if (target->searched_depth == Node::not_searched && target->score != first_win && target->score != second_win)
            find_possible_position(target, target_depth);
        target->score = target_depth % 2 ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
        for (auto ptr : target->childs)
        {
            search(ptr, target_depth + 1, depth_limit);
            target_depth = worse(target->score, ptr->score);
        }
    }
    _current_broad.emplace(target->pos, Chess::empty);
}
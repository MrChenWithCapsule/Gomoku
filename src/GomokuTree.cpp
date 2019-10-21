#include "GomokuTree.h"

GomokuTree::GomokuTree()
{
    _root = _current = _alloc.allocate(1);
    _node_n = 1;
    _current_depth = 1;
    std::allocator_traits<NodeAllocator>::construct(_alloc, _root);
    find_possible_position(_current, _current_depth);
}

void GomokuTree::update(Position pos)
{
    if (_current->searched_depth <= 0)
        find_possible_position(_current, _current_depth);
    auto comp = [&](Node *const left, Node *const right) { return *left < *right; };
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
}

void GomokuTree::cut_childs(Node *target, Node *except)
{
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
}
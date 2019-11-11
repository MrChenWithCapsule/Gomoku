#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(bool is_first) : _is_first{is_first}
{
}

Position ComputerPlayer::get_pos()
{
    auto pos = _tree.decide();
    _tree.update(pos);
    return pos;
}
void ComputerPlayer::update(Position last)
{
    _tree.update(last);
}

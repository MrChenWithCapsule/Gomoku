#pragma once
#include <memory>

#include "ChessBroad.h"
#include "Player.h"

class GomokuGame
{

public:
    void play();
    bool is_valid_position(Position pos);
    bool win_game(Position pos);
    bool is_full();

private:
    BroadType _broad;
    std::unique_ptr<Player> _first_player;
    std::unique_ptr<Player> _second_player;
};
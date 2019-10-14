#pragma once
#include <memory>

#include "ChessBroad.h"
#include "Player.h"

class GomokuGame
{

public:
    void play();

private:
    bool is_valid_position(Position pos);
    bool win_game(Position pos);
    bool is_full();

    BroadType _broad;
    std::unique_ptr<Player> _first_player;
    std::unique_ptr<Player> _second_player;
};
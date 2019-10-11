#include <iostream>
#include <memory>
#include <algorithm>
#include <utility>

#include "ChessBroad.h"
#include "GomokuGame.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "PrintUtil.h"

using std::cin;
using std::cout;
using std::flush;

char image_gamestartup[] =
    R"(
*************************************************
*   ________                       __           *
*  /  _____/  ____   _____   ____ |  | ____ __  *
* /   \  ___ /  _ \ /     \ /  _ \|  |/ /  |  \ *
* \    \_\  (  <_> )  Y Y  (  <_> )    <|  |  / *
*  \______  /\____/|__|_|  /\____/|__|_ \____/  *
*         \/             \/            \/       *
*************************************************
******************    0.EXIT   ******************
******************    1.PVE    ******************
******************    2.PVP    ******************
*************************************************
)";

void GomokuGame::play()
{
    ClearScreen clear;
    PrintBroad print_broad{_broad};

    // Startup
    cout << clear << image_gamestartup << flush;
    for (auto &arr : _broad)
        std::fill(arr.begin(), arr.end(), ' ');

    // Choose mode
    cout << "Select mode: " << flush;
    while (!_first_player)
    {
        int mode;
        cin >> mode;
        switch (mode)
        {
        case 0:
            return;
        case 1:
            _first_player = std::make_unique<HumanPlayer>();
            _second_player = std::make_unique<ComputerPlayer>();
            break;
        case 2:
            _first_player = std::make_unique<HumanPlayer>();
            _second_player = std::make_unique<HumanPlayer>();
            break;

        default:
            cout << "Wrong mode, enter again.\n"
                 << flush;
            break;
        }
    }

    // Play game
    bool is_first_player = true;
    cout << clear << print_broad << flush;
    Position pos;
    while (!is_full())
    {
        auto current_player = is_first_player ? &_first_player : &_second_player;
        auto current_player_name = is_first_player ? "First player" : "Second player";
        auto current_player_chess = is_first_player ? first_player_chess : second_player_chess;

        cout << current_player_name << "'s turn, enter row,column: " << flush;
        pos = (*current_player)->get_position(pos);
        while (!is_valid_position(pos))
        {
            cout << "Not a valid place, enter again: " << flush;
            pos = (*current_player)->get_position(pos);
        }
        _broad[pos.row][pos.column] = current_player_chess;
        cout << clear << print_broad << flush;
        if (win_game(pos))
        {
            cout << current_player_name << " wins.\n"
                 << flush;
            break;
        }
        is_first_player = !is_first_player;
    }
    if (is_full())
        cout << "Broad is full.\n"
             << flush;
    cout << "Press any key to exit.\n"
         << flush;
    cin.get();
    cin.get();
}

bool GomokuGame::is_valid_position(Position pos)
{
    return _broad[pos.row][pos.column] == ' ';
}

bool GomokuGame::is_full()
{
    for (const auto &arr : _broad)
        for (char ch : arr)
            if (ch == ' ')
                return false;
    return true;
}

bool GomokuGame::win_game(Position pos)
{
    auto check = [this](Position delta, Position origin) {
        auto in_range = [](Position pos) { return pos.row >= 0 && pos.row < 15 && pos.column >= 0 && pos.column < 15; };
        char origin_ch = _broad[origin.row][origin.column];
        int count = 0;
        for (Position pos = origin; in_range(pos) && _broad[pos.row][pos.column] == origin_ch; pos = pos + delta)
            ++count;
        for (Position pos = origin - delta; in_range(pos) && _broad[pos.row][pos.column] == origin_ch; pos = pos - delta)
            ++count;
        return count == 5;
    };
    return check({1, 0}, pos) || check({1, 1}, pos) || check({0, 1}, pos) || check({1, -1}, pos);
}

#include <iostream>
#include <memory>

#include "ChessBroad.h"
#include "ClearScreen.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "Player.h"

using std::cin;
using std::cout;
using std::endl;
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
int main(int argc, char const *argv[])
{
    // Startup
    cout << clear << image_gamestartup << flush;

    std::unique_ptr<Player> first_player;
    std::unique_ptr<Player> second_player;
    // Choose mode
    cout << "Select mode: " << flush;
    while (!first_player)
    {
        int mode;
        cin >> mode;
        switch (mode)
        {
        case 0:
            return 0;
        case 1:
            first_player = std::make_unique<HumanPlayer>(true);
            second_player = std::make_unique<ComputerPlayer>(false);
            break;
        case 2:
            first_player = std::make_unique<HumanPlayer>(true);
            second_player = std::make_unique<HumanPlayer>(false);
            break;

        default:
            cout << "Wrong mode, enter again." << endl;
            break;
        }
    }

    // Play game
    ChessBroad broad;
    bool is_first_player = true;
    cout << clear << broad << flush;
    Position pos;
    while (!broad.full())
    {
        auto current_player = is_first_player ? first_player.get() : second_player.get();
        auto current_player_name = is_first_player ? "First player" : "Second player";
        auto current_player_chess = is_first_player ? Chess::first_player : Chess::second_player;

        cout << current_player_name << "'s turn, enter row,column: " << flush;
        while (true)
        {
            auto in_range = [](Position pos) {
                return pos.row >= 0 && pos.row < broad_size && pos.column >= 0 && pos.column < broad_size;
            };
            pos = current_player->get_pos(broad, pos);
            if (!in_range(pos) || broad.get(pos) != Chess::empty)
            {
                cout << "Not a valid place, enter again: " << flush;
                continue;
            }
            broad.emplace(pos, current_player_chess);
            break;
        }
        cout << clear << broad << flush;
        if (broad.win_game(pos))
        {
            cout << current_player_name << " wins." << endl;
            break;
        }
        is_first_player = !is_first_player;
    }
    if (broad.full())
        cout << "Broad is full." << endl;
    cout << "Press any key to exit." << endl;
    cin.get();
    return 0;
}

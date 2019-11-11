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

    std::unique_ptr<Player> players[2];
    // Choose mode
    cout << "Select mode: " << flush;
    while (!players[0])
    {
        int mode;
        cin >> mode;
        switch (mode)
        {
        case 0:
            return 0;
        case 1:
            players[0] = std::make_unique<HumanPlayer>(true);
            players[1] = std::make_unique<ComputerPlayer>(false);
            break;
        case 2:
            players[0] = std::make_unique<HumanPlayer>(true);
            players[1] = std::make_unique<HumanPlayer>(false);
            break;

        default:
            cout << "Wrong mode, enter again." << endl;
            break;
        }
    }

    // Play game
    ChessBroad broad;
    cout << clear << broad << flush;
    int current_player_id = 0;
    while (!broad.full())
    {
        auto current_player_name = (current_player_id == 0) ? "Player 1" : "Player 2";
        auto current_player_chess = (current_player_id == 0) ? Chess::first_player : Chess::second_player;

        cout << current_player_name << "'s turn.\n" << flush;
        Position pos = players[current_player_id]->get_pos();
        broad.emplace(pos, current_player_chess);
        players[1 - current_player_id]->update(pos);
        cout << clear << broad << flush;
        if (broad.win_game(pos))
        {
            cout << current_player_name << " wins." << endl;
            break;
        }
        current_player_id = 1 - current_player_id;
    }
    if (broad.full())
        cout << "Broad is full." << endl;
    cout << "Press any key to exit." << endl;
    cin.get();
    return 0;
}

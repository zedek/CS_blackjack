#include <iostream>
#include <ctime>
#include <Windows.h>
#include <vector>
#include "card.h"
#include <stdio.h>
#include "blackjack.h"

using namespace std;

int main() {
    int y;
    bool loop = true;
    int again;
    //system("cls");
    gotoxy (0, 0);
    cout << "Input number of players:";
    cin >> y;
    system ("cls");
    Game t (y);
    while (true) {
        gotoxy (0, 4);
        cout << ">>";
        //t[0].setcard (1, t[0].get_card (0));
        t.game_hand();
        for (int i = 0; i < y; i++) {
            t.bet_round();
            t.game_hand();
        }
        for (int i = 0; i < y; i++) {
            t.hit_round();
            t.game_hand();
        }
        t.dealer->dealer_move (t.du);
        t.print_score();
        t.calc_winner();
        gotoxy (0, 23);
        cout << "Would you like to play again? F for yes, H for no." << endl;
        bool x = true;
        while (x) {
            if (GetAsyncKeyState (70) & 0x8000) {
                t.new_game();
                t.print_score();
                x = false;
                Sleep (50);
            }
            if (GetAsyncKeyState (72) & 0x8000) {
                gotoxy (0, 23);
                return 0;
            }
        }
    }
    return 0;
}

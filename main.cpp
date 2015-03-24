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
    do {
        //t.player_name(y);
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
        t.calc_winner();
        gotoxy (0, 20);
        cout << "Would you like to play again? 1 for yes 0 for no." << endl;
        cin >> again;
        if (again == 1) {
            system ("cls");
            t.new_game();
            t.print_score();
            again = 0;
        } else loop = false;
    } while (loop);
    gotoxy (0, 23);
    return 0;
}
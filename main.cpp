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
    cout << "Input number of players:";
    cin >> y;
    system ("cls");
    Game t (y);
    gotoxy (0, 4);
    cout << ">>";
    //t[0].setcard (1, t[0].get_card (0));
    t.gameHand();
    int check = 0;
    while (check != y) {
        t.betTurn (check);// x is used to tally up bet/calls/fold
        t.gameHand();    //call/fold is +1 and bet resets it to 0
    }                   //effectivly controls the betting until everyone has called or folded.
    check = 0;         //set it back to 0, starts the hitting off at the player who last raised
    while (check != y) {
        t.hitTurn();
        t.gameHand();
        check++;
    }
    t.calcWinner();
    gotoxy (0, 23);
    return 0;
}
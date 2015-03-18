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
    for (int i = 0; i < y; i++) {
        t.betTurn();
        t.gameHand();
    }
    for (int i = 0; i < y; i++) {
        t.hitTurn();
        t.gameHand();
    }
    t.calcWinner();
    gotoxy (0, 23);
    return 0;
}
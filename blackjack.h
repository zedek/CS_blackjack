#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <string>

#include "card.h"
#include "blackjack.h"

using namespace std;

class Deck {
  public:
    Deck();
    Deck (int x);
    void shuffle();
    Card draw_card();
    bool empty();
    int _top();

  private:
    int ndeck;
    vector<Card> cards;
    int top;
};


class Hand {
  public:
    Hand();
    int getrank (int i);
    void setcard (int i, Card& x);
    void total(); // to control  scoreh
    void hit (Deck& d);
    int size();
    int getscore();
    void fold();
    void showhand();
    int bet (int x);
    int ace;
    bool get_fold();
    bool get_call();

    void is_blackjack();
    void call (int min);
    bool get_blackjack();
    void split_back (Card& card);
    Card& get_card (int i);

    bool mode();

  private:
    vector<Card> cards_in_hand;
    int score;
    bool hascalled;
    int bet_value;
    bool blackjack;
    bool folded;
    bool has_hit = false;
};

class Player : public Hand {
  public:
    Player (Deck& d);
    void showhands();
    string player_name();
    string player_name (int x);
    int get_money();
    void split (Deck& du);
    bool has_split();
    void betting (int& min);
    void showsplit();
    Hand split_hand;
    void split_fold();
    friend ostream& operator<< (ostream& output, Player& h);
  private:
    int money;

    string name;
    bool _split = false;

};

class Dealer: public Hand { // a hybrid between a player class and hand class, it doesn't

  public:
    Dealer (Deck& d);
    Dealer();
    void dealermove (Deck& d);
    friend ostream& operator<< (ostream& output, Dealer* d);
};

class Game {

  public:
    Game (int x);
    void betTurn();
    int winner();
    void nextplayer();
    void print_score();
    void gameHand();

    int minbet();
    void calcWinner();

    void hitTurn();
    int num_players();
    Player& operator[] (int x);

    Deck du;
    vector<Player> players;
    vector<Player*> winners;

    int player = 0;
    int total_bet;

    int min;
    Dealer* dealer;

};

#endif

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <ctime>

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

  private:
    int ndeck;
    vector<Card> cards;
    int top;
};

class Dealer { // a hybrid between a player class and hand class, it doesn't

  public:        // need a money pool like a player would.
    Dealer (Deck& d);
    Dealer();
    void total();
    void hit (Deck& d);
    void dealermove (Deck& d);
    void is_blackjack();
    bool get_blackjack();
    int getscore();
    friend ostream &operator<< (ostream &output, const Dealer& d);

  private:
    vector<Card> hand;  //Dealers hand -- Needs seperate function to calculate total.
    int ace;
    int score;
    bool blackjack;
    bool bust;
};
class Hand {
  public:
    Hand();
    int getrank (int i);
    void setcard (int i, Card& x);
    void total(); // to control  score
    void hit (Deck& d);
    int size();
    int getscore();
    void fold();
    void showhand();
    int bet (int x);
    int ace;
    bool get_fold();
    bool get_call();
    void showsplit();
    void is_blackjack();
    void call (int min);
    bool get_blackjack();
    void split_back (Card& card);
    Card get_card (int i);


  private:
    vector<Card> cards_in_hand;
    int score;
    bool hascalled;
    int bet_value;
    bool blackjack;
    bool folded;
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


  private:
    Hand* split_hand = NULL;
    int money;
    string name;
};

class Game {

  public:
    Game (int x);
    void betTurn();
    int winner();
    void nextplayer();
    void print_score();
    void gameHand();
    bool mode();
    int minbet();
    void calcWinner();
    Player& get_in (int i);
    void hitTurn();
    int num_players();
    Player& operator[] (int x);


    bool betting = true;
    Deck du;


    vector<Player> players;
    vector<Player*> player_in;
    vector<Player*> winners;

    int player = 0;
    int total_bet;
    Card null_card;
    int min;
    Dealer* dealer;

};

#endif
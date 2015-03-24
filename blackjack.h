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
    Card& get_card (int i);
    int   get_rank (int i);
    void  set_card (int i, Card& x);
    void  total(); // to control  scoreh
    void  hit (Deck& d);
    void  bust();
    void  show_hand();
    void  is_blackjack();

    void  split_back (Card& card);
    void  reset (Deck& d);
    int   size();
    int   get_score();
    int   bet (int x);
    bool  get_fold();
    bool  get_call();
    bool  get_blackjack();
    bool  mode();
    int   ace;

  private:
    vector<Card> cards_in_hand;
    int   score;
    int   bet_value;
    bool  blackjack;
    bool  folded;
    bool  has_hit = false;
};

class Player : public Hand {
  public:
    Player (Deck& d);
    void   show_hands();
    void   split (Deck& du);
    void   betting();
    void   show_split();
    void   split_fold();
    void   ten_chip();
    void   fifty_chip();
    void   display_chips (int x);
    int    get_money();
    bool   has_split();
    string player_name();
    string player_name (int x);
    friend ostream& operator<< (ostream& output, Player& h);


    Hand   split_hand;
  private:
    int    money;
    string name;
    bool   _split = false;

};

class Dealer: public Hand { // a hybrid between a player class and hand class, it doesn't

  public:
    Dealer (Deck& d);
    Dealer();
    void dealer_move (Deck& d);
    friend ostream& operator<< (ostream& output, Dealer* d);
};

class Game {

  public:
    Game (int x);
    void bet_round();
    void next_player();
    void print_score();
    void game_hand();
    void calc_winner();
    void hit_round();
    void game_board();
    void new_game();
    int  num_players();
    int  minbet();

    Deck du;
    int  player = 0;
    Dealer* dealer;
    vector<Player> players;
};

#endif

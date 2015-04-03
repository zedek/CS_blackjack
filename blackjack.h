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
	Deck(int x);
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
	int getrank(int i);
	void setcard(int i, Card& x);
	void total(); // to control  scoreh
	void hit(Deck& d);
	int size();
	int getscore();
	void bust();
	void show_hands();
	int bet(int x);
	int ace;
	int get_betValue();
	bool get_fold();
	bool get_call();


	void is_blackjack();
	void call(int min);
	bool get_blackjack();
	void split_back(Card& card);
	Card& get_card(int i);

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
	Player(Deck& d);
	void   showhands();
	string player_name();
	string player_name(int x);
	int    get_money();
	void    set_money(int x);
	void   split(Deck& du);
	bool   has_split();
	void   betting();
	void   showsplit();
	Hand   split_hand;
	void   split_fold();
	friend ostream& operator<< (ostream& output, Player& h);

private:
	int    money;
	string name;
	bool   _split = false;

};

class Dealer : public Hand { // a hybrid between a player class and hand class, it doesn't

public:
	Dealer(Deck& d);
	Dealer();
	void dealermove(Deck& d);
	friend ostream& operator<< (ostream& output, Dealer* d);
};

class Game {

public:
	Game(int x);
	void betTurn();
	void next_player();
	void print_score();
	void game_hand();
	void calcWinner();
	void hitTurn();
	int  num_players();
	int  minbet();
	void game_board();

	Deck du;
	int player = 0;
	Dealer* dealer;
	vector<Player> players;
};

#endif

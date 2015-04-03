#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include "card.h"
#include "blackjack.h"

using namespace std;
Card null_card;

void     clear_line(int x, int y) {
	gotoxy(x, y);
	int to_end = 75 - x;
	for (int i = 0; i < to_end; i++)
		cout << " ";
} //Clears the line starting from x

//       Deck
Deck::Deck() {
	cards.resize(52);
	for (int i = 0; i < 52; i++)	{
		cards[i].set_suit(static_cast<Suit> (i / 13 + HEARTS));
		cards[i].set_rank(static_cast<Rank> (i % 13 + ACE));
	}
	top = 0;
	ndeck = 1;
}
Deck::Deck(int x) {
	cards.resize(52 * x);
	for (int i = 0; i < x * 52; i++)	{
		cards[i].set_suit(static_cast<Suit> (i / x / 13 + HEARTS));
		cards[i].set_rank(static_cast<Rank> (i / x % 13 + ACE));
	}
	top = 0;
	ndeck = x;
}
Card     Deck::draw_card() {
	return cards[top++];
}
void     Deck::shuffle() {
	srand(time(0));
	int f = ndeck * 52;
	for (int i = 0; i < 10000; i++)
		swap(cards[rand() % f], cards[rand() % f]);
	top = 0;
}
bool     Deck::empty() {
	return top >= 52;
}
int      Deck::_top() {
	return top;
}


//       Hand
Hand::Hand() {
	ace = 0;
	score = 0;
	hascalled = false;
	folded = false;
	blackjack = false;
}
Card&    Hand::get_card(int i) {
	return cards_in_hand[i];
}
void     Hand::split_back(Card& card) {
	cards_in_hand.push_back(card);
}
void     Hand::setcard(int i, Card& x) {
	if (cards_in_hand.size() == 0) cards_in_hand.push_back(x);
	else
		cards_in_hand[i] = x;
}
void     Hand::hit(Deck& d) {
	Card& g = d.draw_card();
	if (g.get_rank() == 1)
		ace += 1;
	cards_in_hand.push_back(g);
	if (cards_in_hand.size() > 2)
		has_hit = true;
	total();
}
void     Hand::total() {
	int a = ace;
	score = 0;
	for (size_t i = 0; i <= cards_in_hand.size() - 1; i++) {
		if (getrank(i) == 1) score += 11;
		else if (getrank(i) > 10) score += 10;
		else score += getrank(i);
		is_blackjack();
	}
	if (a != 0) {
		while (score > 21 && a > 0) {
			score -= 10;
			a--;
		}
	}
	if (a == 0 && score > 21) {
		bust();
		return;
	}
}// Allows for multple aces to get reduced as score keeps going over 21.
void     Hand::bust() {
	folded = true;
}
void     Hand::showhand() {
	for (int i = 0; i < size(); i++)
		cards_in_hand[i].display_card(40 + 3 * i, 0, false, true);
}
void     Hand::is_blackjack() {
	if ((cards_in_hand[0].get_rank()  || cards_in_hand[1].get_rank()  > 9) &&
		((cards_in_hand[0].get_rank() || cards_in_hand[1].get_rank() == 1)))
		blackjack = false;
}
void     Hand::call(int min) {
	bet_value = min;
	hascalled = true;
}
int Hand::get_betValue(){
	return bet_value;
}
bool     Hand::get_fold() { return folded; }
bool     Hand::get_blackjack() {
	return blackjack;
}
bool     Hand::mode() {
	return has_hit;
}
int      Hand::getscore() {
	return score;
}
int      Hand::getrank(int i) {
	return cards_in_hand[i].get_rank();
}
int      Hand::size() {
	return cards_in_hand.size();
}
int      Hand::bet(int x) {
        bet_value = x;
	return bet_value;
}



//       Dealer
Dealer::Dealer(Deck& d) {
	hit(d);
	hit(d);
}
Dealer::Dealer() {
}
void     Dealer::dealermove(Deck& d) {
	if (getscore() >= 17);  // if there's an ace above
	else { hit(d); dealermove(d); }
}
ostream& operator<< (ostream& output, Dealer* d) {
	if (d->mode()) {
		for (int i = 0; i < d->size(); i++)
			output << d->get_card(i);
	}
	else
		output << d->get_card(0) << null_card;
	return output;
}

//       Player
Player::Player(Deck& d) {
	hit(d);
	hit(d);
	money = 500;
}
void     Player::showhands() {
	if (split_hand.size() > 0) {
		showhand();
		showsplit();
	}
	else showhand();
}
void     Player::split(Deck& d) {
	if (get_card(0).get_rank() != get_card(1).get_rank()) return;
	Hand sp;
	Card c = get_card(1);
	sp.setcard(0, c);
	sp.hit(d);
	split_hand = sp;
	setcard(1, d.draw_card());
	total();
	_split = true;
}
void     Player::betting() {
	int f = 10;
	bool x = true;
	gotoxy(0, 20);
	cout << f;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	Sleep(500);
	while (x) {
		if (f == 90) clear_line(2, 20);
		if (f == 110) {
			cout << "Max bet is 100";
			f -= 10;
			clear_line(0, 20);
			gotoxy(0, 20);
			cout << f;
		}
		if (GetAsyncKeyState(38) & 0x8000) {
			f += 10;
			gotoxy(0, 20);
			cout << f;
			Sleep(250);
		}
		if (GetAsyncKeyState(40) & 0x8000) {
			if (f > 10) f -= 10;
			gotoxy(0, 20);
			cout << f;
			Sleep(250);
		}
		if (GetAsyncKeyState(71) & 0x8000) {
			money -= f;
			bet(f);
			gotoxy(0, 20);
			cout << f;
			x = false;
			clear_line(0, 20);
			return;
		}
	}
}
void     Player::split_fold() {
	_split = false;
}
void     Player::showsplit() {
	for (int i = 0; i < split_hand.size(); i++)
		split_hand.get_card(i).display_card(60 + 3 * i, 0, false, true);
}
bool     Player::has_split() {
	return _split;
}
int      Player::get_money() {
	return money;
}
void      Player::set_money(int x){
	 money = money + x;
}
string   Player::player_name() {
	return name;
}
string   Player::player_name(int x) {
	if (x < 4) name = 3 + x;
	else if (x == 4) name = '5';
	else name = '6';
	return name;
}
ostream& operator<< (ostream& output, Player& p) {
	output << p.player_name() << " " << p.get_money() << " ";
	if (p.mode()) {
		for (int i = 0; i < p.size(); i++)
			output << p.get_card(i);
		if (p.has_split()) {
			output << "  ";
			for (int i = 0; i < p.split_hand.size(); i++)
				output << p.split_hand.get_card(i);
		}
	}
	else {
		output << p.get_card(0) << null_card;
		if (p.has_split()) {
			output << "  " <<
				p.split_hand.get_card(0) << null_card;
		}
	}
	return output;
}

//       Game
Game::Game(int x) {
	du = Deck(x);
	du.shuffle();
	dealer = new Dealer(du);
	for (int i = 0; i < x; i++) {
		Player nu(du);
		nu.player_name(i);
		players.push_back(nu);
	}
	player = 0;
	print_score();
}
void     Game::nextplayer() {
	if (player == players.size() - 1) {
		gotoxy(0, 4 + players.size() - 1);
		cout << "   ";
		gotoxy(0, 4);
		player = 0;
		cout << ">>";
	}
	else {
		gotoxy(0, 4 + player);
		cout << "   ";
		player++;
		gotoxy(0, 4 + player);
		cout << ">>";
	}
	for (int i = 0; i < 12; i++)
		clear_line(40, i);
}
void     Game::print_score() {
	gotoxy(3, 2);
	cout << "PLAYERS:" << endl
		<< "---------------------------------";
	gotoxy(3, 4);
	for (size_t i = 0; i < players.size(); i++)
		cout << players[i] << " "
		<< endl << "   ";
	gotoxy(3, 4 + players.size());
	cout << "Dealer" << dealer << endl;
}
void     Game::gameHand() {
	clear_line(0, 23);
	if (players[player].get_fold())
		nextplayer();
	gotoxy(0, 23);
	cout << "Press F to pay respects" << endl;
	Sleep(250);
	bool x = true;
	while (x) {
		if (GetAsyncKeyState(70) & 0x8000) {
			players[player].showhands();
			Sleep(250);
			x = false;
		}
	}
}

void     Game::calcWinner() {

	if (dealer->get_blackjack() || dealer->getscore() == 21)	{
		gotoxy(22, 0);
		cout << "dealer wins" << endl;
		return;
	}
	int x = 0;
	int payout;
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i].get_blackjack()) {
			gotoxy(22, 0);
			cout << "Player" << players[i].player_name() << " Has blackjack";
			payout = (players[i].get_betValue()*3.5);
			players[i].set_money(payout);
			x++;
		}
	}
	int y = 0;
	if (x > 0) return;
	else {
		//check for hand size of 5
		for (size_t i = 0; i < players.size(); i++) {
			if (players[i].size() == 5 && players[i].getscore() < 21){
				gotoxy(22, 0);
				cout << "Player " << players[i].player_name() << " is the winner";
				payout = (players[i].get_betValue()*2);
				players[i].set_money(payout);
				y++;
			}
		}
	}	
	int z = 0;
	if (y > 0) return;
	else {
		//check for 21
		for (size_t i = 0; i < players.size(); i++) {
			if (players[i].getscore() == 21) {
				gotoxy(22, 0);
				cout << "Player " << players[i].player_name() << " is the winner";
				payout = (players[i].get_betValue()*2);
				players[i].set_money(payout);
				z++;
			}
		}
	}
	if (z > 0) return;
	else {
		//compare dealer and player scores
		for (size_t i = 0; i < players.size(); i++){
			if ((players[i].getscore() < 21) && (dealer->getscore() < players[i].getscore())) {
				gotoxy(22, 0);
				cout << "Player " << players[i].player_name() << " is the winner";
				payout = (players[i].get_betValue()*2);
				players[i].set_money(payout);
			}
			else
			{
				gotoxy(22, 0);
				cout << "dealer wins" << endl;
			}
		}
	}

}
void     Game::betTurn() {
	Player &p = players[player];
	clear_line(0, 23);
	gotoxy(0, 23);
	cout << "Press S to split, G then UP/Down and G to bet, H to call, J to fold" << endl;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	if (players[player].get_fold()) {
		nextplayer();
		return;
	}
	bool x = true;
	Sleep(100);
	while (x) {
		if (GetAsyncKeyState(83) & 0x8000) {
			p.split(du);
			if (p.has_split())		{
				p.showhands();
				print_score();
				betTurn();
				x = false;
				Sleep(250);
			}
		}
		if (GetAsyncKeyState(71) & 0x8000) {
			p.betting();
			print_score();
			nextplayer();
			Sleep(150);
			x = false;
			return;
		}
		if (GetAsyncKeyState(72) & 0x8000) {
			nextplayer();
			Sleep(150);
			x = false;
			return;
		}
		if (GetAsyncKeyState(74) & 0x8000) {
			if (p.has_split()) {
				int i;
				cout << "Which hand do you want to fold?";
				cin >> i;
				if (i == 1) p.bust();
				else if (i == 2) p.split_fold();
				else cout << "Error";
				betTurn();
				Sleep(150);
				return;
			}
			else {
				p.bust();
				nextplayer();
				Sleep(150);
				return;
			}
		}
	}
}
void     Game::hitTurn() {
	Player &p = players[player];
	p.showhands();
	clear_line(0, 23);
	gotoxy(0, 23);
	cout << "F to hit, H to stand";
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	if (players[player].get_fold()) {
		nextplayer();
		return;
	}
	Sleep(100);
	bool x = true;
	while (x) {
		if (GetAsyncKeyState(70) & 0x8000) {
			p.hit(du);
			p.showhands();
			print_score();
			x = false;
			hitTurn();
		}
		if (GetAsyncKeyState(72) & 0x8000) {
			nextplayer();
			x = false;
			Sleep(100);
			return;
		}
	}
}

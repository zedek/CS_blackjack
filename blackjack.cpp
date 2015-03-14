#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <ctime>
#include "card.h"
#include "blackjack.h"

using namespace std;

//Deck
Deck::Deck() {
    cards.resize (52);
    for (int i = 0; i < 52; i++)	{
        cards[i].set_suit (static_cast<Suit> (i / 13 + HEARTS));
        cards[i].set_rank (static_cast<Rank> (i % 13 + ACE));
    }
    top = 0;
    ndeck = 1;
}
Deck::Deck (int x) {
    cards.resize (52 * x);
    for (int i = 0; i < x * 52; i++)	{
        cards[i].set_suit (static_cast<Suit> (i / x / 13 + HEARTS));
        cards[i].set_rank (static_cast<Rank> (i / x % 13 + ACE));
    }
    top = 0;
    ndeck = x;
}
void Deck::shuffle() {
    srand (time (0));
    int f = ndeck * 52;
    for (int i = 0; i < 10000; i++)
        swap (cards[rand() % f], cards[rand() % f]);
    top = 0;
}
Card Deck::draw_card() {
    return cards[top++];
}
bool Deck::empty() {
    return top >= 52;
}
int  Deck::_top() {
    return top;
}
//Hand
void Hand::split_back (Card& card) {
    cards_in_hand.push_back (card);
}
Hand::Hand() {
    ace = 0;
    score = 0;
    hascalled = false;
    folded = false;
    blackjack = false;
}
Card Hand::get_card (int i) {
    return cards_in_hand[i];
}
void Hand::setcard (int i, Card& x) {
    cards_in_hand[i] = x;
}
int Hand::getscore() {
    return score;
}
void Hand::hit (Deck& d) {
    Card g = d.draw_card();
    if (g.get_rank() == 1) {
        ace += 1;
        gotoxy (15, 20);
    }
    cards_in_hand.push_back (g);
    total();
}
void Hand::total() {
    score = 0;
    int a = ace;
    for (size_t i = 0; i <= cards_in_hand.size() - 1; i++) {
        if (getrank (i) == 1) score += 11;
        else if (getrank (i) > 10) score += 10;
        else score += getrank (i);
        is_blackjack();
    }
    while (score > 21 && a > 0) {
        score -= 10;
        a--;
    }
    if (score > 21)
        fold();
}// Allows for multple aces to get reduced as score keeps going over 21.
int Hand::getrank (int i) {
    return cards_in_hand[i].get_rank();
}
void Hand::fold() {
    folded = true;
}
void Hand::showhand() {
    for (int i = 0; i < size(); i++)
        cards_in_hand[i].display_card (40 + 3 * i, 0, false, true);
}
void Hand::showsplit() {
    for (int i = 0; i < size(); i++)
        cards_in_hand[i].display_card (60 + 3 * i, 0, false, true);
}
void Hand::call (int min) {
    bet_value = min;
    hascalled = true;
}
int Hand::size() {
    return cards_in_hand.size();
}

int Hand::bet (int x) {
    return bet_value = x;
}
bool Hand::get_fold() { return folded; }
bool Hand::get_blackjack() {
    return blackjack;
}
void Hand::is_blackjack() {
    if ((cards_in_hand[0].get_rank() || cards_in_hand[1].get_rank()  > 9) &&
            ((cards_in_hand[0].get_rank() || cards_in_hand[1].get_rank() == 1)))
        blackjack = true;
}
//Dealer
Dealer::Dealer (Deck& d) {
    hit (d);
    hit (d);
    ace = 0;
    score = 0;
    blackjack = false;
    bust = false;
}
Dealer::Dealer() {
}
int Dealer::getscore() {
    return score;
}
void Dealer::total() {
    for (size_t i = 0; i <= hand.size() - 1; i++) {
        if (hand[i].get_rank() == 1) score += 11;
        if (hand[i].get_rank() >= 11) score += 10;
        score += hand[i].get_rank();
        is_blackjack();
    }
}
void Dealer::hit (Deck& d) {
    hand.push_back (d.draw_card());
    total();
}
void Dealer::dealermove (Deck& d) {
    if (score >= 17)  ;  // if there's an ace above
    else { hit (d); dealermove (d); }
}
bool Dealer::get_blackjack() {
    return blackjack;
}
void Dealer::is_blackjack() {
    if ((Dealer::hand[0].get_rank() || Dealer::hand[1].get_rank() > 9) &&
            ((Dealer::hand[0].get_rank() || Dealer::hand[1].get_rank() == 1)))
        blackjack = true;
    else if (score > 21)
        bust = true;
}
//Player
Player::Player (Deck& d) {
    hit (d);
    hit (d);
    money = 500;
    name = "P" + d._top()/2;
}
void Player::showhands() {
    if (split_hand != NULL) {
        showhand();
        showsplit();
    } else showhand();
}
void Player::split (Deck& d) {
    Hand sp;
    sp.setcard (0, get_card (0));
    split_hand = &sp;
}
void Player::betting (int& min) {
    bool x = true;
    int f = min;
    FlushConsoleInputBuffer (GetStdHandle (STD_INPUT_HANDLE));
    while (x) {
        if (-GetAsyncKeyState (38)) {
            f += 10;
            gotoxy (0, 15);
            cout << f;
            Sleep (100);
        }
        if (-GetAsyncKeyState (40)) {
            if (f > 10) f -= 10;
            gotoxy (0, 15);
            cout << f;
            Sleep (100);
        }
        if (-GetAsyncKeyState (72) & 0x8000) {
            money -= f;
            bet (f);
            gotoxy (0, 15);
            cout << f;
            min = f;
            x = false;
        }
    }
}
ostream &operator<< (ostream& output, Hand& h) {
    for (int i = 0; i < h.size(); i++)
        output << h.get_card (i);
    return output;
}
int  Player::get_money() {
    return money;
}
// Game
Game::Game (int x) {
    du = Deck (x);
    dealer = &Dealer (du);
    du.shuffle();
    for (int i = 0; i < x; i++) {
        Player nu (du);
        // nu.player_name (i);
        players.push_back (nu);
    }
    player = 0;
    betting = true;
    min = 0;
    print_score();
}
Player& Game::operator[] (int x) {
    return players[x];
}
void Game::nextplayer() {
    if (player == players.size() - 1) {
        gotoxy (0, 4 + players.size() - 1);
        cout << "   ";
        gotoxy (0, 4);
        player = 0;
        cout << ">>";
    } else {
        gotoxy (0, 4 + player);
        cout << "   ";
        player++;
        gotoxy (0, 4 + player);
        cout << ">>";
    }
    for (int i = 0; i < 2; i++)
        null_card.display_card (40 + 3 * i, 0, false, false);
    /* if ((player > 0) && (players[player - 1].has_split())) {
        for (int i = 0; i < 2; i++)
            null_card.display_card (60 + 3 * i, 0, false, false);
    }*/
}
int  Game::winner() {
    return 0;
}
ostream& operator<< (ostream &output, const Dealer& d) {
    output << &d.hand[0];
    return output;
}
void Game::print_score() {
    gotoxy (3, 2);
    cout << "PLAYERS:" << endl
         << "---------------------------------";
    gotoxy (3, 4);
    for (size_t i = 0; i < players.size(); i++) {
        cout << "P" << i + 1 << " " <<  players[i].get_money() << " ";
        if (betting)
            cout << players[i].get_card (1) << null_card;
        else
            cout << players[i] << " ";
        cout << endl << "   ";
    }
    gotoxy (3, 4 + players.size());
    cout << dealer << endl;
}
bool Game::mode() {
    return betting;
}
int  Game::minbet() {
    return min;
}
void Game::gameHand() {
    char c;
    gotoxy (0, 23);
    cout << "Press F + Enter to see your hand" << endl;
    cin >> c;
    if (c == 'f')
        players[player].showhands();
    else gameHand();
}
void Game::betTurn() {
    Player &p = players[player];
    int c = minbet();
    char x;
    cin >> x;
    switch (x) {
        /*  case ('s') :
              p.split (du);
              if (p.has_split())		{
                  p.showhands();
                  print_score();
                  break;
              }*/
        case ('g') :
            p.bet (c);
            print_score();
            break;
        case ('h') :
            /* p.call();
            }*/
            nextplayer();
            break;
            /*case ('j') :
                if (p.has_split()) {
                    int i;
                    cout << "Which hand do you want to fold?";
                    cin >> i;
                    p.fold();
                    betTurn();
                } else {
                    players[player].fold();
                    nextplayer();
                }
                break;*/
    }
}
void Game::calcWinner() {
    /*  //check for blackjack
    if (dealer->get_blackjack() || dealer->getscore() == 21)	{
        cout << "dealer wins" << endl;
        return;
        //need to add additional component
    }
    int x = 0;
    for (size_t i = 0; i < players.size(); i++) {
        //ask prof adams about overloading bracket twice
        //If a player has a split
        if (players[i].split) {
            for (size_t j = 1; j < players.size(); j++) {
                bool b = players[i][j].get_blackjack();
                if (b) {
                    winners.push_back (&players[i]);
                    x++;
                }
            }
        }
        if (players[i][0].get_blackjack()) {
            winners.push_back (&players[i]);
            x++;
        }
    }
    if (x > 0)
        return;
    else {
        //check for 21
        for (size_t i = 0; i < players.size(); i++) {
            //If a player has a split
            if (players[i].split) {
                for (size_t j = 1; j < players.size(); j++) {
                    if (players[i][j].getscore() == 21)
                        winners.push_back (&players[i]);
                }
            }
            if (players[i][0].getscore() == 21) {
                gotoxy (0, 15);
                cout << "Player " << i + 1 << " is the winner";
                winners.push_back (&players[i]);
                cout << "Player " << i + 1 << " is the winner";
            }
        }
    }
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].split) {
            for (size_t j = 1; j < players.size(); j++) {
                int c = players[i][j].getscore();
                if (dealer->getscore() < c)
                    winners.push_back (&players[i]);
            }
        }
    }*/
}
int  Game::num_players() {
    return players.size();
}
void Game::hitTurn() {
    Player &p = players[player];
    p.showhands();
    char x;
    cin >> x;
    cin.clear();
    gotoxy (0, 24);
    cout << "F to hit, H to stand";
    switch (x) {
        case ('f') :
            p.hit (du);
            p.showhands();
            print_score();
            p.total();
            if (p.get_fold()) {
                nextplayer();
                break;
            } else hitTurn();
            break;
        case ('h') :
            nextplayer();
            break;
    }
}
Player& Game::get_in (int i) {
    return *player_in[i];
}
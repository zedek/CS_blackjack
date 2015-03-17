#ifndef CARD_H
#define CARD_H

#include <iostream>

enum Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ZERO = 24
};
enum Suit { HEARTS = 3, DIAMONDS, CLUBS, SPADES, NUL = 24};

void gotoxy (int x, int y);

const int WIDTH = 12;
const int HEIGHT = 12;

class Card {
  public:
    Card();
    Card (Rank r, Suit s);
    void display_card (int x, int y, bool stacked = false, bool face_up = true);
    //accessor functions
    Rank get_rank() const;
    Suit get_suit() const;
    //mutator functions
    void set_rank (Rank r);
    void set_suit (Suit s);
    //operators
    friend std::ostream& operator<< (std::ostream& out, const Card& card);

  private:
    void display_stamp (int x, int y);
    Rank rank;
    Suit suit;

};
#endif
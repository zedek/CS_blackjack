#include "card.h"
#include <Windows.h>
using namespace std;

void gotoxy (int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
    return;
}
Card::Card() {
    rank = ZERO;
    suit = NUL;
}
Card::Card (Rank r, Suit s) {
    rank = r;
    suit = s;
}
Rank Card::get_rank() const {
    return rank;
}
Suit Card::get_suit() const {
    return suit;
}
void Card::set_rank (Rank r) {
    rank = r;
}
void Card::set_suit (Suit s) {
    suit = s;
}
ostream& operator<< (ostream& out, const Card& card) {
    switch (card.suit) {
        case HEARTS:
        case DIAMONDS:
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                                     FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE |
                                     BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        case CLUBS:
        case SPADES:
        case NUL:
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                                     BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED |
                                     BACKGROUND_INTENSITY);
            break;
    }
    switch (card.rank) {
        case ACE:
            out << "A";
            break;
        case TEN:
            out << "T";
            break;
        case JACK:
            out << "J";
            break;
        case QUEEN:
            out << "Q";
            break;
        case KING:
            out << "K";
            break;
        case ZERO:
            out << "|";
            break;
        default:
            out << card.rank;
            break;
    }
    out << static_cast<char> (card.suit);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    return out;
}
void Card::display_card (int x, int y, bool stacked, bool face_up) {
    const char UPPER_LEFT = 218;
    const char STACKED_UPPER_LEFT = 195;
    const char STACKED_UPPER_RIGHT = 180;
    const char UPPER_RIGHT = 191;
    const char HORIZONTAL = 196;
    const char VERTICAL = 179;
    const char BOTTOM_LEFT = 192;
    const char BOTTOM_RIGHT = 217;
    //top line
    gotoxy (x, y);
    if (stacked)
        cout << STACKED_UPPER_LEFT;
    else
        cout << UPPER_LEFT;
    for (int i = 1; i < WIDTH - 1; i++)
        cout << HORIZONTAL;
    if (stacked)
        cout << STACKED_UPPER_RIGHT;
    else
        cout << UPPER_RIGHT;
    //middle
    if (face_up) {
        for (int i = 1; i < HEIGHT - 1; i++) {
            gotoxy (x, y + i);
            cout << VERTICAL;
            if (i == 1) { //first line
                cout << *this;
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), BACKGROUND_BLUE |
                                         BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
                for (int j = 3; j < WIDTH - 1; j++)
                    cout << " ";
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            } else if (i == HEIGHT - 2) { // last line
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), BACKGROUND_BLUE |
                                         BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
                for (int j = 3; j < WIDTH - 1; j++)
                    cout << " ";
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
                cout << *this;
            } else {
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), BACKGROUND_BLUE |
                                         BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
                for (int j = 1; j < WIDTH - 1; j++)
                    cout << " ";
                SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
            }
            cout << VERTICAL;
        }
    } else {
        for (int i = 1; i < HEIGHT - 1; i++) {
            gotoxy (x, y + i);
            cout << VERTICAL;
            for (int j = 1; j < WIDTH - 1; j++)
                cout << "*";
            cout << VERTICAL;
        }
    }
    //bottom line
    gotoxy (x, y + HEIGHT - 1);
    cout << BOTTOM_LEFT;
    for (int i = 1; i < WIDTH - 1; i++)
        cout << HORIZONTAL;
    cout << BOTTOM_RIGHT;
    display_stamp (x, y);
}
void Card::display_stamp (int x, int y) {
    switch (suit) {
        case HEARTS:
        case DIAMONDS:
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                                     FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE |
                                     BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        case CLUBS:
        case SPADES:
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                                     BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED |
                                     BACKGROUND_INTENSITY);
            break;
    }
    if (rank == 2 || rank == 3) {
        gotoxy (x + WIDTH / 2, y + 2);
        cout << static_cast<char> (suit);
    }
    if (rank > 3 && rank <= 10) {
        gotoxy (x + 3, y + 2);
        cout << static_cast<char> (suit);
        gotoxy (x + WIDTH - 3, y + 2);
        cout << static_cast<char> (suit);
    }
    if (rank == 10 || rank == 8 || rank == 7) {
        gotoxy (x + WIDTH / 2, y + 3);
        cout << static_cast<char> (suit);
    }
    if (rank == 9 || rank == 10 || rank == 7) {
        gotoxy (x + 3, y + 4);
        cout << static_cast<char> (suit);
        gotoxy (x + WIDTH - 3, y + 4);
        cout << static_cast<char> (suit);
    }
    if (rank == 8 || rank == 6) {
        gotoxy (x + 3, y + 5);
        cout << static_cast<char> (suit);
        gotoxy (x + WIDTH - 3, y + 5);
        cout << static_cast<char> (suit);
    }
    if (rank == 5 || rank == 3 || rank == 1 || rank == 9) {
        gotoxy (x + WIDTH / 2, y + 5);
        cout << static_cast<char> (suit);
    }
    //left column
    //right column
    //middle
    //top
    //mid
    //bottom
    //10 and 8 are weird
    if (rank == 9 || rank == 10) {
        gotoxy (x + 3, y + HEIGHT - 5);
        cout << static_cast<char> (suit);
        gotoxy (x + WIDTH - 3, y + HEIGHT - 5);
        cout << static_cast<char> (suit);
    }
    if (rank == 10 || rank == 8) {
        gotoxy (x + WIDTH / 2, y + HEIGHT - 4);
        cout << static_cast<char> (suit);
    }
    if (rank == 2 || rank == 3) {
        gotoxy (x + WIDTH / 2, y + HEIGHT - 3);
        cout << static_cast<char> (suit);
    }
    if (rank > 3 && rank <= 10) {
        gotoxy (x + 3, y + HEIGHT - 3);
        cout << static_cast<char> (suit);
        gotoxy (x + WIDTH - 3, y + HEIGHT - 3);
        cout << static_cast<char> (suit);
    }
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
}
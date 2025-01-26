#include <iostream>
#include <cassert>
#include <array>

typedef char Piece; // pretend type safety

// TODO: enum?
const char EMPTY = 0;
const char PAWN  = 1;
const char KNIGHT = 2;
const char BISHOP = 3;
const char ROOK = 4;
const char QUEEN = 5;
const char KING = 6;

// color of piece determined by sign 
bool is_black(Piece p)
{
    return (p < 0);
}

// TODO: encode move in from-to in 16-bits

// 6 bit value 0..63
typedef char Square;

// mailbox
typedef std::array<Square, 64> Board;

// Board is laid out with a1 = 00, a2 = 01, etc.

// numeric rank
char get_rank(Square s)
{
    char rank = s >> 3;
    assert(rank < 8);
    return rank;
}

// numeric file
char get_file(Square s)
{
    return s & 0x7;
}

std::string get_square_name(Square s)
{
    char r = '1' + get_rank(s);
    char f = 'a' + get_file(s);
    return std::string{f, r};
}

struct Move
{
    Square from;
    Square to;
    bool is_promotion;
    Piece promotion;
};

int main()
{
    // Test cases
    std::cout << get_square_name(000) << "\n";
    std::cout << get_square_name(077) << "\n";
}

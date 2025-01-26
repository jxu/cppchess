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




const std::array<char, 7> PIECE_CHAR = {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}; 

const bool WHITE = 0;
const bool BLACK = 1;

// color of piece determined by sign 
bool is_black(Piece p)
{
    return (p < 0);
}

// TODO: encode move in from-to in 16-bits

// 6 bit value 0..63
typedef char Square;

// 8x8 mailbox
typedef std::array<Square, 64> Board;

// Based on FEN
// Board is laid out with a1 = 00, a2 = 01, etc.
// This is vertically flipped from white's pov
// (Octal)
//    a  b  c  d  e  f  g  h
// 1 00 01 02 03 04 05 06 07 
// 2 10 11 12 13 
// 3 20
// 4 30
// 5 40
// 6 50
// 7 60
// 8 70

struct Position
{
    Board board;
    bool side_to_move;  
    // TODO: castling rights, en passant square, halfmove clock

    Position(std::string fen)
    {
        
    }
};



/* Starting position 
   due to ranks, white is on top and this is not black's view
    a b c d e f g h
  1 R N B Q K B N R
  2 P P P P P P P P
  3 . . . . . . . .
  4 . . . . . . . .
  5 . . . . . . . .
  6 . . . . . . . .
  7 p p p p p p p p
  8 r n b q k b n r
 */


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



// idea for later: unicode chess symbols
std::string print_board(const Board& b)
{
    std::string s;
    for (char r = 7; r >= 0; --r)
    {
        for (char f = 0; f <= 7; ++f)
        {
            char c;
            char ind = 8 * r + f;

            c = PIECE_CHAR[b[ind]];


            s += c;
        }
        s += '\n';
    }


    return s;
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
    //std::cout << get_square_name(000) << "\n";
    //std::cout << get_square_name(077) << "\n";

    Board b;
    b.fill(0);
    std::cout << print_board(b);


}

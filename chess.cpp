#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <cassert>
#include <array>
#include <cctype>

using namespace std;

typedef char Piece; // pretend type safety

// TODO: enum?
// + sign is white, - sign is black
const char EMPTY = 0;
const char PAWN  = 1;
const char KNIGHT = 2;
const char BISHOP = 3;
const char ROOK = 4;
const char QUEEN = 5;
const char KING = 6;

typedef char SquareIndex; // 0..63 


const std::array<char, 7> PIECE_CHAR = {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}; 

const bool WHITE = 0;
const bool BLACK = 1;

// color of piece determined by sign 
bool is_black(Piece p)
{
    return (p < 0);
}



// 8x8 mailbox
typedef std::array<Piece, 64> Board;

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
    bool black_to_move;  
    bool wcastlek; // white castle kingside
    bool wcastleq;
    bool bcastlek;
    bool bcastleq;

    SquareIndex en_passant_target; // can only be rank 3 or 6
    // 0 represents none
    
    // TODO: castling rights, en passant square, halfmove clock


    Position(const std::string& FEN)
    {   
        board.fill(EMPTY);

        std::stringstream buf(FEN);
        char c;
    
        for (int rank = 7; rank >= 0; --rank)
        {
            int file = 0;

            while (file < 8)
            {
                // read character, digit or letter
                cout << "rank file" << rank << file << endl;
                buf >> c;

                cout << "read " << c << endl;

                // digit of empty squares to skip
                if ('1' <= c && c <= '8')
                {
                    file += c - '0';

                }
                else // piece given by letter
                {
                    Piece piece;
                    bool is_black = std::islower(c);
                    c = std::toupper(c);

                    switch (c)
                    {
                        case 'P':
                            piece = PAWN; break;
                        case 'N':
                            piece = KNIGHT; break;
                        case 'B':
                            piece = BISHOP; break;
                        case 'R':
                            piece = ROOK; break;
                        case 'Q':
                            piece = QUEEN; break;
                        case 'K':
                            piece = KING; break;
                            
                        default:
                            throw std::invalid_argument("unrecognized piece");
                    }

                    SquareIndex ind = 8 * rank + file;

                    // write piece to board
                    board[ind] = is_black ? -piece : piece;

                    ++file; // move to next square
                    
                }
                

            }

            if (file > 8)
                throw std::invalid_argument("too many files");

            // expect / separator (if not first rank)

            if (rank > 0)
            {
                buf >> c; 
                if (c != '/')
                    throw std::invalid_argument("expected /");
                
            }

            
        }

        // read remaining info

    }
};

TEST_CASE("read_fen")
{
    //Position empty("8/8/8/8/8/8/8/8 w - - 0 1");

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

    Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Board starting_position = 
    {
        ROOK,   KNIGHT, BISHOP, QUEEN,  KING, BISHOP, KNIGHT, ROOK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        // etc...
    };

    for (SquareIndex i = 0; i < 64; ++i)
    {
        CHECK(p.board[i] == starting_position[i]);
    }
}






// numeric rank
int get_rank(SquareIndex i)
{
    char rank = i >> 3;
    assert(rank < 8);
    return rank;
}


// numeric file
char get_file(SquareIndex i)
{
    return i & 0x7;
}

std::string get_square_name(SquareIndex s)
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

// TODO: encode move in from-to in 16-bits

struct Move
{
    SquareIndex from;
    SquareIndex to;
    bool is_promotion;
    Piece promotion;
};




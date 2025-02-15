#pragma once

#include <iostream>
#include <cassert>
#include <array>
#include <cctype>
#include <sstream>

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

typedef unsigned char uchar;


// color of piece determined by sign 
bool is_black(Piece p)
{
    return (p < 0);
}


/* 0x88 board 
   the other half of the board is garbage, for boundary checking
   the 0-indexed rank and file are indexed as 0b0rrr0fff
   

      a  b  c  d  e  f  g  h
   8 70 71 72 73 74 75 76 77|78 79 7A 7B 7C 7D 7E 7F 
   7 60 61 ...              |
   6 50                     |
   5 40                     |
   4 30                     |
   3 20                     |
   2 10                     |
   1 00                     |

*/


typedef std::array<Piece, 128> Board;

// Coordinate transformations (all 0-indexed)
// rank index 0-7 encodes ranks 1-8
// file index 0-7 encodes files a-h

char sqind(char rank07, char file07)
{
    return 16 * rank07 + file07;
}

char sqrank(char sq)
{
    return sq & 0x7; // 0-indexed
}

char sqfile(char sq)
{
    assert((sq >> 4) <= 7);
    return sq >> 4;
}


// Based on FEN


struct Position
{
    Board board;
    bool black_to_move;  
    bool wcastlek = false; // white castle kingside
    bool wcastleq = false;
    bool bcastlek = false;
    bool bcastleq = false;

    char en_passant_target; // can only be rank 3 or 6
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
                //cout << "rank file" << rank << file << endl;
                buf >> c;

                //cout << "read " << c << endl;

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

                    char ind = sqind(rank, file);

                    // write piece to board
                    board[ind] = is_black ? -piece : piece;

                    ++file; // move to next square
                    
                }
                

            }

            if (file > 8)
                throw std::invalid_argument("too many pieces in this rank");

            // expect / separator (if not first rank)

            if (rank > 0)
            {
                buf >> c; 
                if (c != '/')
                    throw std::invalid_argument("expected /");
                
            }

            
        }

        // read remaining info
        // side to move: w or b
        char side;
        buf >> side;

        if (side == 'w')
            black_to_move = false;
        else if (side == 'b')
            black_to_move = true;
        else
            throw std::invalid_argument("expected side w or b");

        // castling rights (default none)
        // - or letters in KQkq
        std::string castling; 
        buf >> castling;

        // doesn't check for invalid characters
        for (char c : castling)
        {
            if (c == 'K') wcastlek = true;
            if (c == 'Q') wcastleq = true;
            if (c == 'k') bcastlek = true;
            if (c == 'q') bcastleq = true;
        }
        


    }
};



std::string get_square_name(char s)
{
    char r = '1' + sqrank(s);
    char f = 'a' + sqfile(s);
    return std::string{f, r};
}



// idea for later: unicode chess symbols
std::string print_board(const Board& board)
{
    const char PIECE_CHAR[7] = {'.', 'P', 'N', 'B', 'R', 'Q', 'K'}; 
    
    std::string s;
    for (char r = 7; r >= 0; --r)
    {
        for (char f = 0; f <= 7; ++f)
        {
            int p = (int)board[sqind(r,f)];
            char c = (p >= 0) ? PIECE_CHAR[p] : tolower(PIECE_CHAR[-p]);
            s += c;
            s += ' ';
        }
        s += '\n';
    }


    return s;
}

void test_read_fen(void)
{
    //Position empty("8/8/8/8/8/8/8/8 w - - 0 1");

    /* Test starting position 

        a b c d e f g h
      8 r n b q k b n r
      7 p p p p p p p p
      6 . . . . . . . .
      5 . . . . . . . .
      4 . . . . . . . .
      3 . . . . . . . .
      2 P P P P P P P P
      1 R N B Q K B N R
     */

    Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // layout in array order 00 to 7F

    // first rank pieces
    Piece rank0[8] = 
        {ROOK,   KNIGHT, BISHOP, QUEEN,  KING,   BISHOP, KNIGHT, ROOK};

    for (int r = 0; r < 8; ++r)
    {
        for (int f = 0; f < 8; ++f) // only test actual board
        {
            Piece p = pos.board[sqind(r,f)];

            switch (r)
            {
                case 0:
                    assert(p == rank0[f]);
                    break;
                case 1:
                    assert(p == PAWN);
                    break;
                case 6:
                    assert(p == -PAWN);
                    break;
                case 7:
                    assert(p == -rank0[f]);
                    break;
                default:
                    assert(p == EMPTY);
            }
        }
    }

    assert(pos.wcastlek);
    assert(pos.wcastleq);
    assert(pos.bcastlek);
    assert(pos.bcastleq);
}


#pragma once

#include <vector>
#include <algorithm>
#include "board.hpp"


// Move encoding naively for now
// If using 6-bit square index, then in 16 bits:  
// 4-bit flags, 6-bit from, 6-bit to

struct Move
{
    Square from;
    Square to;
    uchar flags;

    Move(Square from, Square to, uchar flags = 0) :
        from(from),
        to(to),
        flags(flags)
    {};

    bool operator==(const Move& other)
    {
        return (from == other.from) && (to == other.to) 
            && (flags == other.flags);
    }

    // TODO: UCI coordinate notation output (and input?), like g7g8q
 
};

// TODO: more efficient move list https://www.chessprogramming.org/Move_List
typedef std::vector<Move> MoveList;

// Generate pseudo-legal rook attacks
// For each direction, iterate over squares until invalid
MoveList generate_rook(Square from, Board board)
{
    MoveList moves;
    return moves;
}

void test_generate_rook()
{
    Board empty_board;
    empty_board.fill(EMPTY);

    MoveList moves_a1 = generate_rook(0x00, empty_board);
    MoveList moves_check = {Move(0x00, 0x01), Move(0x00, 0x02)};

    for (auto move : moves_check)
    {
        assert(std::find(moves_a1.begin(), moves_a1.end(), move) != moves_a1.end());
    }
    
}

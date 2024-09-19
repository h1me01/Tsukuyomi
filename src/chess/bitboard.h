#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

namespace Chess {

    constexpr U64 MASK_FILE[8] = {
            0x101010101010101,
            0x202020202020202,
            0x404040404040404,
            0x808080808080808,
            0x1010101010101010,
            0x2020202020202020,
            0x4040404040404040,
            0x8080808080808080,
    };

    constexpr U64 MASK_RANK[8] = {
            0xff,
            0xff00,
            0xff0000,
            0xff000000,
            0xff00000000,
            0xff0000000000,
            0xff000000000000,
            0xff00000000000000
    };

    constexpr U64 MASK_DIAGONAL[15] = {
            0x80, 0x8040, 0x804020,
            0x80402010, 0x8040201008, 0x804020100804,
            0x80402010080402, 0x8040201008040201, 0x4020100804020100,
            0x2010080402010000, 0x1008040201000000, 0x804020100000000,
            0x402010000000000, 0x201000000000000, 0x100000000000000,
    };

    constexpr U64 MASK_ANTI_DIAGONAL[15] = {
            0x1, 0x102, 0x10204,
            0x1020408, 0x102040810, 0x10204081020,
            0x1020408102040, 0x102040810204080, 0x204081020408000,
            0x408102040800000, 0x810204080000000, 0x1020408000000000,
            0x2040800000000000, 0x4080000000000000, 0x8000000000000000,
    };

    constexpr U64 SQUARE_BB[65] = {
            0x1, 0x2, 0x4, 0x8,
            0x10, 0x20, 0x40, 0x80,
            0x100, 0x200, 0x400, 0x800,
            0x1000, 0x2000, 0x4000, 0x8000,
            0x10000, 0x20000, 0x40000, 0x80000,
            0x100000, 0x200000, 0x400000, 0x800000,
            0x1000000, 0x2000000, 0x4000000, 0x8000000,
            0x10000000, 0x20000000, 0x40000000, 0x80000000,
            0x100000000, 0x200000000, 0x400000000, 0x800000000,
            0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
            0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000,
            0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
            0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000,
            0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
            0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000,
            0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000,
            0x0
    };

    constexpr U64 ROOK_MAGICS[NUM_SQUARES] = {
            0x0080001020400080, 0x0040001000200040, 0x0080081000200080, 0x0080040800100080,
            0x0080020400080080, 0x0080010200040080, 0x0080008001000200, 0x0080002040800100,
            0x0000800020400080, 0x0000400020005000, 0x0000801000200080, 0x0000800800100080,
            0x0000800400080080, 0x0000800200040080, 0x0000800100020080, 0x0000800040800100,
            0x0000208000400080, 0x0000404000201000, 0x0000808010002000, 0x0000808008001000,
            0x0000808004000800, 0x0000808002000400, 0x0000010100020004, 0x0000020000408104,
            0x0000208080004000, 0x0000200040005000, 0x0000100080200080, 0x0000080080100080,
            0x0000040080080080, 0x0000020080040080, 0x0000010080800200, 0x0000800080004100,
            0x0000204000800080, 0x0000200040401000, 0x0000100080802000, 0x0000080080801000,
            0x0000040080800800, 0x0000020080800400, 0x0000020001010004, 0x0000800040800100,
            0x0000204000808000, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
            0x0000040008008080, 0x0000020004008080, 0x0000010002008080, 0x0000004081020004,
            0x0000204000800080, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
            0x0000040008008080, 0x0000020004008080, 0x0000800100020080, 0x0000800041000080,
            0x00FFFCDDFCED714A, 0x007FFCDDFCED714A, 0x003FFFCDFFD88096, 0x0000040810002101,
            0x0001000204080011, 0x0001000204000801, 0x0001000082000401, 0x0001FFFAABFAD1A2
    };

    constexpr U64 BISHOP_MAGICS[NUM_SQUARES] = {
            0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000,
            0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
            0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000,
            0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
            0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000,
            0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
            0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200,
            0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
            0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080,
            0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
            0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800,
            0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
            0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000,
            0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
            0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800,
            0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200
    };

    inline U64 SQUARES_BETWEEN[NUM_SQUARES][NUM_SQUARES];
    inline U64 LINE[NUM_SQUARES][NUM_SQUARES];

    constexpr U64 WHITE_OO_MASK = 0x90;
    constexpr U64 WHITE_OOO_MASK = 0x11;

    constexpr U64 BLACK_OO_MASK = 0x9000000000000000;
    constexpr U64 BLACK_OOO_MASK = 0x1100000000000000;

    constexpr U64 ALL_CASTLING_MASK = 0x9100000000000091;

    constexpr int DEBRUIJN64[64] = {
            0, 47, 1, 56, 48, 27, 2, 60,
            57, 49, 41, 37, 28, 16, 3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11, 4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30, 9, 24,
            13, 18, 8, 12, 7, 6, 5, 63
    };

    // returns index of least significant bit in bitboard
    constexpr Square bsf(const U64 b) {
        return static_cast<Square>(DEBRUIJN64[0x03f79d71b4cb0a89 * (b ^ (b - 1)) >> 58]);
    }

    // returns number of bits in the bitboard
    inline int popCount(U64 b) {
        b = b - ((b >> 1) & 0x5555555555555555);
        b = (b & 0x3333333333333333) + ((b >> 2) & 0x3333333333333333);
        b = (b + (b >> 4)) & 0x0f0f0f0f0f0f0f0f;
        b = (b * 0x0101010101010101) >> 56;
        return static_cast<int>(b);
    }

    // returns number of bits in the bitboard.
    // faster than popCount when the bitboard has few bits
    inline int sparsePopCount(U64 b) {
        int count = 0;
        while (b) {
            count++;
            b &= b - 1;
        }
        return count;
    }

    // returns and removes index of the least significant bit in bitboard
    inline Square popLsb(U64 &b) {
        int lsb = bsf(b);
        b &= b - 1;
        return static_cast<Square>(lsb);
    }

    inline U64 reverse(U64 b) {
        b = (b & 0x5555555555555555) << 1 | b >> 1 & 0x5555555555555555;
        b = (b & 0x3333333333333333) << 2 | b >> 2 & 0x3333333333333333;
        b = (b & 0x0f0f0f0f0f0f0f0f) << 4 | b >> 4 & 0x0f0f0f0f0f0f0f0f;
        b = (b & 0x00ff00ff00ff00ff) << 8 | b >> 8 & 0x00ff00ff00ff00ff;
        return b << 48 | (b & 0xffff0000) << 16 | b >> 16 & 0xffff0000 | b >> 48;
    }

    constexpr U64 shift(Direction d, const U64 b) {
        return d == NORTH ? b << 8
        : d == SOUTH ? b >> 8
        : d == EAST ? (b & ~MASK_FILE[FILE_H]) << 1
        : d == WEST ? (b & ~MASK_FILE[FILE_A]) >> 1
        : d == NORTH_EAST ? (b & ~MASK_FILE[FILE_H]) << 9
        : d == NORTH_WEST ? (b & ~MASK_FILE[FILE_A]) << 7
        : d == SOUTH_EAST ? (b & ~MASK_FILE[FILE_H]) >> 7
        : d == SOUTH_WEST ? (b & ~MASK_FILE[FILE_A]) >> 9
        : d == NORTH_NORTH ? b << 16
        : d == SOUTH_SOUTH ? b >> 16
        : 0;
    }

} // namespace Chess

#endif //BITBOARD_H

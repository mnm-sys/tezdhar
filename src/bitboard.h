/* @file:	tezdhar/src/bitboard.h
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bitboard.h
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains bitboard lookup tables for sliding and non-sliding pieces.
 *
 *
 *			----------------------------------------
 *			Chess Board Square Mapping with Bitboard
 *			----------------------------------------
 *
 *	   Layout 1		       Layout 2			   Layout 3
 *
 * 00 01 02 03 04 05 06 07     63 62 61 60 59 58 57 56     56 57 58 59 60 61 62 63
 * 08 09 10 11 12 13 14 15     55 54 53 52 51 50 49 48     48 49 50 51 52 53 54 55
 * 16 17 18 19 20 21 22 23     47 46 45 44 43 42 41 40     40 41 42 43 44 45 46 47
 * 24 25 26 27 28 29 30 31     39 38 37 36 35 34 33 32     32 33 34 35 36 37 38 39
 * 32 33 34 35 36 37 38 39     31 30 29 28 27 26 25 24     24 25 26 27 28 29 30 31
 * 40 41 42 43 44 45 46 47     23 22 21 20 19 18 17 16     16 17 18 19 20 21 22 23
 * 48 49 50 51 52 53 54 55     15 14 13 12 11 10 09 08     08 09 10 11 12 13 14 15
 * 56 57 58 59 60 61 62 63     07 06 05 04 03 02 01 00     00 01 02 03 04 05 06 07
 *
 *
 * Above three are the most commonly used square-mapping layouts in Chess
 * Programming. The ranks and files can be mapped to either big-endian or
 * little-endian layouts leading to a total of 8 combinations categorized
 * as follows:
 * Layout 1: (BERLEF)	Big-Endian Rank Little-Endian File mapping
 * Layout 2: (LERBEF)	Little-Endian Rank Big-Endian File mapping
 * Layout 3: (LERF)	Little-Endian Rank-File mapping
 * Layout 4: (BERF)	Big-Endian Rank-File mapping
 * Layout 5: (LEFR)	Little-Endian File-Rank mapping
 * Layout 6: (LEFBER)	Little-Endian File Big-Endian Rank mapping
 * Layout 7: (BEFLER)	Big-Endian File Little-Endian Rank mapping
 * Layout 8: (BEFR)	Big-Endian File-Rank mapping
 *
 *
 * Let's consider a piece placed on square 'a1', and compare the bitboard
 * values of the above mentioned layout mappings:
 * Layout 1: 0x01000000_00000000
 * Layout 2: 0x00000000_00000080
 * Layout 3: 0x00000000_00000001
 *
 * The advantage of using Layout 2 from the example above is that it maps
 * the 8x8 chessboard in a visibly exact way. The Rank 1 can be viewed
 * directly by inspecting the Least Significant Byte (LSB) of the Layout 1
 * bitboard. Clearly,
 *
 * Rank 1 = [a1 b1 c1 d1 e1 f1 g1 h1] = LSB = 0x80 = 0b1000_0000 shows that
 * 'a1' is set. This makes debugging easier, by having an intuitive view of
 * the chess board by just inspecting the hexadecimal value of the bitboard.
 * The drawback of little endian file mapping becomes aware if we write
 * bitboards as binary or hexadecimal strings, since we usually write numbers
 * big endian wise.
 *
 * Further lets compute bit-index for square 'c3'
 * sqr = rank * 8 + file
 * Layout 1: 5*8+3= 42 (Rank8= 0, Rank3= 5, Rank1= 7, FileA= 0, FileC= 2, File H= 7)
 * Layout 2: 2*8+5= 21 (Rank8= 7, Rank3= 2, Rank1= 0, FileA= 7, FileC= 5, File H= 0)
 * Layout 3: 2*8+2= 18 (Rank8= 7, Rank3= 2, Rank1= 0, FileA= 0, FileC= 2, File H= 7)
 *
 * Anyway, further we rely on little-endian mapping of files and ranks though,
 * since we retain a kind of "natural" relation - that is a < h and 0 < 7.
 * Rank 1 .. Rank 8 -> 0..7
 * A-File .. H-File -> 0..7
 *
 * This is useful for scenarios like comparison, for example,
 * if (sqr >= d2 && sqr < d5) { ... }, and for looping through board squares,like
 * for (sqr = A1; sqr <= H8; sqr++) { ... }. So, finally we adopt Layout 3 (LERF)
 * i.e. Little Endian Rank-File mapping for this project for the reasons mentioned above.
 *
 *
 * a8 b8 c8 d8 e8 f8 g8 h8			56 57 58 59 60 61 62 63
 * a7 b7 c7 d7 e7 f7 g7 h7			48 49 50 51 52 53 54 55
 * a6 b6 c6 d6 e6 f6 g6 h6			40 41 42 43 44 45 46 47
 * a5 b5 c5 d5 e5 f5 g5 h5			32 33 34 35 36 37 38 39
 * a4 b4 c4 d4 e4 f4 g4 h4	 <<===>>	24 25 26 27 28 29 30 31
 * a3 b3 c3 d3 e3 f3 g3 h3			16 17 18 19 20 21 22 23
 * a2 b2 c2 d2 e2 f2 g2 h2			08 09 10 11 12 13 14 15
 * a1 b1 c1 d1 e1 f1 g1 h1			00 01 02 03 04 05 06 07
 *
 *
 *                                  ^
 *                             Rank |
 *                                  -  Little-endian Rank-File Mapping
 *                                  |  for Bitboard Square Layout
 *                                  -  Sqr bitIndex = rank * 8 + file
 *                                  |
 *                                  -
 *                                  |
 *                                8 -  a8  b8  c8  d8  e8  f8   .  h8
 *                                  |
 *                                7 -  a7   .  c7   .   .  f7   .  h7
 *                                  |
 *                                6 -   .  b6   .   .   .  f6  g6   .
 *                                  |
 *                                5 -   .   .   .  d5  e5   .  g5   .
 *                                  |
 *                                4 -   .   .   .  d4  e4   .   .   .
 *                                  |
 *                                3 -   .   .  c3   .   .  f3   .   .
 *                                  |
 *                                2 -  a2  b2   .   .   .   .  g2  h2
 *                                  |
 *                                1 -  a1  b1   .  d1  e1   .  g1  h1
 *                                  |
 *  <---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|-->
 *                                  |   1   2   3   4   5   6   7   8       File
 *                                  -   a   b   c   d   e   f   g   h
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  v
 *
 */

#ifndef __BITBOARD_H__
#define __BITBOARD_H__	1

#include <stdint.h>	// for uint8_t, uint64_t

/* Bitboard Files */
#define BB_FILE_A	0x101010101010101ULL
#define BB_FILE_B	(BB_FILE_A << 1)
#define BB_FILE_C	(BB_FILE_A << 2)
#define BB_FILE_D	(BB_FILE_A << 3)
#define BB_FILE_E	(BB_FILE_A << 4)
#define BB_FILE_F	(BB_FILE_A << 5)
#define BB_FILE_G	(BB_FILE_A << 6)
#define BB_FILE_H	(BB_FILE_A << 7)

/* Bitboard Ranks */
#define BB_RANK_1	0xffULL
#define BB_RANK_2	(BB_RANK_1 << (8 * 1))
#define BB_RANK_3	(BB_RANK_1 << (8 * 2))
#define BB_RANK_4	(BB_RANK_1 << (8 * 3))
#define BB_RANK_5	(BB_RANK_1 << (8 * 4))
#define BB_RANK_6	(BB_RANK_1 << (8 * 5))
#define BB_RANK_7	(BB_RANK_1 << (8 * 6))
#define BB_RANK_8	(BB_RANK_1 << (8 * 7))

/* Bitboard's White Squares and Black Squares */
#define WHITE_SQRS	0x55aa55aa55aa55aaULL
#define BLACK_SQRS	(~(WHITE_SQRS))
#define ALL_SQRS	(WHITE_SQRS | BLACK_SQRS)
/* TODO: empty sqrs */

/* King side squares and Queen side squares */
#define KS_SQRS		(BB_FILE_E | BB_FILE_F | BB_FILE_G | BB_FILE_H)
#define QS_SQRS		(BB_FILE_A | BB_FILE_B | BB_FILE_C | BB_FILE_D)

/* Central Files and Central Squares */
#define CENTRAL_FILES	(BB_FILE_C | BB_FILE_D | BB_FILE_E | BB_FILE_F)
#define CENTRAL_SQRS	((BB_FILE_D | BB_FILE_E) & (BB_RANK_4 | BB_RANK_5))

/* Useful macros for calculating pawn attacks */
#define NOT_A_FILE	(~(BB_FILE_A))
#define NOT_AB_FILE	(~(BB_FILE_A | BB_FILE_B))
#define NOT_H_FILE	(~(BB_FILE_H))
#define NOT_GH_FILE	(~(BB_FILE_G | BB_FILE_H))

/* Major Diagonals */
#define DIAG_A1_H8	0x8040201008040201ULL
#define DIAG_A8_H1	0x102040810204080ULL

/* Top Half of Bitboard belonging to Black's territory, and
 * Bottom Half of Bitboard belonging to White's territory */
#define TOP_HALF	(BB_RANK_8 | BB_RANK_7 | BB_RANK_6 | BB_RANK_5)
#define BOTTOM_HALF	(BB_RANK_4 | BB_RANK_3 | BB_RANK_2 | BB_RANK_1)

/* Bit Manipulation functions for Bitboard */
#define BIT(sq)			(1ULL << (sq))
#define SQR(r, f)		BIT((r * 8) + f)
#define BITS(bb)		(__builtin_popcountll(bb))
#define SET_BIT(bb, sq)         ((bb) |= BIT(sq))
#define GET_BIT(bb, sq)         ((bb) & BIT(sq))
#define POP_BIT(bb, sq)         ((bb) &= ~BIT(sq))
#define FLIP_BIT(bb, sq)        ((bb) ^= BIT(sq))
#define FLIP_BITS(bb, sq1, sq2)	((bb) ^= BIT(sq1) ^ BIT(sq2))
#define POP_LSB(bb)             ((bb) &= (bb) -1)
#define LSB(bb)			(__builtin_ctzll(bb))
#define MSB(bb)			(63 ^ __builtin_clzll(bb))
#define SUBSET(a, b)		(((a) & (b)) == (a))


/* Shift Bitboard to a particular direction, N:North, E:East, W:West, S:South
 * Bitboard bits are stored in Little-endian rank-file mapping format viz.
 * MSB 63 62 61 60 59 58 57 56 55 54 53 ... 10 09 08 07 06 05 04 03 02 01 00 LSB
 * MSB h8 g8 f8 e8 d8 c8 b8 a8 h7 g7 f7 ... c2 b2 a2 h1 g1 f1 e1 d1 c1 b1 a1 LSB */
#define SHIFT_N(bb)		((bb) >> 8)
#define SHIFT_S(bb)		((bb) << 8)
#define SHIFT_E(bb)		(((bb) << 1) & NOT_A_FILE)
#define SHIFT_W(bb)		(((bb) >> 1) & NOT_H_FILE)

#define SHIFT_NN(bb)		((bb) >> 16)
#define SHIFT_SS(bb)		((bb) << 16)
#define SHIFT_NE(bb)		(((bb) << 9) & NOT_A_FILE)
#define SHIFT_SW(bb)		(((bb) >> 9) & NOT_H_FILE)
#define SHIFT_NW(bb)		(((bb) << 7) & NOT_H_FILE)
#define SHIFT_SE(bb)		(((bb) >> 7) & NOT_A_FILE)

#define SHIFT_NNE(bb)		(((bb) << 17) & NOT_A_FILE)
#define SHIFT_NNW(bb)		(((bb) << 15) & NOT_H_FILE)
#define SHIFT_NEE(bb)		(((bb) << 10) & NOT_AB_FILE)
#define SHIFT_NWW(bb)		(((bb) << 6) & NOT_GH_FILE)

#define SHIFT_SSW(bb)		(((bb) >> 17) & NOT_H_FILE)
#define SHIFT_SSE(bb)		(((bb) >> 15) & NOT_A_FILE)
#define SHIFT_SWW(bb)		(((bb) >> 10) & NOT_GH_FILE)
#define SHIFT_SEE(bb)		(((bb) >> 6) & NOT_AB_FILE)

#define MAX_MAGIC_RETRIES	(0x1 << (28))

/* Sliding pieces lookup table for magic numbers */
struct magic_lut {
	uint64_t mask;		// occupancy mask of piece excluding edges
	uint64_t magic;		// magic number for a particular square
	int obits;		// occupancy mask relevant bits
};


/* Pre-calculated magic numbers: To use these magics define the macro
 * USE_PRE_CALCULATED_MAGIC during compile time else the engine will
 * generate new magic numbers using pseudo random number generator */
#define USE_PRE_CALCULATED_MAGIC	1

static const uint64_t bishop_magic_numbers[64] = {
    0x40040844404084ULL,	0x2004208a004208ULL,	0x10190041080202ULL,	0x108060845042010ULL,
    0x581104180800210ULL,	0x2112080446200010ULL,	0x1080820820060210ULL,	0x3c0808410220200ULL,
    0x4050404440404ULL,		0x21001420088ULL,	0x24d0080801082102ULL,	0x1020a0a020400ULL,
    0x40308200402ULL,		0x4011002100800ULL,	0x401484104104005ULL,	0x801010402020200ULL,
    0x400210c3880100ULL,	0x404022024108200ULL,	0x810018200204102ULL,	0x4002801a02003ULL,
    0x85040820080400ULL,	0x810102c808880400ULL,	0xe900410884800ULL,	0x8002020480840102ULL,
    0x220200865090201ULL,	0x2010100a02021202ULL,	0x152048408022401ULL,	0x20080002081110ULL,
    0x4001001021004000ULL,	0x800040400a011002ULL,	0xe4004081011002ULL,	0x1c004001012080ULL,
    0x8004200962a00220ULL,	0x8422100208500202ULL,	0x2000402200300c08ULL,	0x8646020080080080ULL,
    0x80020a0200100808ULL,	0x2010004880111000ULL,	0x623000a080011400ULL,	0x42008c0340209202ULL,
    0x209188240001000ULL,	0x400408a884001800ULL,	0x110400a6080400ULL,	0x1840060a44020800ULL,
    0x90080104000041ULL,	0x201011000808101ULL,	0x1a2208080504f080ULL,	0x8012020600211212ULL,
    0x500861011240000ULL,	0x180806108200800ULL,	0x4000020e01040044ULL,	0x300000261044000aULL,
    0x802241102020002ULL,	0x20906061210001ULL,	0x5a84841004010310ULL,	0x4010801011c04ULL,
    0xa010109502200ULL,		0x4a02012000ULL,	0x500201010098b028ULL,	0x8040002811040900ULL,
    0x28000010020204ULL,	0x6000020202d0240ULL,	0x8918844842082200ULL,	0x4010011029020020ULL
};

static const uint64_t rook_magic_numbers[64] = {
    0x8a80104000800020ULL,	0x140002000100040ULL,	0x2801880a0017001ULL,	0x100081001000420ULL,
    0x200020010080420ULL,	0x3001c0002010008ULL,	0x8480008002000100ULL,	0x2080088004402900ULL,
    0x800098204000ULL,		0x2024401000200040ULL,	0x100802000801000ULL,	0x120800800801000ULL,
    0x208808088000400ULL,	0x2802200800400ULL,	0x2200800100020080ULL,	0x801000060821100ULL,
    0x80044006422000ULL,	0x100808020004000ULL,	0x12108a0010204200ULL,	0x140848010000802ULL,
    0x481828014002800ULL,	0x8094004002004100ULL,	0x4010040010010802ULL,	0x20008806104ULL,
    0x100400080208000ULL,	0x2040002120081000ULL,	0x21200680100081ULL,	0x20100080080080ULL,
    0x2000a00200410ULL,		0x20080800400ULL, 	0x80088400100102ULL,	0x80004600042881ULL,
    0x4040008040800020ULL,	0x440003000200801ULL,	0x4200011004500ULL,	0x188020010100100ULL,
    0x14800401802800ULL,	0x2080040080800200ULL,	0x124080204001001ULL,	0x200046502000484ULL,
    0x480400080088020ULL,	0x1000422010034000ULL,	0x30200100110040ULL,	0x100021010009ULL,
    0x2002080100110004ULL,	0x202008004008002ULL,	0x20020004010100ULL,	0x2048440040820001ULL,
    0x101002200408200ULL,	0x40802000401080ULL,	0x4008142004410100ULL,	0x2060820c0120200ULL,
    0x1001004080100ULL,		0x20c020080040080ULL,	0x2935610830022400ULL,	0x44440041009200ULL,
    0x280001040802101ULL,	0x2100190040002085ULL,	0x80c0084100102001ULL,	0x4024081001000421ULL,
    0x20030a0244872ULL,		0x12001008414402ULL,	0x2006104900a0804ULL,	0x1004081002402ULL
};


#endif	/* __BITBOARD_H__ */


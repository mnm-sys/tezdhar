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
 * if (sqr >= d2 && sqr < d5) { ... }
 * So, finally we adopt Layout 3 (LERF) i.e. Little Endian Rank-File mapping for
 * this project for the reasons mentioned above.
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

/* Bitboard Files */
#define BB_FILE_A	0x101010101010101ULL
#define BB_FILE_B	(BB_FILE_A >> 1)
#define BB_FILE_C	(BB_FILE_A >> 2)
#define BB_FILE_D	(BB_FILE_A >> 3)
#define BB_FILE_E	(BB_FILE_A >> 4)
#define BB_FILE_F	(BB_FILE_A >> 5)
#define BB_FILE_G	(BB_FILE_A >> 6)
#define BB_FILE_H	(BB_FILE_A >> 7)

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

/* Shift Bitboard to a particular direction:
 * N - North, E - East, W - West, S - South */
#define SHIFT_N(bb)		((bb) >> 8)
#define SHIFT_S(bb)		((bb) << 8)
#define SHIFT_NN(bb)		((bb) >> 16)
#define SHIFT_SS(bb)		((bb) << 16)
#define SHIFT_W(bb)		(((bb) & ~A_FILE) >> 1)
#define SHIFT_E(bb)		(((bb) & ~H_FILE) << 1)
#define SHIFT_NE(bb)		(((bb) & ~H_FILE) >> 7)
#define SHIFT_SW(bb)		(((bb) & ~A_FILE) << 7)
#define SHIFT_NW(bb)		(((bb) & ~A_FILE) >> 9)
#define SHIFT_SE(bb)		(((bb) & ~H_FILE) << 9)

/* Chess board's file/rank based square mapping to bitboard's bit position.
 * Bit 0 starts from H1 square, with A1 square being bit 7, and goes upto
 * A8 square which is bit 63 on the bitboard representation */
enum squares {
	SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1, //  0 ..  7
	SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2, //  8 .. 15
	SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3, // 16 .. 23
	SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4, // 24 .. 31
	SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5, // 32 .. 39
	SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6, // 40 .. 47
	SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7, // 48 .. 55
	SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8, // 56 .. 63
	SQ_NB							// 64
};

#endif	/* __BITBOARD_H__ */


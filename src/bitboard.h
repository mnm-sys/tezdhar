/* @file:	tezdhar/src/bitboard.h
 * @url:	https://github.com/mnm-sys/tezdhar/blob/src/bitboard.h
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	22 Oct. 2022
 * @copyright:	GPLv3
 * @desc:
 *
 * Contains bitboard lookup tables for sliding and non-sliding pieces.
 *
 *
 *			----------------------------------------
 *			Chess Board Square Mapping with Bitboard
 *			----------------------------------------
 *
 *	   Layout 0		       Layout 1			   Layout 2
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
 * Let's consider a piece placed on square 'a1', and compare the bitboard
 * values of the above mentioned layout mappings:
 * Layout 0: 0x01_00_00_00_00_00_00_00
 * Layout 1: 0x00_00_00_00_00_00_00_80
 * Layout 2: 0x00_00_00_00_00_00_00_01
 *
 * The advantage of using Layout 1 from the example above is that it maps
 * the 8x8 chessboard in a visibly exact way. The Rank 1 can be viewed
 * directly by inspecting the Least Significant Byte (LSB) of the Layout 1
 * bitboard. Clearly,
 *
 * Rank 1 = [a1 b1 c1 d1 e1 f1 g1 h1] = LSB = 0x80 = 0b1000_0000 shows that
 * 'a1' is set. This makes debugging easier, by having an intuitive view of
 * the chess board by just inspecting the hexadecimal value of the bitboard.
 *
 * Consider, another example (Ng7xf6) of a black knight at g7 capturing a
 * piece at f6. We set both the bits of squares g7 and f6, and compare the
 * bitboard values of the above mentioned layout mappings:
 * Layout 0: 0x00_00_00_00_00_20_00_40
 * Layout 1: 0x02_00_04_00_00_00_00_00
 * Layout 2: 0x40_00_20_00_00_00_00_00
 *
 * The Rank 8 can be viewed directly by inspecting the Most Significant Byte
 * (MSB) of the Layout 1 bitboard. Clearly,
 * Rank 8 = [a8 b8 c8 d8 e8 f8 g8 h8] = MSB = 0x02 = 0b0000_0010 shows that
 * 'g8' is set.
 *
 * Similarly, Rank 6 is the 6th byte from LSB of the bitboard value:
 * Rank 6 = [a6 b6 c6 d6 e6 f6 g6 h6] = 0x04 = 0b0000_0100 clearly shows
 * that 'f6' is set. Similar visual inference can't be derived easily from
 * other layouts except Layout 1.
 *
 * Although, by convention, most other chess engines and publications use
 * Layout 2, we would be using Layout 1 in this project because of it's above
 * mentiontioned advantages and simplicity.
 */

#ifndef __BITBOARD_H__
#define __BITBOARD_H__	1

#include <inttypes.h>

/* Bitboard Files */
#define BB_FILE_A	0x8080808080808080ULL
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
#define WHITE_SQRS	0xaa55aa55aa55aa55ULL
#define BLACK_SQRS	(~(WHITE_SQRS))
#define ALL_SQRS	(WHITE_SQRS | BLACK_SQRS)

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
#define DIAG_A1_H8	0x0102040810204080ULL
#define DIAG_A8_H1	0x8040201008040201ULL

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
	SQ_H1, SQ_G1, SQ_F1, SQ_E1, SQ_D1, SQ_C1, SQ_B1, SQ_A1,	//  0 ..  7
	SQ_H2, SQ_G2, SQ_F2, SQ_E2, SQ_D2, SQ_C2, SQ_B2, SQ_A2,	//  8 .. 15
	SQ_H3, SQ_G3, SQ_F3, SQ_E3, SQ_D3, SQ_C3, SQ_B3, SQ_A3,	// 16 .. 23
	SQ_H4, SQ_G4, SQ_F4, SQ_E4, SQ_D4, SQ_C4, SQ_B4, SQ_A4,	// 24 .. 31
	SQ_H5, SQ_G5, SQ_F5, SQ_E5, SQ_D5, SQ_C5, SQ_B5, SQ_A5,	// 32 .. 39
	SQ_H6, SQ_G6, SQ_F6, SQ_E6, SQ_D6, SQ_C6, SQ_B6, SQ_A6,	// 40 .. 47
	SQ_H7, SQ_G7, SQ_F7, SQ_E7, SQ_D7, SQ_C7, SQ_B7, SQ_A7,	// 48 .. 55
	SQ_H8, SQ_G8, SQ_F8, SQ_E8, SQ_D8, SQ_C8, SQ_B8, SQ_A8,	// 56 .. 63
	SQ_NONE							// 64
};


/* Rook move's destination square lookup table */
static const uint64_t rook_dest_lut[64] = {
	[SQ_H1] = 0x1010101010101feULL,
	[SQ_G1] = 0x2020202020202fdULL,
	[SQ_F1] = 0x4040404040404fbULL,
	[SQ_E1] = 0x8080808080808f7ULL,
	[SQ_D1] = 0x10101010101010efULL,
	[SQ_C1] = 0x20202020202020dfULL,
	[SQ_B1] = 0x40404040404040bfULL,
	[SQ_A1] = 0x808080808080807fULL,

	[SQ_H2] = 0x10101010101fe01ULL,
	[SQ_G2] = 0x20202020202fd02ULL,
	[SQ_F2] = 0x40404040404fb04ULL,
	[SQ_E2] = 0x80808080808f708ULL,
	[SQ_D2] = 0x101010101010ef10ULL,
	[SQ_C2] = 0x202020202020df20ULL,
	[SQ_B2] = 0x404040404040bf40ULL,
	[SQ_A2] = 0x8080808080807f80ULL,

	[SQ_H3] = 0x101010101fe0101ULL,
	[SQ_G3] = 0x202020202fd0202ULL,
	[SQ_F3] = 0x404040404fb0404ULL,
	[SQ_E3] = 0x808080808f70808ULL,
	[SQ_D3] = 0x1010101010ef1010ULL,
	[SQ_C3] = 0x2020202020df2020ULL,
	[SQ_B3] = 0x4040404040bf4040ULL,
	[SQ_A3] = 0x80808080807f8080ULL,

	[SQ_H4] = 0x1010101fe010101ULL,
	[SQ_G4] = 0x2020202fd020202ULL,
	[SQ_F4] = 0x4040404fb040404ULL,
	[SQ_E4] = 0x8080808f7080808ULL,
	[SQ_D4] = 0x10101010ef101010ULL,
	[SQ_C4] = 0x20202020df202020ULL,
	[SQ_B4] = 0x40404040bf404040ULL,
	[SQ_A4] = 0x808080807f808080ULL,

	[SQ_H5] = 0x10101fe01010101ULL,
	[SQ_G5] = 0x20202fd02020202ULL,
	[SQ_F5] = 0x40404fb04040404ULL,
	[SQ_E5] = 0x80808f708080808ULL,
	[SQ_D5] = 0x101010ef10101010ULL,
	[SQ_C5] = 0x202020df20202020ULL,
	[SQ_B5] = 0x404040bf40404040ULL,
	[SQ_A5] = 0x8080807f80808080ULL,

	[SQ_H6] = 0x101fe0101010101ULL,
	[SQ_G6] = 0x202fd0202020202ULL,
	[SQ_F6] = 0x404fb0404040404ULL,
	[SQ_E6] = 0x808f70808080808ULL,
	[SQ_D6] = 0x1010ef1010101010ULL,
	[SQ_C6] = 0x2020df2020202020ULL,
	[SQ_B6] = 0x4040bf4040404040ULL,
	[SQ_A6] = 0x80807f8080808080ULL,

	[SQ_H7] = 0x1fe010101010101ULL,
	[SQ_G7] = 0x2fd020202020202ULL,
	[SQ_F7] = 0x4fb040404040404ULL,
	[SQ_E7] = 0x8f7080808080808ULL,
	[SQ_D7] = 0x10ef101010101010ULL,
	[SQ_C7] = 0x20df202020202020ULL,
	[SQ_B7] = 0x40bf404040404040ULL,
	[SQ_A7] = 0x807f808080808080ULL,

	[SQ_H8] = 0xfe01010101010101ULL,
	[SQ_G8] = 0xfd02020202020202ULL,
	[SQ_F8] = 0xfb04040404040404ULL,
	[SQ_E8] = 0xf708080808080808ULL,
	[SQ_D8] = 0xef10101010101010ULL,
	[SQ_C8] = 0xdf20202020202020ULL,
	[SQ_B8] = 0xbf40404040404040ULL,
	[SQ_A8] = 0x7f80808080808080ULL
};


/* Knight move's destination square lookup table */
static const uint64_t knight_dest_lut[64] = {
	[SQ_H1] = 0x20400ULL,
	[SQ_G1] = 0x50800ULL,
	[SQ_F1] = 0xa1100ULL,
	[SQ_E1] = 0x142200ULL,
	[SQ_D1] = 0x284400ULL,
	[SQ_C1] = 0x508800ULL,
	[SQ_B1] = 0xa01000ULL,
	[SQ_A1] = 0x402000ULL,

	[SQ_H2] = 0x2040004ULL,
	[SQ_G2] = 0x5080008ULL,
	[SQ_F2] = 0xa110011ULL,
	[SQ_E2] = 0x14220022ULL,
	[SQ_D2] = 0x28440044ULL,
	[SQ_C2] = 0x50880088ULL,
	[SQ_B2] = 0xa0100010ULL,
	[SQ_A2] = 0x40200020ULL,

	[SQ_H3] = 0x204000402ULL,
	[SQ_G3] = 0x508000805ULL,
	[SQ_F3] = 0xa1100110aULL,
	[SQ_E3] = 0x1422002214ULL,
	[SQ_D3] = 0x2844004428ULL,
	[SQ_C3] = 0x5088008850ULL,
	[SQ_B3] = 0xa0100010a0ULL,
	[SQ_A3] = 0x4020002040ULL,

	[SQ_H4] = 0x20400040200ULL,
	[SQ_G4] = 0x50800080500ULL,
	[SQ_F4] = 0xa1100110a00ULL,
	[SQ_E4] = 0x142200221400ULL,
	[SQ_D4] = 0x284400442800ULL,
	[SQ_C4] = 0x508800885000ULL,
	[SQ_B4] = 0xa0100010a000ULL,
	[SQ_A4] = 0x402000204000ULL,

	[SQ_H5] = 0x2040004020000ULL,
	[SQ_G5] = 0x5080008050000ULL,
	[SQ_F5] = 0xa1100110a0000ULL,
	[SQ_E5] = 0x14220022140000ULL,
	[SQ_D5] = 0x28440044280000ULL,
	[SQ_C5] = 0x50880088500000ULL,
	[SQ_B5] = 0xa0100010a00000ULL,
	[SQ_A5] = 0x40200020400000ULL,

	[SQ_H6] = 0x204000402000000ULL,
	[SQ_G6] = 0x508000805000000ULL,
	[SQ_F6] = 0xa1100110a000000ULL,
	[SQ_E6] = 0x1422002214000000ULL,
	[SQ_D6] = 0x2844004428000000ULL,
	[SQ_C6] = 0x5088008850000000ULL,
	[SQ_B6] = 0xa0100010a0000000ULL,
	[SQ_A6] = 0x4020002040000000ULL,

	[SQ_H7] = 0x400040200000000ULL,
	[SQ_G7] = 0x800080500000000ULL,
	[SQ_F7] = 0x1100110a00000000ULL,
	[SQ_E7] = 0x2200221400000000ULL,
	[SQ_D7] = 0x4400442800000000ULL,
	[SQ_C7] = 0x8800885000000000ULL,
	[SQ_B7] = 0x100010a000000000ULL,
	[SQ_A7] = 0x2000204000000000ULL,

	[SQ_H8] = 0x4020000000000ULL,
	[SQ_G8] = 0x8050000000000ULL,
	[SQ_F8] = 0x110a0000000000ULL,
	[SQ_E8] = 0x22140000000000ULL,
	[SQ_D8] = 0x44280000000000ULL,
	[SQ_C8] = 0x88500000000000ULL,
	[SQ_B8] = 0x10a00000000000ULL,
	[SQ_A8] = 0x20400000000000ULL
};


/* Bishop move's destination square lookup table */
static const uint64_t bishop_dest_lut[64] = {
	[SQ_H1] = 0x8040201008040200ULL,
	[SQ_G1] = 0x80402010080500ULL,
	[SQ_F1] = 0x804020110a00ULL,
	[SQ_E1] = 0x8041221400ULL,
	[SQ_D1] = 0x182442800ULL,
	[SQ_C1] = 0x10204885000ULL,
	[SQ_B1] = 0x102040810a000ULL,
	[SQ_A1] = 0x102040810204000ULL,

	[SQ_H2] = 0x4020100804020002ULL,
	[SQ_G2] = 0x8040201008050005ULL,
	[SQ_F2] = 0x804020110a000aULL,
	[SQ_E2] = 0x804122140014ULL,
	[SQ_D2] = 0x18244280028ULL,
	[SQ_C2] = 0x1020488500050ULL,
	[SQ_B2] = 0x102040810a000a0ULL,
	[SQ_A2] = 0x204081020400040ULL,

	[SQ_H3] = 0x2010080402000204ULL,
	[SQ_G3] = 0x4020100805000508ULL,
	[SQ_F3] = 0x804020110a000a11ULL,
	[SQ_E3] = 0x80412214001422ULL,
	[SQ_D3] = 0x1824428002844ULL,
	[SQ_C3] = 0x102048850005088ULL,
	[SQ_B3] = 0x2040810a000a010ULL,
	[SQ_A3] = 0x408102040004020ULL,

	[SQ_H4] = 0x1008040200020408ULL,
	[SQ_G4] = 0x2010080500050810ULL,
	[SQ_F4] = 0x4020110a000a1120ULL,
	[SQ_E4] = 0x8041221400142241ULL,
	[SQ_D4] = 0x182442800284482ULL,
	[SQ_C4] = 0x204885000508804ULL,
	[SQ_B4] = 0x40810a000a01008ULL,
	[SQ_A4] = 0x810204000402010ULL,

	[SQ_H5] = 0x804020002040810ULL,
	[SQ_G5] = 0x1008050005081020ULL,
	[SQ_F5] = 0x20110a000a112040ULL,
	[SQ_E5] = 0x4122140014224180ULL,
	[SQ_D5] = 0x8244280028448201ULL,
	[SQ_C5] = 0x488500050880402ULL,
	[SQ_B5] = 0x810a000a0100804ULL,
	[SQ_A5] = 0x1020400040201008ULL,

	[SQ_H6] = 0x402000204081020ULL,
	[SQ_G6] = 0x805000508102040ULL,
	[SQ_F6] = 0x110a000a11204080ULL,
	[SQ_E6] = 0x2214001422418000ULL,
	[SQ_D6] = 0x4428002844820100ULL,
	[SQ_C6] = 0x8850005088040201ULL,
	[SQ_B6] = 0x10a000a010080402ULL,
	[SQ_A6] = 0x2040004020100804ULL,

	[SQ_H7] = 0x200020408102040ULL,
	[SQ_G7] = 0x500050810204080ULL,
	[SQ_F7] = 0xa000a1120408000ULL,
	[SQ_E7] = 0x1400142241800000ULL,
	[SQ_D7] = 0x2800284482010000ULL,
	[SQ_C7] = 0x5000508804020100ULL,
	[SQ_B7] = 0xa000a01008040201ULL,
	[SQ_A7] = 0x4000402010080402ULL,

	[SQ_H8] = 0x2040810204080ULL,
	[SQ_G8] = 0x5081020408000ULL,
	[SQ_F8] = 0xa112040800000ULL,
	[SQ_E8] = 0x14224180000000ULL,
	[SQ_D8] = 0x28448201000000ULL,
	[SQ_C8] = 0x50880402010000ULL,
	[SQ_B8] = 0xa0100804020100ULL,
	[SQ_A8] = 0x40201008040201ULL
};


/* Queen move's destination square lookup table */
static const uint64_t queen_dest_lut[64] = {
	[SQ_H1] = 0x81412111090503feULL,
	[SQ_G1] = 0x2824222120a07fdULL,
	[SQ_F1] = 0x404844424150efbULL,
	[SQ_E1] = 0x8080888492a1cf7ULL,
	[SQ_D1] = 0x10101011925438efULL,
	[SQ_C1] = 0x2020212224a870dfULL,
	[SQ_B1] = 0x404142444850e0bfULL,
	[SQ_A1] = 0x8182848890a0c07fULL,

	[SQ_H2] = 0x412111090503fe03ULL,
	[SQ_G2] = 0x824222120a07fd07ULL,
	[SQ_F2] = 0x4844424150efb0eULL,
	[SQ_E2] = 0x80888492a1cf71cULL,
	[SQ_D2] = 0x101011925438ef38ULL,
	[SQ_C2] = 0x20212224a870df70ULL,
	[SQ_B2] = 0x4142444850e0bfe0ULL,
	[SQ_A2] = 0x82848890a0c07fc0ULL,

	[SQ_H3] = 0x2111090503fe0305ULL,
	[SQ_G3] = 0x4222120a07fd070aULL,
	[SQ_F3] = 0x844424150efb0e15ULL,
	[SQ_E3] = 0x888492a1cf71c2aULL,
	[SQ_D3] = 0x1011925438ef3854ULL,
	[SQ_C3] = 0x212224a870df70a8ULL,
	[SQ_B3] = 0x42444850e0bfe050ULL,
	[SQ_A3] = 0x848890a0c07fc0a0ULL,

	[SQ_H4] = 0x11090503fe030509ULL,
	[SQ_G4] = 0x22120a07fd070a12ULL,
	[SQ_F4] = 0x4424150efb0e1524ULL,
	[SQ_E4] = 0x88492a1cf71c2a49ULL,
	[SQ_D4] = 0x11925438ef385492ULL,
	[SQ_C4] = 0x2224a870df70a824ULL,
	[SQ_B4] = 0x444850e0bfe05048ULL,
	[SQ_A4] = 0x8890a0c07fc0a090ULL,

	[SQ_H5] = 0x90503fe03050911ULL,
	[SQ_G5] = 0x120a07fd070a1222ULL,
	[SQ_F5] = 0x24150efb0e152444ULL,
	[SQ_E5] = 0x492a1cf71c2a4988ULL,
	[SQ_D5] = 0x925438ef38549211ULL,
	[SQ_C5] = 0x24a870df70a82422ULL,
	[SQ_B5] = 0x4850e0bfe0504844ULL,
	[SQ_A5] = 0x90a0c07fc0a09088ULL,

	[SQ_H6] = 0x503fe0305091121ULL,
	[SQ_G6] = 0xa07fd070a122242ULL,
	[SQ_F6] = 0x150efb0e15244484ULL,
	[SQ_E6] = 0x2a1cf71c2a498808ULL,
	[SQ_D6] = 0x5438ef3854921110ULL,
	[SQ_C6] = 0xa870df70a8242221ULL,
	[SQ_B6] = 0x50e0bfe050484442ULL,
	[SQ_A6] = 0xa0c07fc0a0908884ULL,

	[SQ_H7] = 0x3fe030509112141ULL,
	[SQ_G7] = 0x7fd070a12224282ULL,
	[SQ_F7] = 0xefb0e1524448404ULL,
	[SQ_E7] = 0x1cf71c2a49880808ULL,
	[SQ_D7] = 0x38ef385492111010ULL,
	[SQ_C7] = 0x70df70a824222120ULL,
	[SQ_B7] = 0xe0bfe05048444241ULL,
	[SQ_A7] = 0xc07fc0a090888482ULL,

	[SQ_H8] = 0xfe03050911214181ULL,
	[SQ_G8] = 0xfd070a1222428202ULL,
	[SQ_F8] = 0xfb0e152444840404ULL,
	[SQ_E8] = 0xf71c2a4988080808ULL,
	[SQ_D8] = 0xef38549211101010ULL,
	[SQ_C8] = 0xdf70a82422212020ULL,
	[SQ_B8] = 0xbfe0504844424140ULL,
	[SQ_A8] = 0x7fc0a09088848281ULL
};


/* King move's destination square lookup table. This table doesn't
 * contains king's castling moves, which should be checked seperately */
static const uint64_t king_dest_lut[64] = {
	[SQ_H1] = 0x302ULL,
	[SQ_G1] = 0x705ULL,
	[SQ_F1] = 0xe0aULL,
	[SQ_E1] = 0x1c14ULL,
	[SQ_D1] = 0x3828ULL,
	[SQ_C1] = 0x7050ULL,
	[SQ_B1] = 0xe0a0ULL,
	[SQ_A1] = 0xc040ULL,

	[SQ_H2] = 0x30203ULL,
	[SQ_G2] = 0x70507ULL,
	[SQ_F2] = 0xe0a0eULL,
	[SQ_E2] = 0x1c141cULL,
	[SQ_D2] = 0x382838ULL,
	[SQ_C2] = 0x705070ULL,
	[SQ_B2] = 0xe0a0e0ULL,
	[SQ_A2] = 0xc040c0ULL,

	[SQ_H3] = 0x3020300ULL,
	[SQ_G3] = 0x7050700ULL,
	[SQ_F3] = 0xe0a0e00ULL,
	[SQ_E3] = 0x1c141c00ULL,
	[SQ_D3] = 0x38283800ULL,
	[SQ_C3] = 0x70507000ULL,
	[SQ_B3] = 0xe0a0e000ULL,
	[SQ_A3] = 0xc040c000ULL,

	[SQ_H4] = 0x302030000ULL,
	[SQ_G4] = 0x705070000ULL,
	[SQ_F4] = 0xe0a0e0000ULL,
	[SQ_E4] = 0x1c141c0000ULL,
	[SQ_D4] = 0x3828380000ULL,
	[SQ_C4] = 0x7050700000ULL,
	[SQ_B4] = 0xe0a0e00000ULL,
	[SQ_A4] = 0xc040c00000ULL,

	[SQ_H5] = 0x30203000000ULL,
	[SQ_G5] = 0x70507000000ULL,
	[SQ_F5] = 0xe0a0e000000ULL,
	[SQ_E5] = 0x1c141c000000ULL,
	[SQ_D5] = 0x382838000000ULL,
	[SQ_C5] = 0x705070000000ULL,
	[SQ_B5] = 0xe0a0e0000000ULL,
	[SQ_A5] = 0xc040c0000000ULL,

	[SQ_H6] = 0x3020300000000ULL,
	[SQ_G6] = 0x7050700000000ULL,
	[SQ_F6] = 0xe0a0e00000000ULL,
	[SQ_E6] = 0x1c141c00000000ULL,
	[SQ_D6] = 0x38283800000000ULL,
	[SQ_C6] = 0x70507000000000ULL,
	[SQ_B6] = 0xe0a0e000000000ULL,
	[SQ_A6] = 0xc040c000000000ULL,

	[SQ_H7] = 0x302030000000000ULL,
	[SQ_G7] = 0x705070000000000ULL,
	[SQ_F7] = 0xe0a0e0000000000ULL,
	[SQ_E7] = 0x1c141c0000000000ULL,
	[SQ_D7] = 0x3828380000000000ULL,
	[SQ_C7] = 0x7050700000000000ULL,
	[SQ_B7] = 0xe0a0e00000000000ULL,
	[SQ_A7] = 0xc040c00000000000ULL,

	[SQ_H8] = 0x203000000000000ULL,
	[SQ_G8] = 0x507000000000000ULL,
	[SQ_F8] = 0xa0e000000000000ULL,
	[SQ_E8] = 0x141c000000000000ULL,
	[SQ_D8] = 0x2838000000000000ULL,
	[SQ_C8] = 0x5070000000000000ULL,
	[SQ_B8] = 0xa0e0000000000000ULL,
	[SQ_A8] = 0x40c0000000000000ULL
};


#endif	/* __BITBOARD_H__ */

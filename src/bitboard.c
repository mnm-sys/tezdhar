/* @file:	tezdhar/src/bitboard.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/src/bitboard.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @copyright:	GPLv3
 * @desc:	Contains bit manipulation functions for bitboard
 *
 *           ----------------------------------------
 *           Chess board Square mapping to Bit Layout
 *           ----------------------------------------
 *
 * a8 b8 c8 d8 e8 f8 g8 h8              63 62 61 60 59 58 57 56
 * a7 b7 c7 d7 e7 f7 g7 h7              55 54 53 52 51 50 49 48
 * a6 b6 c6 d6 e6 f6 g6 h6              47 46 45 44 43 42 41 40
 * a5 b5 c5 d5 e5 f5 g5 h5              39 38 37 36 35 34 33 32
 * a4 b4 c4 d4 e4 f4 g4 h4    <<==>>    31 30 29 28 27 26 25 24
 * a3 b3 c3 d3 e3 f3 g3 h3              23 22 21 20 19 18 17 16
 * a2 b2 c2 d2 e2 f2 g2 h2              15 14 13 12 11 10 09 08
 * a1 b1 c1 d1 e1 f1 g1 h1              07 06 05 04 03 02 01 00
 *
 */

#include <inttypes.h>	// for uint64_t
#include <stdbool.h>	// for bool
#include <stdio.h>	// for printf
#include "bitboard.h"
#include "chess.h"

/* set bit value from chess board's square index */
uint64_t set_bb_sqr(uint64_t * const bb, const uint8_t index)
{
	dbg_print("\nbb = %016"PRIx64"\tindex = %d\n", *bb, index);
	*bb |= (1ULL << (63 - index));
	dbg_print("After set bit: bb = %016"PRIx64"\n", *bb);
}


/* get bit value at given location */
static bool get_bit(const uint64_t * const bb, const uint8_t index)
{
	return (*bb & (1ULL << (index)));
}

/* clear bit at given index */
static void clear_bb_sqr(uint64_t * const bb, const uint8_t index)
{
	get_bit(bb, index) ? *bb ^= (1ULL << (63 - index)) : 0;
}

/* get bitboard containing all white pieces only */
static uint64_t get_white_pieces(const struct bitboards * const bb)
{
	return (bb->wKing | bb->wQueen | bb->wBishop |
			bb->wKnight | bb->wRook | bb->wPawn);
}

/* get bitboard containing all black pieces only */
static uint64_t get_black_pieces(const struct bitboards * const bb)
{
	return (bb->bKing | bb->bQueen | bb->bBishop |
			bb->bKnight | bb->bRook | bb->bPawn);
}

/* get bitboard containing all black pieces present on chessboard */
static uint64_t get_all_pieces(const struct bitboards * const bb)
{
	return (get_white_pieces(bb) | get_black_pieces(bb));
}

/* print bitboard squares of a particular rank */
static void print_rank_bits(const uint64_t * const bb, const uint8_t index)
{
	for (uint8_t i=0; i<8; i++) {
		get_bit(bb, (index - i)) ? printf("1 ") : printf(". ");
	}
}

/* print 3 distinct bitboards simultaneously, adjacent to each other.
 * We print max 3 bitboards only to honor the 80 coloum terminal width */
static void print_3_bitboards(const uint64_t * const bb1,
		const uint64_t * const bb2, const uint64_t * const bb3)
{
	char file[] = "a b c d e f g h";

	for (uint8_t i=63; ((i>=7) && (i<=63)); i-=8) {
		printf("%d ", (i/8)+1);
		print_rank_bits(bb1, i);
		printf("\t");
		print_rank_bits(bb2, i);
		printf("\t");
		print_rank_bits(bb3, i);
		printf(" %d\n", (i/8)+1);
	}
	printf("  %s\t%s\t\t%s\n", file, file, file);
	printf("  %#016"PRIx64"\t%#016"PRIx64"\t%#016"PRIx64"\n", *bb1, *bb2, *bb3);
}

/* Print all 12 base bitboards and additional 3 bitboards */
void print_all_bitboards(const struct bitboards * const bb)
{
	if (!bb) {
		dbg_print("NULL pointer\n");
		return;
	}

	uint64_t wp = get_white_pieces(bb);
	uint64_t bp = get_black_pieces(bb);
	uint64_t ap = get_all_pieces(bb);

	printf("\n\twKing\t\t     wQueen\t\t     wBishop\n");
	print_3_bitboards(&bb->wKing, &bb->wQueen, &bb->wBishop);
	printf("\n\twKnight\t\t     wRook\t\t     wPawn\n");
	print_3_bitboards(&bb->wKnight, &bb->wRook, &bb->wPawn);

	printf("\n\tbKing\t\t     bQueen\t\t     bBishop\n");
	print_3_bitboards(&bb->bKing, &bb->bQueen, &bb->bBishop);
	printf("\n\tbKnight\t\t     bRook\t\t     bPawn\n");
	print_3_bitboards(&bb->bKnight, &bb->bRook, &bb->bPawn);

	printf("\n    White Pieces\t  Black Pieces\t\t   All Pieces\n");
	print_3_bitboards(&wp, &bp, &ap);
}

/* Update struct bitboard with the pieces from current board position */
bool update_bitboards(struct board * const brd)
{
	struct bitboards *p = &brd->bb;
	uint64_t *bb;
	bool flag;

	for (uint8_t i=0; i<8; i++) {
		for (uint8_t j=0; j<8; j++) {
			flag = true;
			switch(brd->sqr[i][j]) {
				case WHITE_KING:	bb = &p->wKing;	break;
				case BLACK_KING:	bb = &p->bKing;	break;
				case WHITE_QUEEN:	bb = &p->wQueen; break;
				case BLACK_QUEEN:	bb = &p->bQueen; break;
				case WHITE_BISHOP:	bb = &p->wBishop; break;
				case BLACK_BISHOP:	bb = &p->bBishop; break;
				case WHITE_KNIGHT:	bb = &p->wKnight; break;
				case BLACK_KNIGHT:	bb = &p->bKnight; break;
				case WHITE_ROOK:	bb = &p->wRook;	break;
				case BLACK_ROOK:	bb = &p->bRook;	break;
				case WHITE_PAWN:	bb = &p->wPawn;	break;
				case BLACK_PAWN:	bb = &p->bPawn;	break;
				case EMPTY_SQR:		flag = false;
				default:		flag = false;
			}
			if (flag) {
				set_bb_sqr(bb, (i*8)+j);
			}
		}
	}
}






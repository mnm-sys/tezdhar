/* @file:	tezdhar/src/bitboard.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bitboard.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains bit manipulation functions for bitboard
 *
 */

#include <inttypes.h>	// for uint64_t
#include <stdbool.h>	// for bool
#include <stdio.h>	// for printf
#include "bitboard.h"
#include "chess.h"

/* set bit value from chess board's square index */
static uint64_t set_bb_sqr(uint64_t * const bb, const uint8_t index)
{
	dbg_print("&bb = %p\t*bb = %016"PRIx64"\tindex = %d\n", bb, *bb, index);
	*bb |= (1ULL << index);
	dbg_print("After set bit: bb = %016"PRIx64"\n", *bb);
	return *bb;
}


/* get bit value at given location */
static bool get_bit(const uint64_t * const bb, const uint8_t index)
{
	return (*bb & (1ULL << (index)));
}

/* clear bit at given index */
static void clear_bb_sqr(uint64_t * const bb, const uint8_t index)
{
	get_bit(bb, index) ? *bb ^= (1ULL << index) : 0;
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
static void print_rank_bits(const uint64_t * const bb, const int8_t rank)
{
	for (uint8_t file = A_FILE; file <= H_FILE; file++) {
		get_bit(bb, ((rank * 8) + file)) ? printf("1 ") : printf(". ");
	}
}

/* print bitboard */
static void print_bitboard(const uint64_t * const bb)
{
	for (int8_t rank = RANK_8; rank >= RANK_1; rank--) {
		printf("\n%d ", rank+1);
		print_rank_bits(bb, rank);
	}
	printf("\n  a b c d e f g h\n");
}

/* print 3 distinct bitboards simultaneously, adjacent to each other.
 * We print max 3 bitboards only to honor the 80 coloum terminal width */
static void print_3_bitboards(const uint64_t * const bb1,
		const uint64_t * const bb2, const uint64_t * const bb3)
{
	char file[] = "a b c d e f g h";

	for (int8_t rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d ", rank+1);
		print_rank_bits(bb1, rank);
		printf("\t");
		print_rank_bits(bb2, rank);
		printf("\t");
		print_rank_bits(bb3, rank);
		printf(" %d\n", rank+1);
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

	for (uint8_t r = RANK_1; r <= RANK_8; r++) {
		for (uint8_t f = A_FILE; f <= H_FILE; f++) {
			flag = true;
			switch(brd->sqr[r][f]) {
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
				set_bb_sqr(bb, (r*8)+f);
			}
		}
	}
}






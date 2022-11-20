/* @file:	tezdhar/src/bitboard.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bitboard.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains bit manipulation functions for bitboard
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"	// for HAVE___BUILTIN_POPCOUNTLL, HAVE___BUILTIN_FFSLL
#endif

#include "bitboard.h"	// for GET_BIT, SET_BIT
#include "chess.h"	// for struct bitboard


/* Brian Kernighan’s algorithm: We can use Brian Kernighan’s algorithm to
 * improve the naive bit counting algorithm’s performance. The idea is to only
 * consider the set bits of an integer by turning off its rightmost set bit
 * (after counting it), so the next iteration of the loop considers the next
 * rightmost bit.
 *
 * The expression n & (n-1) can be used to turn off the rightmost set bit of
 * a number n. This works as the expression n-1 flips all the bits after the
 * rightmost set bit of n, including the rightmost set bit itself. Therefore,
 * n & (n-1) results in the last bit flipped of n.
 */
static uint8_t brain_kernighan_algo(uint64_t bb)
{
	uint8_t count = 0;	// bit counter

	/* consecutively reset least significant 1st bit */
	while (bb) {
		count++;	// increment count
		bb &= bb - 1;	// reset least significant 1st bit
	}

	return count;
}


/* count bits within a bitboard
 * TODO: if builtin macros are used pass appropiate
 * linker flags to use hardware popcount */
static uint8_t count_bits(const uint64_t bb)
{
#if defined HAVE___BUILTIN_POPCOUNTLL
	return __builtin_popcountll(bb);
#else
	return brain_kernighan_algo(bb);
#endif
}


/* find first set (ffs) least significant 1st bit index */
static uint8_t get_ls1b(const uint64_t bb)
{
#if defined HAVE___BUILTIN_FFSLL
	return __builtin_ffsll(bb);
#elif defined HAVE_FFSLL
#include <string.h>
	return ffsll(bb);
#else
	/* make sure bitboard is not 0 */
	if (bb) {
		/* count trailing bits before LS1B */
		return count_bits((bb & -bb) - 1);
	} else {
		return 0;
	}
#endif
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
static void print_rank_bits(const uint64_t bb, const int8_t rank)
{
	for (uint8_t file = A_FILE; file <= H_FILE; file++) {
		GET_BIT(bb, ((rank * 8) + file)) ? printf("1 ") : printf(". ");
	}
}


/* print bitboard */
void print_bitboard(const uint64_t bb)
{
	for (int8_t rank = RANK_8; rank >= RANK_1; rank--) {
		printf("\n%d ", rank+1);
		print_rank_bits(bb, rank);
	}
	printf("\n  a b c d e f g h\n");
	printf("popcount = %d\n", count_bits(bb));
	printf("ls1b idx = %d\n", get_ls1b(bb));
}


/* print 3 distinct bitboards simultaneously, adjacent to each other.
 * We print max 3 bitboards only to honor the 80 coloum terminal width */
static void
print_3_bitboards(const uint64_t bb1, const uint64_t bb2, const uint64_t bb3)
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
	print_3_bitboards(bb->wKing, bb->wQueen, bb->wBishop);
	printf("\n\twKnight\t\t     wRook\t\t     wPawn\n");
	print_3_bitboards(bb->wKnight, bb->wRook, bb->wPawn);

	printf("\n\tbKing\t\t     bQueen\t\t     bBishop\n");
	print_3_bitboards(bb->bKing, bb->bQueen, bb->bBishop);
	printf("\n\tbKnight\t\t     bRook\t\t     bPawn\n");
	print_3_bitboards(bb->bKnight, bb->bRook, bb->bPawn);

	printf("\n    White Pieces\t  Black Pieces\t\t   All Pieces\n");
	print_3_bitboards(wp, bp, ap);
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
				case EMPTY_SQR:		flag = false; break;
				default:		dbg_print("Unknown "
							"piece found on board\n");
							return false;
			}
			if (flag) {
				SET_BIT(*bb, (r*8)+f);
			}
		}
	}
	return true;
}


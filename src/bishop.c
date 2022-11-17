/* @file:	tezdhar/src/bishop.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bishop.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	King moves routines and bishop-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* bishop occupancy mask */
static uint64_t bishop_occ_mask[64];


/* mask relevant bishop occupancy bits */
static uint64_t mask_bishop_occ(uint8_t sq)
{
	uint64_t occ = 0ULL;	// bishop occupancy mask bitboard
	int8_t r, f;		// occupancy rank & file of bishop
	uint8_t tr = sq / 8;	// target rank
	uint8_t tf = sq % 8;	// target file

	/* mask NE occupancy bits */
	for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) {
		occ |= BIT((r*8) + f);
	}

	/* mask NW occupancy bits */
	for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) {
		occ |= BIT((r*8) + f);
	}

	/* mask SE occupancy bits */
	for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) {
		occ |= BIT((r*8)+ f);
	}

	/* mask SW occupancy bits */
	for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) {
		occ |= BIT((r*8) + f);
	}

	return occ;
}


/* Initialize bishop attacks lookup table */
void init_bishop_attacks()
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		bishop_occ_mask[sq] = mask_bishop_occ(sq);
#if DEBUG
		printf("Attack map for bishop at [%s]", sqr_to_coords[sq]);
		print_bitboard(bishop_occ_mask[sq]);
#endif
	}
}


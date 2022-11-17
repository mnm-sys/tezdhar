/* @file:	tezdhar/src/rook.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/rook.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	King moves routines and rook-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* rook occupancy mask */
static uint64_t rook_occ_mask[64];


/* mask relevant rook occupancy bits */
static uint64_t mask_rook_occ(const uint8_t sq)
{
	uint64_t occ = 0ULL;		// rook occupancy mask bitboard
	int8_t r, f;			// occupancy rank & file of rook
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask North occupancy bits */
	for (r = tr + 1; r <= 6; r++) {
		occ |= BIT(r * 8 + tf);
	}

	/* mask South occupancy bits */
	for (r = tr - 1; r >= 1; r--) {
		occ |= BIT(r * 8 + tf);
	}

	/* mask East occupancy bits */
	for (f = tf + 1; f <= 6; f++) {
		occ |= BIT(tr * 8 + f);
	}

	/* mask West occupancy bits */
	for (f = tf - 1; f >= 1; f--) {
		occ |= BIT(tr * 8 + f);
	}

	return occ;
}


/* Initialize rook attacks lookup table */
void init_rook_attacks()
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		rook_occ_mask[sq] = mask_rook_occ(sq);
#if DEBUG
		printf("Occupancy mask for rook at [%s]", sqr_to_coords[sq]);
		print_bitboard(rook_occ_mask[sq]);
#endif
	}
}


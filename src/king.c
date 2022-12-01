/* @file:	tezdhar/src/king.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/king.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	King moves routines and king-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* king attacks lookup table */
static uint_fast64_t king_attacks_lut[64];


/* We rely on the compass rose to identify ray-directions with
 * following increments to neighbored squares to generate king attacks.
 *
 *
 * noWe         nort         noEa
 *         +7    +8    +9
 *             \  |  /
 * west    -1 <-  0 -> +1    east
 *             /  |  \
 *         -9    -8    -7
 * soWe         sout         soEa
 */
static uint64_t mask_king_attacks(const enum square sq)
{
	const uint64_t bb = BIT(sq);	// king bitboard

	return SHIFT_N(bb) | SHIFT_S(bb) | SHIFT_E(bb) | SHIFT_W(bb) |
		SHIFT_NE(bb) | SHIFT_NW(bb) | SHIFT_SE(bb) | SHIFT_SW(bb);
}


/* Initialize king attacks lookup table */
void init_king_attacks(void)
{
	for (enum square sq = A1; sq <= H8; sq++) {
		king_attacks_lut[sq] = mask_king_attacks(sq);
#if DEBUG
		printf("Attack map for king at [%s]", sqr_to_coords[sq]);
		print_bitboard(king_attacks_lut[sq]);
#endif
	}
}


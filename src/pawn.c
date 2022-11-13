/* @file:	tezdhar/src/pawn.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/pawn.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	Pawn moves and attack routines
 */

#include <stdint.h>	/* for uint64_t */
#include "chess.h"
#include "bitboard.h"

static uint_fast64_t pawn_attacks[2][64];

static uint64_t mask_pawn_attacks(const enum color turn, const uint8_t sq)
{
	uint64_t attacks = 0ULL;	// resultant pawn attacks bitboard
	uint64_t bb 	 = 0ULL;	// current pawn bitboard

	SET_BIT(bb, sq);		// set pawn position on board

	if (turn == WHITE) {
		attacks |= SHIFT_NW(bb) ? SHIFT_NW(bb) : 0ULL;
		attacks |= SHIFT_NE(bb) ? SHIFT_NE(bb) : 0ULL;
	} else {
		attacks |= SHIFT_SE(bb) ? SHIFT_SE(bb) : 0ULL;
		attacks |= SHIFT_SW(bb) ? SHIFT_SW(bb) : 0ULL;
	}

	return attacks;
}


/* Initialize pawn attacks lookup table */
void init_pawn_attacks(void)
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		pawn_attacks[WHITE][sq] = mask_pawn_attacks(WHITE, sq);
		pawn_attacks[BLACK][sq] = mask_pawn_attacks(BLACK, sq);
	}
}


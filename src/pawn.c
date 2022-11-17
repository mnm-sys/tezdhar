/* @file:	tezdhar/src/pawn.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/pawn.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	Pawn moves and attack routines
 */

#include "chess.h"
#include "bitboard.h"

/* pawn attacks lookup table for white and black pawns */
static uint_fast64_t pawn_attacks_lut[2][64];


static uint64_t mask_pawn_attacks(const enum color turn, const uint8_t sq)
{
	const uint64_t bb = (1ULL << sq);	// current pawn bitboard

	return (turn == WHITE) ?
		SHIFT_NE(bb) | SHIFT_NW(bb):
		SHIFT_SE(bb) | SHIFT_SW(bb);
}


/* Initialize pawn attacks lookup table */
void init_pawn_attacks()
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		pawn_attacks_lut[WHITE][sq] = mask_pawn_attacks(WHITE, sq);
		pawn_attacks_lut[BLACK][sq] = mask_pawn_attacks(BLACK, sq);
#if DEBUG
		printf("\nAttack map for white pawn at [%s]", sqr_to_coords[sq]);
		print_bitboard(pawn_attacks_lut[WHITE][sq]);
		printf("\nAttack map for black pawn at [%s]", sqr_to_coords[sq]);
		print_bitboard(pawn_attacks_lut[BLACK][sq]);
#endif
	}
}

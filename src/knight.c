/* @file:	tezdhar/src/knight.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/knight.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	Kinght routines and attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* knight attacks lookup table */
static uint_fast64_t knight_attacks_lut[64];


/* The Knight attacks the target squares independently from other pieces
 * around. The compass rose of all eight attacking directions associated
 * with the to - from square differences from an 8x8 board:
 *
 *         noNoWe    noNoEa
 *             +15  +17
 *              |     |
 * noWeWe  +6 __|     |__+10  noEaEa
 *               \   /
 *                >0<
 *            __ /   \ __
 * soWeWe -10   |     |   -6  soEaEa
 *              |     |
 *            -17   -15
 *        soSoWe     soSoEa
 */
static uint64_t mask_knight_attacks(const uint8_t sq)
{
	const uint64_t bb = BIT(sq);	// current knight bitboard

	return SHIFT_NNE(bb) | SHIFT_NNW(bb) | SHIFT_NEE(bb) | SHIFT_NWW(bb) |
		SHIFT_SSE(bb) | SHIFT_SSW(bb) | SHIFT_SEE(bb) | SHIFT_SWW(bb);
}


/* Initialize knight attacks lookup table */
void init_knight_attacks()
{
	for (enum square sq = A1; sq <= H8; sq++) {
		knight_attacks_lut[sq] = mask_knight_attacks(sq);
#if DEBUG
		printf("Attack map for knight at [%s]", sqr_to_coords[sq]);
		print_bitboard(knight_attacks_lut[sq]);
#endif
	}
}


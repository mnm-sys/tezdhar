/* @file:	tezdhar/src/rook.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/rook.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GPLv3
 * @desc:	Rook moves routines and rook-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* rook occupancy mask */
static uint64_t rook_occ_mask[64];

/* rook relevant occupancy bit count lookup table. For example if you had a
 * rook on a1, the relevant occupancy bits will be from a2-a7 and b1-g1. */
const uint8_t rook_relevant_occu_bits_lut[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};


/* mask relevant rook occupancy bits */
static uint64_t mask_rook_occ(const uint8_t sq)
{
	uint64_t occ = 0ULL;		// rook occupancy mask bitboard
	int8_t r, f;			// occupancy rank & file
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask North occupancy bits */
	for (r = tr + 1; r <= RANK_7; r++) {
		occ |= SQR(r, tf);
	}

	/* mask South occupancy bits */
	for (r = tr - 1; r >= RANK_2; r--) {
		occ |= SQR(r, tf);
	}

	/* mask East occupancy bits */
	for (f = tf + 1; f <= G_FILE; f++) {
		occ |= SQR(tr, f);
	}

	/* mask West occupancy bits */
	for (f = tf - 1; f >= B_FILE; f--) {
		occ |= SQR(tr, f);
	}

	return occ;
}


/* generate rook attacks on the fly */
static uint64_t generate_rook_attacks(const uint8_t sq, const uint64_t blockers)
{
	uint64_t bb, attacks = 0ULL;	// rook attack mask bitboard
	int8_t r, f;			// attack mask rank and file
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask North attack bits */
	for (r = tr + 1; r <= RANK_8; r++) {
		bb = SQR(r, tf);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask South attack bits */
	for (r = tr - 1; r >= RANK_1; r--) {
		bb = SQR(r, tf);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask East attack bits */
	for (f = tf + 1; f <= H_FILE; f++) {
		bb = SQR(tr, f);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask West attack bits */
	for (f = tf - 1; f >= A_FILE; f--) {
		bb = SQR(tr, f);
		BREAK_IF_BLOCKED(bb);
	}

	return attacks;
}


/* Initialize rook attacks lookup table */
void init_rook_attacks()
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		rook_occ_mask[sq] = mask_rook_occ(sq);
#if DEBUG
		printf("Occupancy mask for rook at [%s]", sqr_to_coords[sq]);
		print_bitboard(rook_occ_mask[sq]);
		printf("Attack mask for rook at [%s]", sqr_to_coords[sq]);
		print_bitboard(generate_rook_attacks(sq, 0ULL));
#endif
	}
}


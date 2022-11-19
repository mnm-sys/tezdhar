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
static uint64_t mask_bishop_occ(const uint8_t sq)
{
	uint64_t occ = 0ULL;		// bishop occupancy mask bitboard
	int8_t r, f;			// occupancy rank & file of bishop
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask NE occupancy bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_7 && f <= G_FILE; r++, f++) {
		occ |= SQR(r, f);
	}

	/* mask NW occupancy bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_7 && f >= B_FILE; r++, f--) {
		occ |= SQR(r, f);
	}

	/* mask SE occupancy bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_2 && f <= G_FILE; r--, f++) {
		occ |= SQR(r, f);
	}

	/* mask SW occupancy bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_2 && f >= B_FILE; r--, f--) {
		occ |= SQR(r, f);
	}

	return occ;
}


/* generate bishop attacks on the fly */
static uint64_t generate_bishop_attacks(const uint8_t sq, const uint64_t blockers)
{
	uint64_t bb, attacks = 0ULL;	// bishop attacks bitboard
	int8_t r, f;			// attacks rank & file
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask NE attack bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_8 && f <= H_FILE; r++, f++) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask NW attack bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_8 && f >= A_FILE; r++, f--) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask SE attack bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_1 && f <= H_FILE; r--, f++) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb);
	}

	/* mask SW attack bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_1 && f >= A_FILE; r--, f--) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb);
	}

	return attacks;
}


/* Initialize bishop attacks lookup table */
void init_bishop_attacks()
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		bishop_occ_mask[sq] = mask_bishop_occ(sq);
#if DEBUG
		printf("Occupancy mask for bishop at [%s]", sqr_to_coords[sq]);
		print_bitboard(bishop_occ_mask[sq]);
		printf("Attack mask for bishop at [%s]", sqr_to_coords[sq]);
		print_bitboard(generate_bishop_attacks(sq, 0ULL));
#endif
	}
}


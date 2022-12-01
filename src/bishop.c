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

static uint64_t Bmask[64];	// bishop occupancy masks
static uint64_t Bmagic[64];	// bishop magic numbers

/* bishop relevant occupancy bit count lookup table */
static const uint8_t bishop_rel_occu_bits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6
};


/* mask relevant bishop occupancy bits excluding edges */
uint64_t bishop_occu_mask(const uint8_t sq)
{
	uint64_t mask = 0ULL;		// bishop occupancy mask bitboard
	int8_t r, f;			// occupancy rank & file of bishop
	const uint8_t tr = sq / 8;	// target rank
	const uint8_t tf = sq % 8;	// target file

	/* mask NE occupancy bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_7 && f <= G_FILE; r++, f++) {
		mask |= SQR(r, f);
	}

	/* mask NW occupancy bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_7 && f >= B_FILE; r++, f--) {
		mask |= SQR(r, f);
	}

	/* mask SE occupancy bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_2 && f <= G_FILE; r--, f++) {
		mask |= SQR(r, f);
	}

	/* mask SW occupancy bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_2 && f >= B_FILE; r--, f--) {
		mask |= SQR(r, f);
	}

	return mask;
}


/* generate bishop attacks on the fly */
uint64_t bishop_attacks_on_the_fly(const int8_t sq, const uint64_t blockers)
{
	uint64_t bb, attacks = 0ULL;	// bishop attacks bitboard
	int8_t r, f;			// attacks rank and file
	const uint8_t tr = sq / 8;	// target rank
	const uint8_t tf = sq % 8;	// target file

	/* mask NE attack bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_8 && f <= H_FILE; r++, f++) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb, attacks, blockers);
	}

	/* mask NW attack bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_8 && f >= A_FILE; r++, f--) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb, attacks, blockers);
	}

	/* mask SE attack bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_1 && f <= H_FILE; r--, f++) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb, attacks, blockers);
	}

	/* mask SW attack bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_1 && f >= A_FILE; r--, f--) {
		bb = SQR(r, f);
		BREAK_IF_BLOCKED(bb, attacks, blockers);
	}

	return attacks;
}


/* Initialize bishop attacks lookup table */
void init_bishop_attacks(void)
{
	for (uint8_t sq = A1; sq <= H8; sq++) {
		Bmask[sq] = bishop_occu_mask(sq);
		Bmagic[sq] = find_magic_number(sq, bishop_rel_occu_bits[sq], BISHOP);
#if DEBUG
		printf("Occupancy mask for bishop at [%s]", sqr_to_coords[sq]);
		print_bitboard(Bmask[sq]);
		//printf("Attack mask for bishop at [%s]", sqr_to_coords[sq]);
		//print_bitboard(bishop_attacks_on_the_fly(sq, 0ULL));
		printf("Bishop magic number[%s] = 0x%llx\n", sqr_to_coords[sq], Bmagic[sq]);
#endif
	}
}


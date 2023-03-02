/* @file:	tezdhar/src/rook.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/rook.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GNU GPLv3
 * @copyright:	2023 (C) Manavendra Nath Manav
 * @desc:	Rook moves routines and rook-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* Rook lookup table for magic index */
static struct magic_lut R_lut[64];

/* Rook attacks table [square][blocker_occupancy_variation] */
static uint64_t Rattacks[64][4096];	// 2048 KiB (32 KiB for each square)


/* mask relevant rook occupancy bits excluding edges */
static uint64_t rook_occu_mask(const int8_t sq)
{
	uint64_t mask = 0ULL;		// rook occupancy mask bitboard
	int8_t r, f;			// occupancy rank & file of rook
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask North occupancy bits */
	for (r = tr + 1; r <= RANK_7; r++) {
		mask |= SQR(r, tf);
	}

	/* mask South occupancy bits */
	for (r = tr - 1; r >= RANK_2; r--) {
		mask |= SQR(r, tf);
	}

	/* mask East occupancy bits */
	for (f = tf + 1; f <= G_FILE; f++) {
		mask |= SQR(tr, f);
	}

	/* mask West occupancy bits */
	for (f = tf - 1; f >= B_FILE; f--) {
		mask |= SQR(tr, f);
	}

	return mask;
}


/* generate rook attacks on the fly */
uint64_t rook_attacks_on_the_fly(const int8_t sq, const uint64_t blockers)
{
	uint64_t bb, attacks = 0ULL;	// rook attacks bitboard
	int8_t r, f;			// attacks rank and file
	const int8_t tr = sq / 8;	// target rank
	const int8_t tf = sq % 8;	// target file

	/* mask North attack bits */
	for (r = tr + 1; r <= RANK_8; r++) {
		bb = SQR(r, tf);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask South attack bits */
	for (r = tr - 1; r >= RANK_1; r--) {
		bb = SQR(r, tf);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask East attack bits */
	for (f = tf + 1; f <= H_FILE; f++) {
		bb = SQR(tr, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask West attack bits */
	for (f = tf - 1; f >= A_FILE; f--) {
		bb = SQR(tr, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	//dbg_print("sq:%d\tblockers:0x%llx\tattacks:0x%llx\n", sq, blockers, attacks);
	return attacks;
}


/* Initialize rook magic numbers for each square. The decision to use pre-
 * calculated magics can be made both during compile time (by using macros)
 * as well as during runtime (by using a boolean flag). Runtime flag is set
 * when find_magic_number() function fails even after max retries */
bool init_rook_magic(const bool use_pre_calc_magic)
{
	enum square sq;
	for (sq = A1; sq <= H8; sq++) {
		/* rook attack mask excluding edges */
		R_lut[sq].mask = rook_occu_mask(sq);

		/* rook relevant occupancy bits count */
		R_lut[sq].obits = count_bits(R_lut[sq].mask);

#ifdef USE_PRE_CALCULATED_MAGIC
		R_lut[sq].magic = rook_magic_numbers[sq];
#else
		if (use_pre_calc_magic) {
			R_lut[sq].magic = rook_magic_numbers[sq];
		} else {
			R_lut[sq].magic = find_magic_number(ROOK, sq, R_lut[sq].mask, R_lut[sq].obits);
		}
#endif

		if (!R_lut[sq].magic) {
			dbg_print("Failed to get rook magic no. for sq %s\n", sqr_to_coords[sq]);
			return false;
		}
		printf("\rGenerating magic numbers for Rook: %.2Lf %%", ((long double)sq / 0.63));
		fflush(stdout);
	}
	printf("\n");

#ifdef DEBUG
	for (sq = A1; sq <= H8; sq++) {
		//printf("Occupancy mask for rook at [%s]", sqr_to_coords[sq]);
		//print_bitboard(R_lut[sq].mask);
		//printf("Rook occupancy mask relevant bits at [%s] = %d\n", sqr_to_coords[sq], R_lut[sq].obits);
		//printf("Rook magic number[%s] = 0x%llx\n", sqr_to_coords[sq], R_lut[sq].magic);
	}
#endif
	return true;
}


/* Initialize rook attacks lookup table */
bool init_rook_attacks(void)
{
	enum square sq;		// board square
	uint64_t occu;		// occupancy variation
	int i, magic_idx;	// occupancy index & magic index

	for (sq = A1; sq <= H8; sq++) {
		printf("\n\n\n <<<<================= [ %d ] =================>>>>\n\n", sq);
		for (i = 0; i < (1 << R_lut[sq].obits); i++) {
			occu = set_occupancy(i, R_lut[sq].obits, R_lut[sq].mask);
			printf("Rook Occu variation [%2d][%4d] = 0x%-16llx\t", sq, i, occu);

			magic_idx = (occu * R_lut[sq].magic) >> (64 - R_lut[sq].obits);
			printf("Relv bits: %2d\tMagic[%2d]: 0x%-16llx\t", R_lut[sq].obits, sq, R_lut[sq].magic);
			printf("Magic index: %4d\t", magic_idx);

			Rattacks[sq][magic_idx] = rook_attacks_on_the_fly(sq, occu);
			printf("Rattacks[%2d][%4d] = 0x%llx\n", sq, magic_idx, Rattacks[sq][magic_idx]);
		}
	}
}


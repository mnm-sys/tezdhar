/* @file:	tezdhar/src/bishop.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bishop.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Nov. 2022
 * @license:	GNU GPLv3
 * @copyright:	2023 (C) Manavendra Nath Manav
 * @desc:	Bishop magic numbers generation and bishop-attacks lookup table
 */

#include "bitboard.h"
#include "chess.h"

/* Bishop lookup table for magic index */
static struct magic_lut B_lut[64];

/* Bishop attacks table [square][blocker_occupancy_variation] */
static uint64_t Battacks[64][512];	// 256 KiB (4 KiB for each square)


/* mask relevant bishop occupancy bits excluding edges */
static uint64_t bishop_occu_mask(const enum square sq)
{
	uint64_t mask = 0ULL;	// bishop occupancy mask bitboard
	signed int r, f;	// occupancy rank & file of bishop
	const int tr = sq / 8;	// target rank
	const int tf = sq % 8;	// target file

	/* mask North East occupancy bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_7 && f <= G_FILE; r++, f++) {
		mask |= SQR(r, f);
	}

	/* mask North West occupancy bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_7 && f >= B_FILE; r++, f--) {
		mask |= SQR(r, f);
	}

	/* mask South East occupancy bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_2 && f <= G_FILE; r--, f++) {
		mask |= SQR(r, f);
	}

	/* mask South West occupancy bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_2 && f >= B_FILE; r--, f--) {
		mask |= SQR(r, f);
	}

	return mask;
}


/* generate bishop attacks on the fly */
uint64_t bishop_attacks_on_the_fly(const enum square sq, const uint64_t blockers)
{
	uint64_t bb, attacks = 0ULL;	// bishop attacks bitboard
	signed int r, f;		// attacks rank and file
	const int tr = sq / 8;		// target rank
	const int tf = sq % 8;		// target file

	/* mask North East attack bits */
	for (r = tr + 1, f = tf + 1; r <= RANK_8 && f <= H_FILE; r++, f++) {
		bb = SQR(r, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask North West attack bits */
	for (r = tr + 1, f = tf - 1; r <= RANK_8 && f >= A_FILE; r++, f--) {
		bb = SQR(r, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask South East attack bits */
	for (r = tr - 1, f = tf + 1; r >= RANK_1 && f <= H_FILE; r--, f++) {
		bb = SQR(r, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	/* mask South West attack bits */
	for (r = tr - 1, f = tf - 1; r >= RANK_1 && f >= A_FILE; r--, f--) {
		bb = SQR(r, f);
		attacks |= bb;
		if (blockers & bb) {
			break;
		}
	}

	//dbg_print("sq:%d\tblockers:0x%llx\tattacks:0x%llx\n", sq, blockers, attacks);
	return attacks;
}


/* Initialize bishop magic numbers for each square. The decision to use pre-
 * calculated magics can be made both during compile time (by using macros)
 * as well as during runtime (by using a boolean flag). Runtime flag is set
 * when find_magic_number() function fails even after max retries */
bool init_bishop_magic(const bool use_pre_calc_magic)
{
	enum square sq;
	for (sq = A1; sq <= H8; sq++) {
		/* bishop attack mask excluding edges */
		B_lut[sq].mask = bishop_occu_mask(sq);

		/* bishop relevant occupancy bits count */
		B_lut[sq].obits = count_bits(B_lut[sq].mask);

		if (use_pre_calc_magic) {
			B_lut[sq].magic = bishop_magic_numbers[sq];
		} else {
			B_lut[sq].magic = find_magic_number(BISHOP, sq, B_lut[sq].mask, B_lut[sq].obits);
		}

		if (!B_lut[sq].magic) {
			dbg_print("Failed to get bishop magic no. for sq %s\n", sqr_to_coords[sq]);
			return false;
		}
		printf("\rGenerating magic numbers for Bishop: %.2f %%", ((double)sq / 0.63));
		fflush(stdout);
	}
	printf("\n");

#ifdef DEBUG
	for (sq = A1; sq <= H8; sq++) {
		//printf("Occupancy mask for bishop at [%s]", sqr_to_coords[sq]);
		//print_bitboard(B_lut[sq].mask);
		//printf("Bishop occupancy relevant bits on [%d] = %d\n", sqr_to_coords[sq], B_lut[sq].obits);
		//printf("Bishop magic number[%s] = 0x%llx\n", sqr_to_coords[sq], B_lut[sq].magic);
	}
#endif
	return true;
}


/* Initialize bishop attacks lookup table */
void init_bishop_attacks(void)
{
	enum square sq;		// board square
	uint64_t occu;		// occupancy variation
	int i, magic_idx;	// occupancy index & magic index

	for (sq = A1; sq <= H8; sq++) {
		//printf("\n\n\n <<<<================= [ %d ] =================>>>>\n\n", sq);
		for (i = 0; i < (1 << B_lut[sq].obits); i++) {
			occu = set_occupancy(i, B_lut[sq].obits, B_lut[sq].mask);

			magic_idx = (int)((occu * B_lut[sq].magic) >> (64 - B_lut[sq].obits));

			Battacks[sq][magic_idx] = bishop_attacks_on_the_fly(sq, occu);
#if DEBUG
			//printf("Bishop Occu variation [%2d][%3d] = 0x%-16llx\t", sq, i, occu);
			//printf("Relv bits: %2d\tMagic[%2d]: 0x%-16llx\t", B_lut[sq].obits, sq, B_lut[sq].magic);
			//printf("Magic index: %3d\t", magic_idx);
			//printf("Battacks[%2d][%3d] = 0x%llx\n", sq, magic_idx, Battacks[sq][magic_idx]);
#endif
		}
	}
}


/* Return bishop attacks for a particular blocker occupancy */
uint64_t get_bishop_attacks(const enum square sq, uint64_t occu)
{
	occu &= B_lut[sq].mask;
	occu *= B_lut[sq].magic;
	occu >>= 64 - B_lut[sq].obits;

	return Battacks[sq][occu];
}


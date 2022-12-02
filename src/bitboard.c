/* @file:	tezdhar/src/bitboard.c
 * @project:	TEZDHAR Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/bitboard.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains bit manipulation functions for bitboard
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"	// for HAVE___BUILTIN_POPCOUNTLL, HAVE___BUILTIN_FFSLL
#endif

#ifdef HAVE_STDLIB_H
#  include <stdlib.h>		// for srandomdev, random
#endif

#ifdef HAVE_STRING_H
#  include <string.h>		// for memset, ffsll
#elif defined HAVE_STRINGS_H
#  include <strings.h>		// for bzero
#endif

#ifdef HAVE_TIME_H
#  include <time.h>		// for clock, time
#endif

#ifdef HAVE_GETPID
#  include <sys/types.h>	// for pid_t
#  include <unistd.h>		// for getpid
#endif

#include "bitboard.h"		// for GET_BIT, SET_BIT
#include "chess.h"		// for struct bitboard


/* arbitrary global random number seed which can
 * be updated by the init_random_seed() function */
static unsigned int random_seed = 1804289383;


/* Bit scan index for de Bruijn sequence for 64-bit bitboard */
static const uint8_t index64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};


/**
 * bitScanForward
 * https://www.chessprogramming.org/index.php?title=BitScan
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
static inline uint8_t __attribute((hot)) bitScanForward(uint64_t bb)
{
	static const uint64_t debruijn64 = 0x03f79d71b4cb0a89ULL;
	//assert (bb != 0);
	return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
}


/* Brian Kernighan’s algorithm: We can use Brian Kernighan’s algorithm to
 * improve the naive bit counting algorithm’s performance. The idea is to only
 * consider the set bits of an integer by turning off its rightmost set bit
 * (after counting it), so the next iteration of the loop considers the next
 * rightmost bit.
 *
 * The expression n & (n-1) can be used to turn off the rightmost set bit of
 * a number n. This works as the expression n-1 flips all the bits after the
 * rightmost set bit of n, including the rightmost set bit itself. Therefore,
 * n & (n-1) results in the last bit flipped of n.
 */
static inline uint8_t __attribute((hot)) brain_kernighan_algo(uint64_t bb)
{
	uint8_t count = 0;	// bit counter

	/* consecutively reset least significant 1st bit */
	while (bb) {
		count++;	// increment count
		bb &= bb - 1;	// reset least significant 1st bit
	}
	return count;
}


/* count bits within a bitboard */
inline int __attribute((hot)) count_bits(const uint64_t bb)
{
#if defined HAVE___BUILTIN_POPCOUNTLL
	return __builtin_popcountll(bb);
#else
	return brain_kernighan_algo(bb);
#endif
}


/* find first set (ffs) least significant 1st bit index */
static inline int __attribute((hot)) get_ls1b(const uint64_t bb)
{
#if defined HAVE___BUILTIN_FFSLL
	return __builtin_ffsll((int64_t)bb);

#elif defined HAVE___BUILTIN_CTZLL
	return bb ? __builtin_ctzll(bb) : 0;

#elif defined HAVE___BUILTIN_POPCOUNTLL
	return bb ? __builtin_popcountll((bb & -bb) -1) : 0;

#elif defined HAVE_FFSLL
	return ffsll(bb);
#else
	return bb ? bitScanForward(bb) : 0;
#endif
}


/* get bitboard containing all white pieces only */
static uint64_t get_white_pieces(const struct bitboards * const bb)
{
	return (bb->wKing | bb->wQueen | bb->wBishop |
			bb->wKnight | bb->wRook | bb->wPawn);
}


/* get bitboard containing all black pieces only */
static uint64_t get_black_pieces(const struct bitboards * const bb)
{
	return (bb->bKing | bb->bQueen | bb->bBishop |
			bb->bKnight | bb->bRook | bb->bPawn);
}


/* get bitboard containing all black pieces present on chessboard */
static uint64_t get_all_pieces(const struct bitboards * const bb)
{
	return (get_white_pieces(bb) | get_black_pieces(bb));
}


/* print bitboard squares of a particular rank */
static void print_rank_bits(const uint64_t bb, const int8_t rank)
{
	for (uint8_t file = A_FILE; file <= H_FILE; file++) {
		GET_BIT(bb, ((rank * 8) + file)) ? printf("1 ") : printf(". ");
	}
}


/* print bitboard */
void print_bitboard(const uint64_t bb)
{
	for (int8_t rank = RANK_8; rank >= RANK_1; rank--) {
		printf("\n%d ", rank+1);
		print_rank_bits(bb, rank);
	}
	printf("\n  a b c d e f g h\n");
	printf("popcount = %d\n", count_bits(bb));
	printf("ls1b idx = %d\n", get_ls1b(bb));
}


/* print 3 distinct bitboards simultaneously, adjacent to each other.
 * We print max 3 bitboards only to honor the 80 coloum terminal width */
	static void
print_3_bitboards(const uint64_t bb1, const uint64_t bb2, const uint64_t bb3)
{
	char file[] = "a b c d e f g h";

	for (int8_t rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d ", rank+1);
		print_rank_bits(bb1, rank);
		printf("\t");
		print_rank_bits(bb2, rank);
		printf("\t");
		print_rank_bits(bb3, rank);
		printf(" %d\n", rank+1);
	}
	printf("  %s\t%s\t\t%s\n", file, file, file);
}


/* Print all 12 base bitboards and additional 3 bitboards */
void print_all_bitboards(const struct bitboards * const bb)
{
	uint64_t wp, bp, ap;

	if (!bb) {
		dbg_print("NULL pointer\n");
		return;
	}

	wp = get_white_pieces(bb);
	bp = get_black_pieces(bb);
	ap = get_all_pieces(bb);

	printf("\n\twKing\t\t     wQueen\t\t     wBishop\n");
	print_3_bitboards(bb->wKing, bb->wQueen, bb->wBishop);
	printf("\n\twKnight\t\t     wRook\t\t     wPawn\n");
	print_3_bitboards(bb->wKnight, bb->wRook, bb->wPawn);

	printf("\n\tbKing\t\t     bQueen\t\t     bBishop\n");
	print_3_bitboards(bb->bKing, bb->bQueen, bb->bBishop);
	printf("\n\tbKnight\t\t     bRook\t\t     bPawn\n");
	print_3_bitboards(bb->bKnight, bb->bRook, bb->bPawn);

	printf("\n    White Pieces\t  Black Pieces\t\t   All Pieces\n");
	print_3_bitboards(wp, bp, ap);
}


/* Update struct bitboard with the pieces from current board position */
bool update_bitboards(struct board * const brd)
{
	struct bitboards *p = &brd->bb;
	uint64_t *bb;
	bool flag;

	for (uint8_t r = RANK_1; r <= RANK_8; r++) {
		for (uint8_t f = A_FILE; f <= H_FILE; f++) {
			flag = true;
			switch(brd->sqr[r][f]) {
				case WHITE_KING:	bb = &p->wKing;	break;
				case BLACK_KING:	bb = &p->bKing;	break;
				case WHITE_QUEEN:	bb = &p->wQueen; break;
				case BLACK_QUEEN:	bb = &p->bQueen; break;
				case WHITE_BISHOP:	bb = &p->wBishop; break;
				case BLACK_BISHOP:	bb = &p->bBishop; break;
				case WHITE_KNIGHT:	bb = &p->wKnight; break;
				case BLACK_KNIGHT:	bb = &p->bKnight; break;
				case WHITE_ROOK:	bb = &p->wRook;	break;
				case BLACK_ROOK:	bb = &p->bRook;	break;
				case WHITE_PAWN:	bb = &p->wPawn;	break;
				case BLACK_PAWN:	bb = &p->bPawn;	break;
				case EMPTY_SQR:		flag = false; break;
				default: dbg_print("Unknown piece found on board\n");
					 return false;
			}
			if (flag) {
				SET_BIT(*bb, (r*8)+f);
			}
		}
	}
	return true;
}


/* Function hash32() uses a combination of bit shifts and integer
 * multiplication to hash the input key. On most machines a bit shift
 * of 3 bits or less, following by an addition can be performed in one
 * cycle. For example, Pentium's 'lea' instruction can be used to good
 * effect to compute a 'shift & add' in one cycle. [http://web.archive.org
 * /web/20070111091013/http://www.concentric.net/~Ttwang/tech/inthash.htm]
 */
static inline uint32_t hash32(int32_t key)
{
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);

	key = (key ^ 61) ^ (key >> 16);
	key = key + (key << 3);
	key = key ^ (key >> 4);
	key = key * 0x27d4eb2d; // multiply by a prime or an odd constant
	key = key ^ (key >> 15);
	return (uint32_t)key;
}


/* 64-bit Mix Functions */
static inline uint64_t hash64(uint64_t key)
{
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	return key;
}


/* 64-bit to 32-bit Hash Functions. One such use for this kind of hash function
 * is to hash a 64 bit virtual address to a hash table index. Because the output
 * of the hash function is narrower than the input, the result is no longer
 * one-to-one. Another usage is to hash two 32 bit integers into one hash value.
 */
static inline uint32_t hash64_to_32(int64_t key)
{
	key = (~key) + (key << 18);	// key = (key << 18) - key - 1;
	key = key ^ (key >> 31);
	key = key * 21;			// key = (key + (key << 2)) + (key << 4);
	key = key ^ (key >> 11);
	key = key + (key << 6);
	key = key ^ (key >> 22);
	return (uint32_t) key;
}


/* Robert Jenkins' 96 bit Mix function to generate random hash seed
 * [[[<<<... http://burtleburtle.net/bob/hash/doobs.html ...>>>]]]
 *
 * Variable 'c' contains the input key. When the mixing is complete, variable
 * 'c' also contains the hash result. Variable 'a', and 'b' contain initialized
 * random bits. Notice the total number of internal state is 96 bits, much
 * larger than the final output of 32 bits. Also notice the sequence of
 * subtractions rolls through variable 'a' to variable 'c' three times. Each row
 * will act on one variable, mixing in information from the other two variables,
 * followed by a shift operation.
 *
 * Subtraction is similar to multiplication in that changes in upper bits of the
 * key do not influence lower bits of the addition. The 9 bit shift operations
 * in Robert Jenkins' mixing algorithm shifts the key to the right 61 bits in
 * total, and shifts the key to the left 34 bits in total. As the calculation is
 * chained, each exclusive-or doubles the number of states. There are at least
 * 2^9 different combined versions of the original key, shifted by various
 * amounts. That is why a single bit change in the key can influence widely
 * apart bits in the hash result.
 *
 * The uniform distribution of the hash function can be determined from the
 * nature of the subtraction operation. Look at a single bit subtraction
 * operation between a key, and a random bit. If the random bit is 0, then the
 * key remains unchanged. If the random bit is 1, then the key will be flipped.
 * A carry will occur in the case where both the key bit and the random bit are
 * 1. Subtracting the random bits will cause about half of the key bits to be
 * flipped. So even if the key is not uniform, subtracting the random bits will
 * result in uniform distribution.
 */
static inline int32_t mix(uint32_t a, uint32_t b, uint32_t c)
{
	a=a-b;  a=a-c;  a=a^(c >> 13);
	b=b-c;  b=b-a;  b=b^(a << 8);
	c=c-a;  c=c-b;  c=c^(b >> 13);
	a=a-b;  a=a-c;  a=a^(c >> 12);
	b=b-c;  b=b-a;  b=b^(a << 16);
	c=c-a;  c=c-b;  c=c^(b >> 5);
	a=a-b;  a=a-c;  a=a^(c >> 3);
	b=b-c;  b=b-a;  b=b^(a << 10);
	c=c-a;  c=c-b;  c=c^(b >> 15);
	return (int32_t)c;
}


/* Init random number generator seed */
void init_random_seed(void)
{
	uint32_t seed = random_seed;

#ifdef HAVE_ARC4RANDOM
	/* Seeding not required for arc4random() */
	dbg_print("PRNG = arc4random()\n");
	return;
#else
#  if defined HAVE_CLOCK && defined HAVE_TIME && defined HAVE_GETPID
	seed = hash32(mix(hash32(clock()), hash32(time(NULL)), hash32(getpid())));
#  endif
#endif

#ifdef HAVE_RANDOM
	dbg_print("PRNG = random()\n");
#  if defined HAVE_SRANDOMDEV
	/* The srandomdev() routine initializes a state array, using the random
	 * number device (/dev/random) which returns good random numbers.
	 */
	srandomdev();
#  elif defined HAVE_SRANDOM
	srandom(seed);
#  endif

#elif defined HAVE_RAND
	dbg_print("PRNG = rand()\n");
#  if defined HAVE_SRANDDEV
	/* The sranddev() function initializes a seed, using the random() random
	 * number device which returns good random numbers
	 */
	sranddev();
#  elif defined HAVE_SRAND
	/* The srand() function sets its argument seed as the seed for a new
	 * sequence of pseudo-random numbers to be returned by rand(). These
	 * sequences are repeatable by calling srand() with the same seed value.
	 */
	srand(seed);
#  endif

#elif defined HAVE_MRAND48
	dbg_print("PRNG = mrand48()\n");
#  if defined HAVE_SRAND48
	/* The srand48() function is used to initialize the internal buffer r(n)
	 * of mrand48(), such that the 32 bits of the seed value are copied into
	 * the upper 32 bits of r(n), with the lower 16 bits of r(n) arbitrarily
	 * being set to 0x330e.
	 */
	srand48(seed);
#  endif
#else
	dbg_print("PRNG = pseudo_random_u32()\n");
	random_seed = seed;
#endif
	dbg_print("Seed Value = %u\n", seed);
}


/* generate 32-bit pseudo random numbers */
#if !defined HAVE_ARC4RANDOM && !defined HAVE_RANDOM && !defined HAVE_RAND && !defined HAVE_MRAND48
static uint32_t pseudo_random_u32(void)
{
	/* get random seed */
	uint32_t n = hash32(mix(random_seed, hash32((int)random_seed), hash32((int)n)));

	/* XOR shift algorithm */
	n ^= n << 13;
	n ^= n >> 17;
	n ^= n << 5;

	/* Re-seed */
	random_seed = hash32(mix(random_seed, hash32((int)random_seed), hash32((int)n)));

	return hash32(mix(random_seed, hash32((int)random_seed), hash32((int)n)));
}
#endif


/* generate 64-bit random number */
static uint64_t random_u64(void)
{
	uint64_t u1, u2, u3, u4;	// four different pseudo random numbers

#ifdef HAVE_ARC4RANDOM
	/* The original version of this random number generator used the RC4
	 * (also known as ARC4) algorithm.  In OS X 10.12 it was replaced with
	 * the NIST-approved AES cipher */
	static uint32_t (*fp)(void) = &arc4random;
	//dbg_print("arc4random()\n");

#elif defined HAVE_RANDOM
	/* The random() function uses a non-linear, additive feedback, random
	 * number generator, employing a default table of size 31 long
	 * integers. It returns successive pseudo-random numbers in the range
	 * from 0 to (2^31)−1. The period of this random number generator is
	 * very large, approximately 16*((2^31)−1). */
	static long (*fp)(void) = &random;
	//dbg_print("random()\n");

#elif defined HAVE_RAND
	/* The rand() function computes a sequence of pseudo-random integers
	 * in the range of 0 to RAND_MAX */
	static int (*fp)(void) = &rand;
	//dbg_print("rand()\n");

#elif defined HAVE_MRAND48
	/* The rand48() family of functions generates pseudo-random numbers,
	 * using a linear congruential algorithm working on integers 48 bits
	 * in size. The particular formula employed is r(n+1) = (a * r(n) +
	 * c) mod m. The default value for the multiplicand `a' is 0x5deece66d
	 * The default value for the the addend `c' is 0xb. The modulo is always
	 * fixed at m = 2 ** 48. r(n) is called the seed of the random number
	 * generator. */
	static int (*fp)(void) = &mrand48;
	//dbg_print("mrand48()\n");

#else
	static uint32_t (*fp)(void) = &pseudo_random_u32;
	//dbg_print("pseudo_random_u32()\n");
#endif

	u1 = (uint64_t)(fp()) & 0xFFFF;
	u2 = (uint64_t)(fp()) & 0xFFFF;
	u3 = (uint64_t)(fp()) & 0xFFFF;
	u4 = (uint64_t)(fp()) & 0xFFFF;

	//dbg_print("u1: 0x%llx, u2: 0x%llx, u3: 0x%llx, u4: 0x%llx\n", u1, u2, u3, u4);

	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}


/* generate 64-bit sparse random number */
static inline uint64_t random_u64_fewbits(void)
{
	return random_u64() & random_u64() & random_u64();
}


/* The 64-bit magic hashing function. The bits in 'key' will get shifted up by
 * multiplication according to a pattern in the magic constant. The result will
 * be shifted down to create an index.
 * @key:	 a sparsely populated input key
 * @magic:	 a magic constant used to hash the key
 * @bits_in_idx: the number of bits in the index
 */
	static inline uint64_t
magic_hashing(const uint64_t key, const uint64_t magic, const uint8_t bits_in_idx)
{
	return ((key * magic) >> (64 - bits_in_idx));
}


/* Calculate every possible blocker board variation for each square/piece combo.
 *
 * blocker mask: A bitboard containing all squares that can block a piece, for a
 * given piece type and the square the piece is on. It excludes terminating edge
 * squares because they always block. The blocker mask is always the same for a
 * given square and piece type.
 *
 * blocker board: A bitboard containing occupied squares. It only has squares
 * which are also in the blocker mask. Blocker boards include friendly & enemy
 * pieces, and it is a subset of the blocker mask.
 *
 * move board: A bitboard containing all squares a piece can move to, given a
 * piece type, a square, and a blocker board. It includes terminating edge
 * squares if the piece can move there. The resulting move board may include
 * moves that capture your own pieces, however these moves are easily removed
 * afterward: moveboard &= ~friendly_pieces;
 *
 * Example for a rook on the e4 square, and there are some random pieces on e2,
 * e5, e7, b4, and c4.
 *
 *      The blocker mask        A blocker board         The move board
 *   8  0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0  8
 *   7  0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0  7
 *   6  0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0  6
 *   5  0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0  5
 *   4  0 1 1 1 0 1 1 0         0 1 1 0 0 0 0 0         0 0 1 1 0 1 1 1  4
 *   3  0 0 0 0 1 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 1 0 0 0  3
 *   2  0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0         0 0 0 0 1 0 0 0  2
 *   1  0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0         0 0 0 0 0 0 0 0  1
 *      a b c d e f g h         a b c d e f g h         a b c d e f g h
 *
 * There are 2^bits blocker boards, where bits is the number of 1's in the
 * blocker mask, which are the only relevant bits. Also, each integer from 0 to
 * 2^bits has a unique sequence of 1's and 0's of length bits. So this function
 * just corresponds each bit in the given integer to a relevant bit in the
 * blocker mask, and turns it off/on accordingly to generate a unique blocker
 * board.
 */
static uint64_t set_occupancy(const int index, const uint8_t bits_in_mask, uint64_t attack_mask)
{
	/* occupancy map */
	uint64_t occupancy = 0ULL;

	/* loop over the range of bits within attack mask */
	for (uint8_t count = 0; count < bits_in_mask; count++)
	{
		/* get LS1B index of attacks mask */
		enum square sq = get_ls1b(attack_mask);

		/* pop LS1B in attack map */
		POP_BIT(attack_mask, sq);

		/* make sure occupancy is on board */
		if (index & (1 << count)) {
			/* populate occupancy map */
			occupancy |= (1ULL << sq);
		}
	}
	return occupancy;
}


static bool init_occupancy_indicies(const enum square sq, const uint8_t relv_bits, const enum chessmen piece,
		const uint64_t attack_mask, uint64_t occupancies[], uint64_t attacks[])
{
	// loop over occupancy indicies
	for (int idx = 0; idx < (1 << relv_bits); idx++)
	{
		// init occupancies
		occupancies[idx] = set_occupancy(idx, relv_bits, attack_mask);

		// init attacks
		if (piece == BISHOP) {
			attacks[idx] = bishop_attacks_on_the_fly(sq, occupancies[idx]);
		} else if (piece == ROOK) {
			attacks[idx] = rook_attacks_on_the_fly(sq, occupancies[idx]);
		} else {
			/* unreachable code for valid piece */
			dbg_print("Invalid piece for Slider magic generation\n");
			return false;
		}
	}
	return true;
}


static void init_used_attacks(uint64_t used_attacks[])
{
#ifdef HAVE_MEMSET
	memset(used_attacks, 0ULL, 4096);
#elif defined HAVE_BZERO
	bzero(used_attacks, 4096);
#else
	for (int idx = 0; idx < 4096; idx++) {
		used_attacks[i] = 0ULL;
	}
#endif
}


/* skip inappropriate magic numbers */
static inline bool skip_magic(const uint64_t attack_mask, const uint64_t magic)
{
	if (count_bits((attack_mask * magic) & BB_RANK_8) < 6) {
		//dbg_print("Skipping inappropiate magic number: %llx\n", magic);
		return true;
	} else {
		return false;
	}
}


/* test magic number for hash collisions */
static bool test_magic(const uint8_t relv_bits, const uint64_t magic,
		const uint64_t occupancies[], const uint64_t attacks[], uint64_t used_attacks[])
{
	// test magic index loop
	for (int idx = 0; (idx < (1 << relv_bits)); idx++) {
		// get magic index
		uint64_t magic_idx = magic_hashing(occupancies[idx], magic, relv_bits);

		// check if magic index works
		if (used_attacks[magic_idx] == 0ULL) {
			used_attacks[magic_idx] = attacks[idx];
		} else {
			if (used_attacks[magic_idx] != attacks[idx]) {
				dbg_print("Hash collision for magic index: %llx\n", magic_idx);
				return false; // magic index doesn't work
			}
		}
	}
	return true;
}


/* The goal of the magic numbers method is to very quickly look up
 * a pre-calculated move board for a given blocker board. Otherwise
 * you'd have to (slowly) calculate the move board every time. This
 * only applies to sliding pieces, namely the rook and bishop. The
 * queen is just a combination of the rook and bishop.
 *
 * Magic numbers can be found for each square & piece type combo.
 * To do this, you have to calculate every possible blocker board
 * variation for each square/piece combo.
 *
 * @piece:		switch for slider piece type (rook or bishop)
 * @attack_mask:	attack mask for a current piece
 * @sq:			source square of the piece
 * @relv_bits:		relevant occupancy bit count (e.g. m=12
 *			for Rook on a1 and m=6 for Bishop on a1
 */
uint64_t find_magic_number(const enum chessmen piece, const enum square sq, const uint64_t attack_mask, const uint8_t relv_bits)
{
	uint64_t occupancies[4096];	// blockers occupancies mask
	uint64_t attacks[4096];		// attack tables for given blockers
	uint64_t used_attacks[4096];	// already used/occupied attacks
	uint64_t magic;			// random magic number candidate

	if (!init_occupancy_indicies(sq, relv_bits, piece, attack_mask, occupancies, attacks)) {
		return 0ULL;
	}

	for (int try = 1; try; try++)
	{
		magic = random_u64_fewbits();
		//dbg_print("sq: %u\ttry: %u\tmagic: 0x%llx\n", sq, try, magic);

		if (skip_magic(attack_mask, magic)) {
			continue;
		}

		init_used_attacks(used_attacks);

		if (test_magic(relv_bits, magic, occupancies, attacks, used_attacks)) {
			return magic;
		}
	}

	dbg_print("Magic number generation failed!\n");
	return 0ULL;
}


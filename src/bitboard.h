/* @file:	tezdhar/src/bitboard.h
 * @url:	https://github.com/mnm-sys/tezdhar/blob/src/bitboard.h
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	22 Oct. 2022
 * @copyright:	GPLv3
 * @desc:
 *
 * Contains bitboard lookup tables for sliding and non-sliding pieces.
 */


/*			----------------------------------------
 *			Chess Board Square Mapping with Bitboard
 *			----------------------------------------
 *
 *	   Layout 0		       Layout 1			   Layout 2
 *
 * 00 01 02 03 04 05 06 07     63 62 61 60 59 58 57 56     56 57 58 59 60 61 62 63 
 * 08 09 10 11 12 13 14 15     55 54 53 52 51 50 49 48     48 49 50 51 52 53 54 55 
 * 16 17 18 19 20 21 22 23     47 46 45 44 43 42 41 40     40 41 42 43 44 45 46 47 
 * 24 25 26 27 28 29 30 31     39 38 37 36 35 34 33 32     32 33 34 35 36 37 38 39 
 * 32 33 34 35 36 37 38 39     31 30 29 28 27 26 25 24     24 25 26 27 28 29 30 31 
 * 40 41 42 43 44 45 46 47     23 22 21 20 19 18 17 16     16 17 18 19 20 21 22 23 
 * 48 49 50 51 52 53 54 55     15 14 13 12 11 10 09 08     08 09 10 11 12 13 14 15 
 * 56 57 58 59 60 61 62 63     07 06 05 04 03 02 01 00     00 01 02 03 04 05 06 07 
 *
 *
 * Let's consider a piece placed on square 'a1', and compare the bitboard
 * values of the above mentioned layout mappings:
 * Layout 0: 0x01_00_00_00_00_00_00_00
 * Layout 1: 0x00_00_00_00_00_00_00_80
 * Layout 2: 0x00_00_00_00_00_00_00_01
 *
 * The advantage of using Layout 1 from the example above is that it maps
 * the 8x8 chessboard in a visibly exact way. The Rank 1 can be viewed
 * directly by inspecting the Least Significant Byte (LSB) of the Layout 1
 * bitboard. Clearly,
 *
 * Rank 1 = [a1 b1 c1 d1 e1 f1 g1 h1] = LSB = 0x80 = 0b1000_0000 shows that
 * 'a1' is set. This makes debugging easier, by having an intuitive view of
 * the chess board by just inspecting the hexadecimal value of the bitboard.
 *
 * Consider, another example (Ng7xf6) of a black knight at g7 capturing a
 * piece at f6. We set both the bits of squares g7 and f6, and compare the
 * bitboard values of the above mentioned layout mappings:
 * Layout 0: 0x00_00_00_00_00_20_00_40
 * Layout 1: 0x02_00_04_00_00_00_00_00
 * Layout 2: 0x40_00_20_00_00_00_00_00
 *
 * The Rank 8 can be viewed directly by inspecting the Most Significant Byte
 * (MSB) of the Layout 1 bitboard. Clearly,
 * Rank 8 = [a8 b8 c8 d8 e8 f8 g8 h8] = MSB = 0x02 = 0b0000_0010 shows that
 * 'g8' is set.
 *
 * Similarly, Rank 6 is the 6th byte from LSB of the bitboard value:
 * Rank 6 = [a6 b6 c6 d6 e6 f6 g6 h6] = 0x04 = 0b0000_0100 clearly shows
 * that 'f6' is set. Similar visual inference can't be derived easily from
 * other layouts except Layout 1.
 *
 * Although, by convention, most other chess engines and publications use
 * Layout 2, we would be using Layout 1 in this project because of it's above
 * mentiontioned advantages.
 */



// pre-calcuclated pawn attacks lookup table
uint64_t white_Pawn_attack_lut = {

};

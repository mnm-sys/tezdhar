/* @file:	tezdhar/src/queen.c
 * @project:	Tezdhar Chess Engine
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/queen.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Mar. 2023
 * @license:	GNU GPLv3
 * @copyright:	2023 (C) Manavendra Nath Manav
 * @desc:	Queen moves routines
 */

#include "chess.h"

/* Return rook attacks for a particular blocker occupancy */
uint64_t get_queen_attacks(const enum square sq, const uint64_t occu)
{
	return get_bishop_attacks(sq, occu) | get_rook_attacks(sq, occu);
}


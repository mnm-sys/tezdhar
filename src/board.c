/* @file:	tezdhar/src/board.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/board.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @copyright:	GPLv3
 * @desc:	Contains routines to update the board position
 */

#include <stdio.h>	// for perror
#include <stdlib.h>	// for malloc
#include <string.h>	// for strcpy
#include "chess.h"
#include "bitboard.h"


#if 0
/* Remove pieces from board and make every square empty */
static void empty_board(struct board *board)
{
	for (int i=0; i<8; i++)
		for (int j=0; j<8; j++)
			board->sqr[i][j] = EMPTY_SQR;
}

static void clear_board_struct(struct board * const brd)
{
	memset(brd, 0, sizeof(struct board));
}

/* Set bitboard's square from file & rank */
static int8_t set_sqr(const uint8_t file, const uint8_t rank)
{

}



/* set bitboard's square from square index using following map
 *
 * 		   Square Mapping to Bit Layout
 *
 * a8 b8 c8 d8 e8 f8 g8 h8		63 62 61 60 59 58 57 56
 * a7 b7 c7 d7 e7 f7 g7 h7		55 54 53 52 51 50 49 48
 * a6 b6 c6 d6 e6 f6 g6 h6		47 46 45 44 43 42 41 40
 * a5 b5 c5 d5 e5 f5 g5 h5		39 38 37 36 35 34 33 32
 * a4 b4 c4 d4 e4 f4 g4 h4    <<==>>	31 30 29 28 27 26 25 24
 * a3 b3 c3 d3 e3 f3 g3 h3		23 22 21 20 19 18 17 16
 * a2 b2 c2 d2 e2 f2 g2 h2		15 14 13 12 11 10 09 08
 * a1 b1 c1 d1 e1 f1 g1 h1		07 06 05 04 03 02 01 00
 */
static uint64_t set_bb_sqr(uint64_t *bb, uint8_t index)
{
	return (*bb = 1 << (63 - index));
}

/* clear bitboard's square index bit to 0 */
static void clear_bb_sqr(uint64_t bb, uint8_t sqr)
{

}
#endif

/* Clear King and Queen side castling rights of both players */
void clear_castling_rights(struct board *board)
{
	for (int i=0; i<4; i++) {
		board->castling[i] = false;
	}
}



/* Get the first occupied square from a bitboard */


/* Get the next occupied square from a bitboard */


/* PopCnt(x) which returns the number of one bits set in the bitboard */

/* Initialize board with given fen string and select players for White
 * and Black. After the FEN is parsed the game status, castling rights,
 * which player has to move are set in the board struct.
 * TODO: free(fen) memory after use
 */
bool init_board(char *fen, struct board *brd, enum player w, enum player b)
{
	memset(brd, 0, sizeof(struct board));
	brd->enpassant = -1;
	brd->whitePlayer = w;
	brd->blackPlayer = b;

	if (!fen) {
		if (!(fen = malloc(MAX_FEN_LEN))) {
			perror("malloc failed");
			return false;
		}
		strcpy(fen, INITIAL_FEN);
	}

	strcpy(brd->fen, fen);
	if (!parse_fen_record(fen, brd)) {
		dbg_print("Unable to parse FEN correctly\n");
		return false;
	}

	update_bitboards(brd);
	print_all_bitboards(&brd->bb);
	return true;
}


/* setup move struct before parsing user input movetext */
void setup_move_struct(const char * const movetext, struct move * const move)
{
	if (!move || !movetext) {
		return;
	}

	strncpy(move->movetext, movetext, MAX_MOVE_LEN);

	move->chessman		= EMPTY;
	move->promoted		= EMPTY;

	move->from_file		= MAX_FILE;
	move->from_rank		= RANK_MAX;
	move->to_file		= MAX_FILE;
	move->to_rank		= RANK_MAX;

	move->castle_ks		= false;
	move->castle_qs		= false;
	move->null		= false;
	move->invalid		= false;
	move->draw_offered	= false;
	move->ep		= false;
	move->capture		= false;
	move->check		= false;
	move->checkmate		= false;
}


/* @file:	tezdhar/src/board.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/board.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains routines to update the board position
 */

#include <stdio.h>	// for perror
#include <stdlib.h>	// for malloc
#include <string.h>	// for strcpy, memset

#include "chess.h"
#include "bitboard.h"


/* Clear King and Queen side castling rights of both players */
void clear_castling_rights(struct board *board)
{
	for (int i=0; i<4; i++) {
		board->castling[i] = false;
	}
}


/* Initialize board with given fen string and select players for White
 * and Black. After the FEN is parsed the game status, castling rights,
 * which player has to move are set in the board struct.
 * TODO: free(fen) memory after use
 */
bool init_board(char *fen, struct board *brd, enum player w, enum player b)
{
	memset(brd, 0, sizeof(struct board));
	brd->enpassant = -1;	//TODO: can also set this to SQ_NB
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
	//dbg_print_all_bitboards(&brd->bb);
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


/* Init leaper pieces (King, Knight and Pawns) attacks lookup table */
void init_leaper_attacks(void)
{
	init_pawn_attacks();
	init_knight_attacks();
	init_king_attacks();
}

void init_magic_numbers(void)
{
#ifdef USE_PRE_CALCULATED_MAGIC
	do {
		init_bishop_magic(true);
		init_rook_magic(true);
	} while(0);
#else
	do {
		init_random_seed();

		if (!init_bishop_magic(false)) {
			printf("Max bishop magic generation retries %u exhaused.\n", MAX_MAGIC_RETRIES);
			printf("Falling back to use pre-calculated bishop magic numbers\n");
			init_bishop_magic(true);
		}

		if (!init_rook_magic(false)) {
			printf("Max rook magic generation retries %u exhaused.\n", MAX_MAGIC_RETRIES);
			printf("Falling back to use pre-calculated rook magic numbers\n");
			init_rook_magic(true);
		}
	} while (0);
#endif
}

/* Init slider pieces (Bishop, Rook and Queen) attacks lookup table */
void init_slider_attacks(void)
{
	init_bishop_attacks();
	init_rook_attacks();
}


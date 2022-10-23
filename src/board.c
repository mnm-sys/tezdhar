/* @file:	tezdhar/src/board.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/board.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @copyright:	GPLv3
 * @desc:	Contains routines to update the board position
 */

#include "chess.h"
#include <stdio.h>	// for perror
#include <stdlib.h>	// for malloc
#include <string.h>	// for strcpy

/* Remove pieces from board and make every square empty */
static void emptyBoard(struct board *board)
{
	for (int i=0; i<8; i++)
		for (int j=0; j<8; j++)
			board->sqr[i][j] = EMPTY_SQR;
}


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
 */
bool init_board(char *fen, struct board *brd, enum player w, enum player b)
{
	emptyBoard(brd);
	if (!fen) {
		if (!(fen = malloc(MAX_FEN_LEN))) {
			perror("malloc failed");
			return false;
		}
		strcpy(fen, INITIAL_FEN);
	}
	strcpy(brd->fen, fen);
	parse_fen_record(fen, brd);
	brd->whitePlayer = w;
	brd->blackPlayer = b;
	return true;
}



/* setup move struct before parsing user input movetext */
void setup_move_struct(const char * const movetext, struct move *move)
{
	strncpy(move->movetext, movetext, MAX_MOVE_LEN);

	move->chessman		= EMPTY;
	move->promoted		= EMPTY;

	move->from_file		= -1;
	move->from_rank		= -1;
	move->to_file		= -1;
	move->to_rank		= -1;

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




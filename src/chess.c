/**********************************************************
 * Project Tezdhar - A simple Chess Engine written in C   *
 **********************************************************
 *                                                        *
 *          +---+---+---+---+---+---+---+---+             *
 *       8  | r | n | b | q | k | b | n | r |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       7  | p | p | p | p | p | p | p | p |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       6  |   |   |   |   |   |   |   |   |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       5  |   |   |   |   |   |   |   |   |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       4  |   |   |   |   |   |   |   |   |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       3  |   |   |   |   |   |   |   |   |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       2  | P | P | P | P | P | P | P | P |             *
 *          +---+---+---+---+---+---+---+---+             *
 *       1  | R | N | B | Q | K | B | N | R |             *
 *          +---+---+---+---+---+---+---+---+             *
 *            a   b   c   d   e   f   g   h               *
 *                                                        *
 **********************************************************
 * Repository:  https://github.com/mnm-sys/tezdhar.git    *
 **********************************************************/


/* @file:	tezdhar/src/chess.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/chess.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	This file contains the main() entry function to setup the
 * 		board and start the chess engine. It also contains routines
 * 		to parse command line options.
 */

#include "chess.h"
#include "bitboard.h"

#include <stdlib.h>	// for exit


static bool is_player_turn(const struct board * const brd)
{
	switch(brd->status) {
		case WHITE_TURN:
		case BLACK_TURN:
		case WHITE_UNDER_CHECK:
		case BLACK_UNDER_CHECK:
		case WHITE_CAN_ACCEPT_DRAW:
		case BLACK_CAN_ACCEPT_DRAW:
			return true;
		default:
			return false;
	}
}


static bool is_human_player(const struct board * const brd)
{
	return (((brd->turn == WHITE) && (brd->whitePlayer == HUMAN)) ||
			((brd->turn == BLACK) && (brd->blackPlayer == HUMAN)));
}


static enum game_status start_game(enum player wPlayer, enum player bPlayer, struct board * const brd)
{
	char movetext[MAX_MOVE_LEN] = "";
	struct move move;
	brd->whitePlayer = wPlayer;
	brd->blackPlayer = bPlayer;
	while(is_player_turn(brd)) {
		if (is_human_player(brd)) {
			print_fen_str(brd);
			print_board(brd);
			do {
				move = parse_input_move(input_user_move(movetext, brd));
			} while (0);// (validateInputMove(input, &move, board));
		} else {
			//                      bestMove(board->turn, board);
		}
		brd->turn = !brd->turn;
	}
	return brd->status;
}


/*
 * Main entry point for the program
 */
int main(int argc, char *argv[])
{
	struct board board;

	printf("Tezdhar Chess Engine %s by %s\n%s\n", VERSION, AUTHOR, URL);
	printf("This is free software: you are free to redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");

	if (!init_board(NULL, &board, HUMAN, AI)) {
		printf("Failed to initialize chess board. Exiting ...\n");
		exit(EXIT_FAILURE);
	}

	init_leaper_attacks();
	init_magic_numbers();
	init_slider_attacks();

//#ifdef TEST_CODE
	// define test bitboard
	U64 occupancy = 0ULL;

	// set blocker pieces on board
	SET_BIT(occupancy, C5);
	SET_BIT(occupancy, F2);
	SET_BIT(occupancy, G7);
	SET_BIT(occupancy, B2);
	SET_BIT(occupancy, G5);
	SET_BIT(occupancy, E2);
	SET_BIT(occupancy, E7);

	// print occupancies
	print_bitboard(occupancy);

	// print rook attacks
	print_bitboard(get_queen_attacks(C2, occupancy));

	// print bishop attacks
	print_bitboard(get_queen_attacks(E3, occupancy));
//#endif

	//start_game(HUMAN, HUMAN, &board);

	return 0;
}

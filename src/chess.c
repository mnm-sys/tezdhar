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
 * @copyright:	GPLv3
 * @desc:
 *
 * This file contains the main() entry function to setup the board
 * and start the chess engine. It also contains routines to parse
 * command line options.
 */

#include "chess.h"
#include <stdio.h>	// for printf


/*
 * Main entry point for the program
 */
int main(int argc, char *argv[])
{
	struct board board;
	struct move move;
	char user_input[MAX_INPUT_LEN];

	printf("Tezdhar Chess Engine %s by %s\n%s\n", VERSION, AUTHOR, URL);
	printf("This is free software: you are free to redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\r\n");

	init_board(NULL, &board, HUMAN, AI);
	print_fen_str(&board);
	print_board(&board);
	print_board_struct_info(&board);

	while (1) {
		printf("\n<< .. Testing updation of move struct .. >>\n");
		printf("Your move: ");
		move = parse_input_move(input_user_move(user_input));
		print_move_struct_info(__FILE__, __LINE__, __func__, &move);
	}

	return 0;
}

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
 * Author: Manavendra Nath Manav                          *
 * Email: manav.nit@gmail.com                             *
 * Repo: https://github.com/mnm-sys/tezdhar.git           *
 * Copyright: 2022 @ Manavendra Nath Manav                *
 **********************************************************/

#include "chess.h"
#include <stdio.h>	// for printf


/*
 * Main entry point for the program
 */
int main(int argc, char *argv[])
{
        struct board board;

        printf("Tezdhar Chess Engine %s by %s\n%s\n", VERSION, AUTHOR, URL);
        printf("This is free software: you are free to redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");

        init_board(NULL, &board, HUMAN, AI);
        print_fen_str(&board);
        print_board(&board);
	print_board_status(&board);

	return 0;
}

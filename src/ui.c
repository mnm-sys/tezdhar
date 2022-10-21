/* @file:	tezdhar/src/ui.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/ui.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @copyright:	GNU GPLv3 2022
 * @desc:
 *
 */

#include <langinfo.h>	// for nl_langinfo
#include <locale.h>	// for setlocale
#include <stdio.h>	// for printf
#include <stdbool.h>	// for bool
#include <string.h>	// for strcmp
#include <wchar.h>	// for wprintf
#include <errno.h>	// for errno

#include "chess.h"

/* Maximum length of Standard Algebraic Notation move
 * text including annotation symbols, if any */
#define MAX_SAN_LEN 16

enum chessmen_utf {
	WCK 	= 0x2654,	// ♔ white chess king
	WCQ 	= 0x2655,	// ♕ white chess queen
	WCR 	= 0x2656,	// ♖ white chess rook
	WCB 	= 0x2657,	// ♗ white chess bishop
	WCN 	= 0x2658,	// ♘ white chess knight
	WCP 	= 0x2659,	// ♙ white chess pawn

	BCK 	= 0x265A,	// ♚ black chess king
	BCQ 	= 0x265B,	// ♛ black chess queen
	BCR 	= 0x265C,	// ♜ black chess rook
	BCB 	= 0x265D,	// ♝ black chess bishop
	BCN 	= 0x265E,	// ♞ black chess knight
	BCP 	= 0x265F	// ♟ black chess pawn
};

enum box_drawing_utf {
	LF	= 0x000A,	// \n line Feed
	CR	= 0x000D,	// \r carriage Return
	NBSP	= 0x00A0,	//   non breaking space

	HRZ	= 0x2501,	// ━ horizontal
	VRT	= 0x2503,	// ┃ vertical

	DW_RHT	= 0x250F,	// ┏ down and right
	DW_LFT	= 0x2513,	// ┓ down and left

	UP_RHT	= 0x2517,	// ┗ up and right
	UP_LFT	= 0x251B,	// ┛ up and left
	VRT_RHT	= 0x2523,	// ┣ vertical and right

	VRT_LFT	= 0x252B,	// ┫ vertical and left
	DW_HRZ	= 0x2533,	// ┳ down and horizontal

	UP_HRZ	= 0x253B,	// ┻ up and horizontal
	VRT_HRZ	= 0x254B	// ╋ vertical and horizontal
};

static void draw_top_border()
{
	wprintf(L"\n%lc%lc%lc%lc%lc%lc", NBSP, NBSP, DW_RHT, HRZ, HRZ, HRZ);

	for (int i=0; i<7; i++) {
		wprintf(L"%lc%lc%lc%lc", DW_HRZ, HRZ, HRZ, HRZ);
	}

	wprintf(L"%lc\n", DW_LFT);
}

static void draw_middle_border()
{
	wprintf(L"\n%lc%lc%lc%lc%lc%lc", NBSP, NBSP, VRT_RHT, HRZ, HRZ, HRZ);

	for (int i=0; i<7; i++) {
		wprintf(L"%lc%lc%lc%lc", VRT_HRZ, HRZ, HRZ, HRZ);
	}

	wprintf(L"%lc\n", VRT_LFT);
}

static void draw_bottom_border()
{
	wprintf(L"\n  %lc%lc%lc%lc", UP_RHT, HRZ, HRZ, HRZ);

	for (int i=0; i<7; i++) {
		wprintf(L"%lc%lc%lc%lc", UP_HRZ, HRZ, HRZ, HRZ);
	}

	wprintf(L"%lc\n%lc", UP_LFT, NBSP);

	for (char file='a'; file<'i'; file++) {
		wprintf(L"%lc%lc%lc%lc", NBSP, NBSP, NBSP, file);
	}

	wprintf(L"%lc%lc\n", CR, LF);
}

static void draw_sqr_box(wchar_t piece)
{
	wprintf(L"%lc%lc%lc%lc", VRT, NBSP, piece, NBSP);
}


static void draw_utf_board(struct board *board)
{
	wchar_t piece;
	draw_top_border();

	for (int i=0; i<8; i++) {
		wprintf(L"%d ", (8-i));
		for (int j=0; j<8; j++) {
			switch (board->sqr[i][j]) {
				case EMPTY_SQR:		piece = NBSP; break; 
				case BLACK_ROOK:	piece = BCR; break;
				case BLACK_KNIGHT:	piece = BCN; break;
				case BLACK_BISHOP:	piece = BCB; break;
				case BLACK_QUEEN:	piece = BCQ; break;
				case BLACK_KING:	piece = BCK; break;
				case BLACK_PAWN:	piece = BCP; break;
				case WHITE_ROOK:	piece = WCR; break;
				case WHITE_KNIGHT:	piece = WCN; break;
				case WHITE_BISHOP:	piece = WCB; break;
				case WHITE_QUEEN:	piece = WCQ; break;
				case WHITE_KING:	piece = WCK; break;
				case WHITE_PAWN:	piece = WCP; break;
				default:		piece = NBSP; break;
			}
			draw_sqr_box(piece);
		}
		wprintf(L"%lc", VRT);
		if (i<7) {
			draw_middle_border();
		}
	}

	draw_bottom_border();
}

static void draw_ascii_board(struct board *board)
{
	printf("\n");
	for (int i=0; i<8; i++) {
		printf("   +---+---+---+---+---+---+---+---+\n%d ", 8-i);
		for (int j=0; j<8; j++) {
			printf(" | ");
			switch(board->sqr[i][j]) {
				case EMPTY_SQR: printf(" ");break;
				case BLACK_ROOK: printf("r"); break;
				case BLACK_KNIGHT: printf("n"); break;
				case BLACK_BISHOP: printf("b"); break;
				case BLACK_QUEEN: printf("q"); break;
				case BLACK_KING: printf("k"); break;
				case BLACK_PAWN: printf("p"); break;
				case WHITE_ROOK: printf("R"); break;
				case WHITE_KNIGHT: printf("N"); break;
				case WHITE_BISHOP: printf("B"); break;
				case WHITE_QUEEN: printf("Q"); break;
				case WHITE_KING: printf("K"); break;
				case WHITE_PAWN: printf("P"); break;
				default: printf("x");
			}
		}
		printf(" |\n");
	}

	printf("   +---+---+---+---+---+---+---+---+\n");
	printf("     a   b   c   d   e   f   g   h\n\n");
}

static bool term_has_unicode()
{
	/* Set a locale for the ctype and multibyte functions.
	 * This controls recognition of upper and lower case,
	 * alphabetic or non-alphabetic characters, and so on.
	 */
	if (!setlocale(LC_CTYPE, "en_US.UTF-8")) {
		dbg_print("Failed to set locale as UTF-8\n");
	}

	/* The nl_langinfo() function shall return a pointer to a 
	 * string containing information relevant to the particular
	 * language or cultural area defined in the current locale.
	 */
	dbg_print("Current locale is: %s\n", nl_langinfo(CODESET));
	return (strcmp(nl_langinfo(CODESET), "UTF-8") == 0) ? true : false; 
}

void print_board(struct board *board)
{
	if (term_has_unicode()) {
		draw_utf_board(board);
	} else {
		draw_ascii_board(board);
	}
}

void print_board_struct_info(struct board *board)
{
	//#ifdef DEBUG
	printf("Game Status: %d\n", board->status);

	printf("Player Turn: ");
	if (board->status == WHITE_TURN) {
		printf("White\n");
	} else {
		if (board->status == BLACK_TURN) {
			printf("Black\n");
		} else {
			printf("None\n");
		}
	}

	printf("Castling Rights:\n");
	printf("White KS: %d\n", board->castling[0]);
	printf("White QS: %d\n", board->castling[1]);
	printf("Black KS: %d\n", board->castling[2]);
	printf("Black QS: %d\n", board->castling[3]);
	printf("En-passant square: %d\n", board->enpassant);
	printf("Half-moves: %d\n", board->halfmove);
	printf("Full-moves: %d\n", board->fullmove);
	//#endif
}

void print_move_struct_info(const char *f, int l, const char *fn, struct move *m)
{
	//#ifdef DEBUG
	int i = printf("-------- Called by: %s:%d:%s() --------\n", f, l, fn);
	const char *chessmen[] = {"King", "Queen", "Knight",
		"Bishop", "Rook", "Pawn", "Empty"};
	const char files[] = "-abcdefgh";
	const char ranks[] = "-87654321";

	printf("move->movetext		= %s\n", m->movetext);
	printf("move->chessman          = %s\n", chessmen[m->chessman]);
	printf("move->promoted          = %s\n", chessmen[m->promoted]);

	printf("move->from_file		= %c\n", files[m->from_file + 1]);
	printf("move->from_rank         = %c\n", ranks[m->from_rank + 1]);
	printf("move->to_file           = %c\n", files[m->to_file + 1]);
	printf("move->to_rank           = %c\n", ranks[m->to_rank + 1]);

	printf("move->castle_ks         = %d\n", m->castle_ks);
	printf("move->castle_qs         = %d\n", m->castle_qs);
	printf("move->null              = %d\n", m->null);
	printf("move->invalid           = %d\n", m->invalid);
	printf("move->draw_offered      = %d\n", m->draw_offered);
	printf("move->ep                = %d\n", m->ep);
	printf("move->capture           = %d\n", m->capture);
	printf("move->check             = %d\n", m->check);
	printf("move->checkmate         = %d\n", m->checkmate);

	while (--i) {
		printf("-");
	}
	printf("\n");
	//#endif
}

char *input_user_move(char *buf)
{
	if (!buf) {
		return NULL;
	}

	if (!fgets(buf, MAX_SAN_LEN, stdin)) {
		perror("fgets() failed");
		return NULL;
	} else {
		/* remove the trailing '\n' */
		buf[strcspn(buf, "\n")] = 0;
		return buf;
	}
}


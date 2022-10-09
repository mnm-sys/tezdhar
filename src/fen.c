/* tezdhar/src/fen.c
 *
 * Contains routines to parse FEN string and update the board
 * position accordingly.
 */

#include "chess.h"	// for struct board
#include <stdio.h>	// for printf, fprintf
#include <ctype.h>	// for isdigit, isspace
#include <string.h>	// for strcpy, strlen


/* Remove pieces from board and make every square empty */
static void emptyBoard(struct board *board)
{
	for (int i=0; i<8; i++)
		for (int j=0; j<8; j++)
			board->sqr[i][j] = EMPTY_SQR;
}
	

/* Parse pieces from FEN string and place them on board 
 * at their designated squares according to FEN
 */
static char *parse_pieces(char *fen, struct board *board)
{
	int i = 0, j = 0;
	enum pieces piece;
	bool valid = fen ? true : false;

	if (!valid) {
		fprintf(stderr, "Error parsing FEN string as it is NULL\n");
		return NULL;
	}

	while (*fen != ' ' && valid) {
		piece = EMPTY_SQR;
		switch(*fen) {
			case 'r': piece = BLACK_ROOK; break;
			case 'n': piece = BLACK_KNIGHT; break;
			case 'b': piece = BLACK_BISHOP; break;
			case 'q': piece = BLACK_QUEEN; break;
			case 'k': piece = BLACK_KING; break;
			case 'p': piece = BLACK_PAWN; break;
			case 'R': piece = WHITE_ROOK; break;
			case 'N': piece = WHITE_KNIGHT; break;
			case 'B': piece = WHITE_BISHOP; break;
			case 'Q': piece = WHITE_QUEEN; break;
			case 'K': piece = WHITE_KING; break;
			case 'P': piece = WHITE_PAWN; break;
			case '/': i++; j = 0; break;
			case '1': break;
			case '2': j += 1; break;
			case '3': j += 2; break;
			case '4': j += 3; break;
			case '5': j += 4; break;
			case '6': j += 5; break;
			case '7': j += 6; break;
			case '8': j = 0; break;
			default: valid = false;
		}

		if (*fen != '/') {
			if(!isdigit(*fen)) {
				board->sqr[i][j] = piece;
			}
			j++;
		}
		valid ? true : fprintf(stderr, "Invalid character in FEN record: %c\n", *fen);
		fen++;
	}

	return valid ? fen : NULL;
}


/* Clear King and Queen side castling rights of both players */
static void clear_castling_rights(struct board *board)
{
	for (int i=0; i<4; i++)
		board->castling[i] = false;
}


/* First parse which player's turn is it to play, then
 * set castling rights for each player. Finally, set the
 * en-passant square position, if any.
 */
static char *parse_flags(char *fen, struct board *board)
{
	char rank = -1, file = -1, field = 0;
	bool turn = true;
	bool valid = fen ? true : false;

	clear_castling_rights(board);
	board->enpassant = -1;

	while (valid && field < 4) {
		switch(*fen) {
			case '-': break;
			case ' ': field++; break;

			/* Since 'b' can represent both black's turn
			 * or file 'b' on board, so we toggle turn
			 * after assigning the player's turn value,
			 * so that when 'b' is encountered again, it
			 * is used to assign en-passant square. */
			case 'b': case 'B': if (turn) {
						board->status = BLACK_TURN;
					    }
					    else
						file = B_file;
					    turn = false;
					    break;	
			case 'w': case 'W': board->status = WHITE_TURN;
					    turn = false;
					    break;

			case 'K': board->castling[WHITE_KS] = true; break;
			case 'Q': board->castling[WHITE_QS] = true; break;
			case 'k': board->castling[BLACK_KS] = true; break;
			case 'q': board->castling[BLACK_QS] = true; break;
			case 'a': file = A_file; break; 
			case 'c': file = C_file; break; 
			case 'd': file = D_file; break; 
			case 'e': file = E_file; break; 
			case 'f': file = F_file; break; 
			case 'g': file = G_file; break; 
			case 'h': file = H_file; break; 
			case '1': rank = RANK_1; break;
			case '2': rank = RANK_2; break;
			case '3': rank = RANK_3; break;
			case '4': rank = RANK_4; break;
			case '5': rank = RANK_5; break;
			case '6': rank = RANK_6; break;
			case '7': rank = RANK_7; break;
			case '8': rank = RANK_8; break;
			default: valid = false;
		}
		valid ? true : fprintf(stderr, "Invalid character in FEN record: %c\n", *fen);
		fen++;
	}

	if ((file > -1) && (rank > -1)) {
		board->enpassant = (rank * 8) + file;
	}

	return valid ? fen : NULL;
}

static bool has_digits_only(char *fen)
{
	if (fen) {
		while (*fen) {
			if (isspace(*fen) || isdigit(*fen)) {
				fen++;
			} else {
				fprintf(stderr, "Invalid character in FEN: %c\n", *fen);
				return false;
			}
		}
		return true;
	} else {
		fprintf(stderr, "Error parsing FEN string as it is NULL\n");
		return false;
	}
}

static bool parse_move_cnt(char *fen, struct board *b)
{
	if (has_digits_only(fen)) {
		sscanf(fen, "%d%d", &(b->halfmove), &(b->fullmove));
		dbg_print("hm = %d, fm = %d\n", b->halfmove, b->fullmove);
		return true;
	} else {
		fprintf(stderr, "FEN contains non-numeric move count: %s\n", fen);
		return false;
	}
}

static void parse_fen_record(char *fen, struct board *board)
{
	parse_move_cnt(parse_flags(parse_pieces(fen, board), board), board);
}


int print_fen_str(struct board *board)
{
	return printf("\nFEN: %s\n", board->fen);
}


/*
 * Initialize board with given fen string and select players for White
 * and Black. After the FEN is parsed the game status, castling rights,
 * which player has to move are set in the board struct.
 */
bool init_board(char *fen, struct board *board, enum player w, enum player b)
{
	emptyBoard(board);
	fen = fen ? fen : INITIAL_FEN;
	strcpy(board->fen, fen);
	parse_fen_record(fen, board);
	board->whitePlayer = w;
	board->blackPlayer = b;
	return true;
}


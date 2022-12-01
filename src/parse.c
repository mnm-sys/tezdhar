/* @file:	tezdhar/src/parse.c
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/parse.c
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	Contains routines to parse FEN, SAN, ICCF and PGN formats
 * 		and update the board position accordingly.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "chess.h"	// for struct board
#include <stdio.h>	// for printf, fprintf
#include <ctype.h>	// for isdigit, isspace
#include <string.h>	// for strcpy, strlen
#include <stdlib.h>	// for free


static enum chessmen get_chessman(const char pos)
{
	switch (pos) {
		case 'K': return KING;
		case 'Q': return QUEEN;
		case 'B': return BISHOP;
		case 'N': return KNIGHT;
		case 'R': return ROOK;
		case 'P': return PAWN;
		default:  return EMPTY;
	}
}


static enum pieces get_piece(const char pos)
{
	switch(pos) {
		case 'r': return BLACK_ROOK;
		case 'n': return BLACK_KNIGHT;
		case 'b': return BLACK_BISHOP;
		case 'q': return BLACK_QUEEN;
		case 'k': return BLACK_KING;
		case 'p': return BLACK_PAWN;
		case 'R': return WHITE_ROOK;
		case 'N': return WHITE_KNIGHT;
		case 'B': return WHITE_BISHOP;
		case 'Q': return WHITE_QUEEN;
		case 'K': return WHITE_KING;
		case 'P': return WHITE_PAWN;
		default:  return EMPTY_SQR;
	}
}


static int8_t get_file_index(const char pos)
{
	switch(pos) {
		case 'a': return A_FILE;
		case 'b': return B_FILE;
		case 'c': return C_FILE;
		case 'd': return D_FILE;
		case 'e': return E_FILE;
		case 'f': return F_FILE;
		case 'g': return G_FILE;
		case 'h': return H_FILE;
		default:  return -1;
	}
}


static int8_t get_rank_index(const char pos)
{
	switch(pos) {
		case '1': return RANK_1;
		case '2': return RANK_2;
		case '3': return RANK_3;
		case '4': return RANK_4;
		case '5': return RANK_5;
		case '6': return RANK_6;
		case '7': return RANK_7;
		case '8': return RANK_8;
		default:  return -1;
	}
}



static void set_castling_rights(struct board * const board, const char pos)
{
	switch(pos) {
		case 'K': board->castling[WHITE_KS] = true; break;
		case 'Q': board->castling[WHITE_QS] = true; break;
		case 'k': board->castling[BLACK_KS] = true; break;
		case 'q': board->castling[BLACK_QS] = true; break;
		default:  break;
	}
}

/* Parse pieces from FEN string and place them on board 
 * at their designated squares according to FEN
 * TODO: Also, parse FEN id, if provided
 */
static char *parse_pieces_from_fen(char *fen, struct board * const board)
{
	int i = RANK_8, j = A_FILE;
	enum pieces piece;
	bool valid = fen ? true : false;

	if (!valid) {
		fprintf(stderr, "Error parsing FEN string as it is NULL\n");
		return NULL;
	}

	while (*fen != ' ' && valid) {
		piece = EMPTY_SQR;
		switch(*fen) {
			case 'r': case 'n': case 'b':
			case 'q': case 'k': case 'p':
			case 'R': case 'N': case 'B':
			case 'Q': case 'K': case 'P':
				piece = get_piece(*fen);
				break;

			case '/': i--; j = A_FILE; break;
			case '1': break;

			case '2': case '3': case '4':
			case '5': case '6': case '7':
				  j += *fen - '1'; break;

			case '8': j = A_FILE; break;
			default:  valid = false;
		}

		if (*fen != '/') {
			if(!isdigit(*fen)) {
				board->sqr[i][j] = piece;
				dbg_print("brd->sqr[%d][%d] = %c\n", i, j, *fen);
			}
			j++;
		}
		valid ? true : fprintf(stderr, "Invalid character in FEN record: %c\n", *fen);
		fen++;
	}

	return valid ? fen : NULL;
}


/* First parse which player's turn is it to play, then
 * set castling rights for each player. Finally, set the
 * en-passant square position, if any.
 */
static char *parse_fen_flags(char *fen, struct board *board)
{
	int8_t rank = -1, file = -1, field = 0;
	bool turn = true;
	bool valid = fen ? true : false;

	clear_castling_rights(board);
	board->enpassant = -1;

	while (valid && field < 4) {
		switch(*fen) {
			case '-': break;
			case ' ': field++; break;

			case 'b': case 'B':
				  /* Since 'b' can represent both black's turn
				   * or file 'b' on board, so we toggle turn
				   * after assigning the player's turn value,
				   * so that when 'b' is encountered again, it
				   * is used to assign en-passant square. */
				  if (turn) {
					  board->status = BLACK_TURN;
				  } else {
					  file = B_FILE;
				  }
				  turn = false;
				  break;	

			case 'w': case 'W':
				  board->status = WHITE_TURN;
				  turn = false;
				  break;

			case 'K': case 'Q': case 'k': case 'q':
				  set_castling_rights(board, *fen);
				  break;

			case 'a': case 'c': case 'd': case 'e':
			case 'f': case 'g': case 'h':
				  file = get_file_index(*fen);
				  break;

			case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8':
				  rank = get_rank_index(*fen);
				  break;

			default: valid = false;
		}
		valid ? true : fprintf(stderr, "Invalid character in FEN record: %c\n", *fen);
		fen++;
	}

	if ((file > -1) && (rank > -1)) {
		board->enpassant = (int8_t)(rank * 8) + file;
	}

	return valid ? fen : NULL;
}


static bool str_has_digits_only(char *fen)
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
	if (str_has_digits_only(fen)) {
		sscanf(fen, "%hu%hu", &(b->halfMoves), &(b->fullMoves));
		dbg_print("hm = %d, fm = %d\n", b->halfMoves, b->fullMoves);
		return true;
	} else {
		fprintf(stderr, "FEN contains non-numeric move count: %s\n", fen);
		return false;
	}
}


bool parse_fen_record(char *fen, struct board *board)
{
	fen = parse_pieces_from_fen(fen, board);
	fen = parse_fen_flags(fen, board);
	return(parse_move_cnt(fen, board));
}


void print_fen_str(struct board *board)
{
	printf("\nFEN: %s\n", board->fen);
}


/* strip substring from a string, and return success if substring is found */
static bool strip_text(char * const str, const char * const delim[], int len)
{
	char *match;
	size_t len_match, len_delim;

	if (!str || !delim) {
		return false;
	}

	for (int i = 0; i < len; i++) {
#ifdef HAVE_STRCASESTR
		match = strcasestr(str, delim[i]);
#else
		match = strstr(str, delim[i]);
#endif
		len_delim = strlen(delim[i]);
		if (match && len_delim) {
			len_match = strlen(match);
			if (len_match >= len_delim) {
				dbg_print("Stripping [%s] from [%s]\n",
						delim[i], str);
				memmove(match, match+len_delim,
						len_match - len_delim + 1);
				dbg_print("Stripped Movetext = %s\n", str);
				return true;
			}
		}
	}

	return false;
}


/* According to UCI protocol specifications, a "nullmove" from the Engine to
 * the GUI should be sent as 0000. A null move just passes the turn to the
 * other side (and possibly forfeits en-passant capturing). Several
 * possibilities present themselves as to what the SAN notation can be,
 * but none claim to be the official standard specification for SAN.
 */
static bool is_null_move(char * const movetext, struct move * const move)
{
	const char * const delim[] = {
		"(null)",
		"00-00",
		"null",		// Stockfish SAN
		"0000",
		"pass",
		"@@@@",		// WinBoard protocol
		"any",
		"Z0",		// ChessAssistant, Aquarium
		"<>",
		"--",		// PGN SAN, Fritz, Chessbase, SCID
		"$0"		// NAG (Numeric Annotation Glyph)
	};
	int len = sizeof(delim)/sizeof(delim[0]);

	if (!movetext) {
		return false;
	}

	if (strip_text(movetext, delim, len)) {
		move->null = true;
		return true;
	} else {
		return false;
	}
}


/* FIXME: Some annotations which use Unicode symbols are not compared here
 * for the sake of brevity. Fixing this issue would also mean inputting the
 * user move in wchar_t format and also reading the PGN file in wchar_t format.
 */
static void strip_evaluation_annotation_symbols(char * const movetext)
{
	/* since the function returns after first match,
	 * the strings are listed in descending length order */
	const char * const annotations[] = {
		"????",			// absurdly bad blunder
		"!!!!",			// extraordinarily brilliant move
		"\?\?!",		// peculiar move
		"!!?", "?!?", "!?!",	// particularly unusual move
		"(?)",			// inferior move
		"(!)",			// objectively good move
		"!!!",			// exceptionally brilliant move
		"???",			// exceptionally bad blunder

		"+/=",			// slight plus position for white
		"=/+",			// slight plus position for black
		"+/-",			// clear plus for white
		"-/+",			// clear plus for black

		"+-",			// decisive advantage for white
		"-+",			// decisive advantage for black

		"??",			// blunder
		"?!",			// dubious move
		"!?",			// interesting move
		"!!",			// brilliant move
		"TN",			// theoretical novelty

		"?",			// mistake
		"!",			// good move
	};
	int len = sizeof(annotations)/sizeof(annotations[0]);

	if (movetext) {
		strip_text(movetext, annotations, len);
	}
}


/* strip equal position annnotation symbols at the end */
static void strip_eq_pos_annotation(char * const movetext)
{
	if (movetext) {
		char *p = strrchr(movetext, '=');

		/* the following check is necessary to distinguish between
		 * equal position annotation and pawn promotion notation */
		if (p) {
			if ((*(p+1) == '\0') || (*(p+1) == ' ')) {
				*p = '\0';
			}
		}
	}
}


/* strip end-of-game annotation symbols */
static void strip_eog_indicators(char * const movetext)
{
	const char * const indicators[] = {
		"White Resigns",
		"Black Resigns",
		"1-0", "0-1",
		"+/-", "-/+",
		"+/+", "-/-"
	};
	int len = sizeof(indicators)/sizeof(indicators[0]);

	if (movetext) {
		strip_text(movetext, indicators, len);
	}
}


/* FIDE specifies draw offers to be recorded by an equals sign
 * with parentheses "(=)" after the move on the score sheet.
 */
static void strip_draw_offered_flag(char * const movetext, struct move * const move)
{
	const char * const indicators[] = {"(=)"};
	int len = sizeof(indicators)/sizeof(indicators[0]);

	if (movetext) {
		if (strip_text(movetext, indicators, len)) {
			move->draw_offered = true;
		}
	}
}


/* strip check to king annotation indicators */
static void strip_check_indicators(char * const movetext, struct move * const move)
{
	const char * const indicators[] = {
		"dis. ch.",		// discovered check
		"dbl. ch.",		// double check
		"ch.", "ch",		// check
		"++",			// double check
		"+"			// check
	};
	int len = sizeof(indicators)/sizeof(indicators[0]);

	if (movetext) {
		if (strip_text(movetext, indicators, len)) {
			move->check = true;
		}
	}
}


/* strip checkmate annotations */
static void strip_checkmate_indicators(char * const movetext, struct move * const move)
{
	const char * const indicators[] = {"mate", "++", "#"};
	int len = sizeof(indicators)/sizeof(indicators[0]);

	if (movetext) {
		if (strip_text(movetext, indicators, len)) {
			move->checkmate = true;
		}
	}
}


/* check for king side castling sequence */
static bool is_ks_castling_seq(char * const movetext, struct move * const move)
{
	const char * const seq[] = {
		"0-0",		// digit zero format (FIDE standard)
		"O-O"		// uppercase letter O (PGN specification)
	};
	int len = sizeof(seq)/sizeof(seq[0]);

	if (!movetext) {
		return false;
	}

	dbg_print("movetext = %s\n", movetext);

	/* QS castling move string "0-0-0" or "O-O-O" must be checked before the
	 * KS castling, otherwise "0-0" will be stripped from "0-0-0" leaving
	 * behind "-0" with false positives about KS castling */
	if ((strcmp(movetext, "0-0-0")==0) || (strcmp(movetext, "O-O-O"))==0) {
		return false;
	}

	if (strip_text(movetext, seq, len)) {
		move->chessman = KING;
		move->castle_ks = true;
		return true;
	} else {
		return false;
	}
}


/* check for queen side castling sequence */
static bool is_qs_castling_seq(char * const movetext, struct move * const move)
{
	const char * const seq[] = {
		"0-0-0",	// digit zero format (FIDE standard)
		"O-O-O"		// uppercase letter O (PGN specification)
	};
	int len = sizeof(seq)/sizeof(seq[0]);

	if (!movetext) {
		return false;
	}

	dbg_print("movetext = %s\n", movetext);

	if (strip_text(movetext, seq, len)) {
		move->chessman = KING;
		move->castle_qs = true;
		return true;
	} else {
		return false;
	}
}


/* When a pawn promotes, the piece promoted to is indicated at the end of the
 * move notation, for example: e8Q (promoting to queen). In standard FIDE
 * notation, no punctuation is used; in Portable Game Notation (PGN) and many
 * publications, pawn promotion is indicated by the equals sign (e8=Q). Other
 * formulations used in chess literature include parentheses (e.g. e8(Q)) and
 * a forward slash (e.g. e8/Q).
 */
static bool is_pawn_promotion(char * const movetext, struct move * const move)
{
	char *p = NULL;
	bool flag = false;
	const char * const delim[] = {
		"8=", "8(", "8/", "8",		// white side pawn promotion
		"1=", "1(", "1/", "1"};		// black side pawn promotion
	int len = sizeof(delim)/sizeof(delim[0]);

	if (!movetext) {
		return false;
	}

	for (int i = 0; i < len; i++) {
		p = strstr(movetext, delim[i]);
		if (p) {
			if (*(p+1)) {
				/* FIDE notation without punctuation */
				switch (*(p+1)) {
					/* UCI protocol allows for use of
					 * lowercase letters for piece
					 * promotion, for example e7e8q */
					case 'Q': case 'q':
					case 'R': case 'r':
					case 'N': case 'n':
					case 'B': move->promoted = \
						  get_chessman(*(p+1));
						  flag = true;
						  break;
					case 'b': /* check for special moves
						     like f8b4, a8b8, a1a5,
						     c8b7, a1b1, Qc8b8, etc. */
						  if (strlen(p+1) == 1) {
							  move->promoted = BISHOP;
							  flag = true;
						  } break;
					default:  break;
				}
				if (flag) {
					*(p+1) = '\0';
					move->chessman = PAWN;
					return true;
				} else {
					/* PGN and other formulations */
					switch (*(p+1)) {
						case '=':
						case '/':
						case '(': flag = true; break;
						default:  break;
					}
				}
			}
		}
		if (flag) {
			break;
		}
	}

	if (flag) {
		if (strlen(p) > 2) {
			move->promoted = get_chessman(*(p+2));
			if (move->promoted != EMPTY) {
				*(p+1) = '\0';
				move->chessman = PAWN;
				return true;
			}
		}
	}

	return false;
}


static bool is_pawn_move(const char * const movetext, struct move * const move)
{
	const char delim[] = "KQBNR";

	if (!movetext) {
		return false;
	}

	if (strpbrk(movetext, delim)) {
		move->invalid = true;
		return false;
	} else {
		move->chessman = PAWN;
		return true;
	}
}


static bool strip_ep_suffix(char * const movetext, struct move * const move)
{
	const char * const suffix[] = {"e.p.","ep.", "ep"};
	int len = sizeof(suffix)/sizeof(suffix[0]);

	if (!movetext) {
		return false;
	}

	if (strip_text(movetext, suffix, len)) {
		move->ep = true;
		return true;
	} else {
		return false;
	}
}


static int get_moving_piece_count(const char * const movetext)
{
	int count = 0;

	if (movetext) {
		for (size_t i=0; i<strlen(movetext); i++) {
			if (get_chessman(movetext[i]) != EMPTY) {
				count++;
			}
		}
	}

	return count;
}


static int get_x_symbol_count(const char * const movetext)
{
	int count = 0;

	if (movetext) {
		for (size_t i=0; i<strlen(movetext); i++) {
			if (movetext[i] == 'x') {
				count++;
			}
		}
	}

	return count;
}


static bool move_has_valid_chars(const char * const movetext, struct move *move)
{
	const char charset[] = "KQBNRabcdefghx12345678";
	size_t span = strspn(movetext, charset);

	if (!movetext) {
		return false;
	}

	if (strlen(movetext) != span) {
		move->invalid = true;
		dbg_print("Move: %s contains invalid character: '%c'\n",
				move->movetext, movetext[span]);
		return false;
	}

	/* each valid move should have only one moving piece */
	if (get_moving_piece_count(movetext) > 1) {
		move->invalid = true;
		dbg_print("Invalid: Move has multiple moving pieces: %s\n",
				move->movetext);
		return false;
	}

	/* each valid move should make only one capture, if any */
	if (get_x_symbol_count(movetext) > 1) {
		move->invalid = true;
		dbg_print("Invalid: Move has multiple captures: %s\n",
				move->movetext);
		return false;
	}

	return true;
}


static bool strip_char_from_string(char * const movetext, const char ch)
{
	char *match = NULL;

	if (!movetext) {
		return false;
	}

	match = strchr(movetext, ch);
	if (match) {
		size_t len = strlen(match);
		if (!len) {
			return false;
		}

		dbg_print("Stripping first [%c] from [%s]\n", ch, movetext);
		memmove(match, match+1, len);
		dbg_print("Stripped Movetext = %s\n", movetext);
		return true;
	}

	return false;
}


static void strip_non_essential_symbols(char * const movetext)
{
	if (movetext) {
		strip_char_from_string(movetext, 'P');
		strip_char_from_string(movetext, '-');
		//strip_char_from_string(movetext, 'O');
	}
}


/* UCI move format using from-to square notation, for example,
 * d3d7 or d3-d7 or d3xd7 are all equivalent moves */
static bool is_uci_move_format(char * const movetext)
{
	size_t len;
	char *buf;
	const char charset[] = "abcdefgh12345678";

	if (!movetext) {
		return false;
	}

	/* Since '-' symbol has been stripped earlier, we need to strip only
	 * the symbol 'x', if present. Note that we are stripping the duplicate
	 * move text instead of the original, so that the original is retained
	 * unstripped, in case if this is not UCI format */
	buf = strdup(movetext);
	strip_char_from_string(buf, 'x');
	len = strlen(buf);

	/* note that the promoted piece, if any, has been
	 * stripped earlier, so the max lenght is 4 only */
	if ((len != 4) || (strspn(buf, charset) != len)) {
		return false;
	}

	if ((islower(buf[0]) == 0) || (islower(buf[2]) == 0)) {
		return false;
	}

	if ((isdigit(buf[1]) == 0) || (isdigit(buf[3]) == 0)) {
		return false;
	}

	free(buf);
	return true;
}


static void parse_stripped_uci_move(char *movetext, struct move *move)
{
	int8_t i = 0, *p[] = {
		&move->from_file,
		&move->from_rank,
		&move->to_file,
		&move->to_rank
	};
	size_t len = strlen(movetext);

	if (len > 4) {
		if (strip_char_from_string(movetext, 'x')) {
			move->capture = true;
		}
		len = strlen(movetext);
	}

	if ((movetext) && (len == 4)) {
		while (*movetext) {
			if (i%2) {
				*p[i] = get_rank_index(movetext[0]);
			} else {
				*p[i] = get_file_index(movetext[0]);
			}
			movetext++;
			i++;
		}
	}
}


/* parse 2 symbols of non-capture SAN move */
static bool parse_2_sym_nc_san(const char *movetext, struct move * const move)
{
	if ((!movetext) || (!move)) {
		return false;
	} else {
		if (strlen(movetext) != 2) {
			return false;
		}
	}

	if (islower(movetext[0])) {
		move->chessman = PAWN;
		move->to_file = get_file_index(movetext[0]);
	} else {
		move->invalid = true;
		return false;
	}

	if (isdigit(movetext[1])) {
		move->to_rank = get_rank_index(movetext[1]);
	} else {
		move->invalid = true;
		return false;
	}

	return true;
}


/* parse 3 symbols of non-capture SAN move */
static bool parse_3_sym_nc_san(const char *movetext, struct move * const move)
{
	if ((!movetext) || (!move)) {
		return false;
	} else {
		if (strlen(movetext) != 3) {
			return false;
		}
	}

	if (isupper(movetext[0])) {
		move->chessman = get_chessman(movetext[0]);
	} else {
		move->invalid = true;
		return false;
	}

	if (islower(movetext[1])) {
		move->to_file = get_file_index(movetext[1]);
	} else {
		move->invalid = true;
		return false;
	}

	if (isdigit(movetext[2])) {
		move->to_rank = get_rank_index(movetext[2]);
	} else {
		move->invalid = true;
		return false;
	}

	return true;
}


/* parse 4 symbols of non-capture SAN moves like Nbd7, Rae1, Rac8, Nge4, etc. */
static bool parse_4_sym_nc_san(const char *movetext, struct move * const move)
{
	if ((!movetext) || (!move)) {
		return false;
	} else {
		if (strlen(movetext) != 4) {
			return false;
		}
	}

	if (isupper(movetext[0])) {
		move->chessman = get_chessman(movetext[0]);
	} else {
		move->invalid = true;
		return false;
	}

	if (islower(movetext[1])) {
		move->from_file = get_file_index(movetext[1]);
	} else {
		if (isdigit(movetext[1])) {
			move->from_rank = get_rank_index(movetext[1]);
		} else {
			move->invalid = true;
			return false;
		}
	}

	if (islower(movetext[2])) {
		move->to_file = get_file_index(movetext[2]);
	} else {
		move->invalid = true;
		return false;
	}

	if (isdigit(movetext[3])) {
		move->to_rank = get_rank_index(movetext[3]);
	} else {
		move->invalid = true;
		return false;
	}

	return true;
}


/* parse 5 symbols of non-capture SAN move like Qh4e1, etc. */
static bool parse_5_sym_nc_san(const char *movetext, struct move * const move)
{
	if ((!movetext) || (!move)) {
		return false;
	} else {
		if (strlen(movetext) != 5) {
			return false;
		}
	}

	if (isupper(movetext[0])) {
		move->chessman = get_chessman(movetext[0]);
	} else {
		move->invalid = true;
		return false;
	}

	if (islower(movetext[1])) {
		move->from_file = get_file_index(movetext[1]);
	} else {
		move->invalid = true;
		return false;
	}

	if (isdigit(movetext[2])) {
		move->from_rank = get_rank_index(movetext[2]);
	} else {
		move->invalid = true;
		return false;
	}

	if (islower(movetext[3])) {
		move->to_file = get_file_index(movetext[3]);
	} else {
		move->invalid = true;
		return false;
	}

	if (isdigit(movetext[4])) {
		move->to_rank = get_rank_index(movetext[4]);
	} else {
		move->invalid = true;
		return false;
	}

	return true;
}


static void parse_non_capture_san_move(char *movetext, struct move *move)
{
	if (!parse_2_sym_nc_san(movetext, move)) {
		if (!parse_3_sym_nc_san(movetext, move)) {
			if (!parse_4_sym_nc_san(movetext, move)) {
				parse_5_sym_nc_san(movetext, move);
			}
		}
	}
}


/* parse single length from-token of SAN capture moves
 * like bxa8, gxf5, Rxb7, Nxe5, Qxf7+, etc. */
static bool parse_1_sym_from_token(const char *tok, struct move * const m)
{
	if ((!tok) || (!m)) {
		return false;
	} else {
		if (strlen(tok) != 1) {
			return false;
		}
	}

	if (isupper(tok[0])) {
		m->chessman = get_chessman(tok[0]);
		return true;
	} else {
		if (islower(tok[0])) {
			m->chessman = PAWN;
			m->from_file = get_file_index(tok[0]);
			return true;
		} else {
			m->invalid = true;
			dbg_print("Illegal from-token: %c in move: %s\n",
					tok[0], m->movetext);
			return false;
		}
	}

	return false;
}


/* parse double length from-token of SAN capture moves
 * like Qgxf7, R7xd5, b4xc5, b7xa8Q, Rdxe5, etc. */
static bool parse_2_sym_from_token(const char *tok, struct move * const m)
{
	if ((!tok) || (!m)) {
		return false;
	} else {
		if (strlen(tok) != 2) {
			return false;
		}
	}

	/* Parse and process first symbol */
	if (isupper(tok[0])) {
		m->chessman = get_chessman(tok[0]);
	} else {
		if (islower(tok[0])) {
			m->chessman = PAWN;
			m->from_file = get_file_index(tok[0]);
		} else {
			m->invalid = true;
			dbg_print("Illegal from-token: %c in move: %s\n",
					tok[0], m->movetext);
			return false;
		}
	}

	/* Parse and process second symbol */
	if (isupper(tok[1])) {
		m->invalid = true;
		dbg_print("Illegal from-token: %c in move: %s\n",
				tok[1], m->movetext);
		return false;
	} else {
		if (islower(tok[1])) {
			m->from_file = get_file_index(tok[1]);
			return true;
		} else {
			if (isdigit(tok[1])) {
				if (m->chessman == EMPTY) {
					m->chessman = PAWN;
				}
				m->from_rank = get_rank_index(tok[1]);
				return true;
			} else {
				m->invalid = true;
				dbg_print("Illegal from-token: %c in move: %s\n",
						tok[1], m->movetext);
				return false;
			}
		}
	}
}


/* parse triple length from-token of SAN capture moves like Qh4xe1, etc. */
static bool parse_3_sym_from_token(const char *tok, struct move * const m)
{
	if ((!tok) || (!m)) {
		return false;
	} else {
		if (strlen(tok) != 3) {
			return false;
		}
	}

	/* Parse and process first symbol */
	if (isupper(tok[0])) {
		m->chessman = get_chessman(tok[0]);
	} else {
		m->invalid = true;
		dbg_print("Illegal from-token: %c in move: %s\n",
				tok[0], m->movetext);
		return false;
	}

	/* Parse and process second symbol */
	if (islower(tok[1])) {
		m->from_file = get_file_index(tok[1]);
	} else {
		m->invalid = true;
		dbg_print("Illegal from-token: %c in move: %s\n",
				tok[1], m->movetext);
		return false;
	}

	/* Parse and process third symbol */
	if (isdigit(tok[2])) {
		m->from_rank = get_rank_index(tok[2]);
	} else {
		m->invalid = true;
		dbg_print("Illegal from-token: %c in move: %s\n",
				tok[2], m->movetext);
		return false;
	}

	return true;
}


/* Parse single length to-square token of SAN capture move */
static bool parse_1_sym_to_sqr_tok(char * const tok, struct move * const m)
{
	if ((!tok) || (!m)) {
		return false;
	} else {
		if (strlen(tok) != 1) {
			return false;
		}
	}

	if (islower(tok[0])) {
		m->to_file = get_file_index(tok[0]);
		return true;
	} else {
		m->invalid = true;
		dbg_print("Illegal to-square token: %c in move: %s\n",
				tok[0], m->movetext);
		return false;
	}
}


/* Parse double length to-square token of SAN capture move */
static bool parse_2_sym_to_sqr_tok(char * const tok, struct move * const m)
{
	char *sym = NULL;

	if ((!tok) || (!m)) {
		return false;
	} else {
		if (strlen(tok) != 2) {
			return false;
		}
	}

#ifdef HAVE_STRNDUP
	sym = strndup(tok, 1);
#elif defined HAVE_STRDUP
	sym = strdup(tok);
#endif

	if (sym) {
		sym[1] = '\0';
	} else {
		perror("str[n]dup failed");
		return false;
	}

	if (parse_1_sym_to_sqr_tok(sym, m)) {
		if (isdigit(tok[1])) {
			m->to_rank = get_rank_index(tok[1]);
			return true;
		} else {
			m->invalid = true;
			dbg_print("Illegal to-square token: %c in move: %s\n",
					tok[1], m->movetext);
		}
	}

	free(sym);
	return false;
}


static bool parse_san_capture_move(char * const movetext, struct move *move)
{
	char *saveptr, *token;

	/* Step 1: find if it's a capture move, if yes, then seperate the
	 * move into from-square and to-square tokens, then process each
	 * token seperately */
	if ((!movetext) || (!move)) {
		return false;
	}

	token = strtok_r(movetext, "x", &saveptr);

	if (!token) {
		return false;
	}

	if (!parse_1_sym_from_token(token, move)) {
		if (!parse_2_sym_from_token(token, move)) {
			if (!parse_3_sym_from_token(token, move)) {
				dbg_print("Invalid from-token: %s in move: %s\n",
						token, move->movetext);
				return false;
			}
		}
	}

	token = strtok_r(NULL, "x", &saveptr);

	if (!token) {
		move->invalid = true;
		return false;
	} else {
		move->capture = true;
	}

	if (!parse_2_sym_to_sqr_tok(token, move)) {
		if (!parse_1_sym_to_sqr_tok(token, move)) {
			dbg_print("Invalid to-token: %s in move: %s\n",
					token, move->movetext);
			return false;
		}
	}

	return true;
}


static void parse_stripped_san_move(char *movetext, struct move *move)
{
	if (movetext) {
		if (strchr(movetext, 'x')) {
			parse_san_capture_move(movetext, move);
		} else {
			parse_non_capture_san_move(movetext, move);
		}
	}
}


static void strip_annotations(char * const movetext, struct move * const move)
{
	if (movetext) {
		/* Step 2: first get rid of annotations at the end */
		strip_evaluation_annotation_symbols(movetext);
		strip_eq_pos_annotation(movetext);
		strip_eog_indicators(movetext);

		/* Step 2.5: check if player has offered draw */
		strip_draw_offered_flag(movetext, move);

		/* Step 3: strip check and checkmate suffix indicators */
		strip_check_indicators(movetext, move);
		strip_checkmate_indicators(movetext, move);
	}
}


/* TODO: Check for UCI castling moves (e1g1) */
static bool is_castling_move(char * const movetext, struct move * const move)
{
	/* Step 3.5: check for castling move */
	if (!movetext) {
		return false;
	}

	dbg_print("movetext = %s\n", movetext);

	if (is_qs_castling_seq(movetext, move) ||
			is_ks_castling_seq(movetext, move))
	{
		dbg_print("movetext = %s\n", movetext);
		if (strlen(movetext)) {
			move->invalid = true;
			dbg_print("Invalid chars in castling move: %s\n",
					move->movetext);
		}
		return true;
	} else {
		return false;
	}
}


static bool is_special_move(char * const movetext, struct move * const move)
{
	if (movetext) {
		if (is_castling_move(movetext, move)) {
			return true;
		} else {
			if (is_pawn_promotion(movetext, move) ||
					strip_ep_suffix(movetext, move)) {
				if (is_pawn_move(movetext, move)) {
					return true;
				}
			}
		}
	}

	return false;
}


static bool extra_checks_for_legality(const char * const movetext, struct move *move)
{
	/* The king cannot give check to another king i.e. it's not
	 * possible to give check or checkmate with a king's move */

	/* The promoted piece cannot be a pawn or a king */

	/* The bishop cannot move to the same file or rank, for example,
	 * the bishop on 'd' file cannot move to any squares on the 'd'
	 * file. Similarly, the bishop on rank '6' cannot move to any
	 * squares present on the same rank. */

	/* The rook can move either in it's own file or in it's own rank */
	return true;
}

/* clean up the move string. This gets rid of any extra
 * annotations and things like 'x', '+', '=' and so forth. */
static bool clean_move(char * const movetext, struct move *move)
{

	/* Step 1: check for null move */
	if (is_null_move(movetext, move)) {
		return true;
	}
	dbg_print("After is_null_move(): movetext = %s\n", movetext);

	strip_annotations(movetext, move);
	dbg_print("After strip_annotations(): movetext = %s\n", movetext);

	if (is_special_move(movetext, move)) {
		if (move->invalid) {
			return true;
		}
	}
	dbg_print("After is_special_move(): movetext = %s\n", movetext);

	/* Step 5.5: Now, since all annnotations are stripped
	 * check that move text contains only valid chars */
	strip_non_essential_symbols(movetext);
	if (!move_has_valid_chars(movetext, move)) {
		dbg_print("movetext = %s\n", movetext);
		return true;
	}
	dbg_print("After move_has_valid_chars(): movetext = %s\n", movetext);

	return false;
}


/* Parse Standard Algebraic Notation (SAN) (e4) and Universal Chess Interface (UCI)
 * (e2e4) move format. The user may input both short as well as long algebraic
 * notation moves. For example, the shorter SAN notation "Nc3" is equivalent
 * to the longer UCI notation "b2c3". The function handles both the short and
 * long notation formats interchangeably.
 */
struct move parse_input_move(char * const movetext)
{
	struct move move;

	/* Step 0: setup move struct */
	if (movetext == NULL) {
		dbg_print("Invalid: Move string is empty");
		setup_move_struct("", &move);
		move.invalid = true;
		return move;
	} else {
		setup_move_struct(movetext, &move);
	}

	if (clean_move(movetext, &move)) {
		return move;
	}

	/* Step 6: check for UCI from/to square move format */
	if (is_uci_move_format(movetext)) {
		/* Step 7: parse UCI move */
		dbg_print("movetext = %s\n", movetext);
		parse_stripped_uci_move(movetext, &move);
	} else {
		/* Step 8: parse SAN move */
		dbg_print("movetext = %s\n", movetext);
		parse_stripped_san_move(movetext, &move);
	}

	if (extra_checks_for_legality(movetext, &move)) {
		move.invalid = true;
	}

	return move;
}


/* tezdhar/src/chess.h
 *
 * This header file contains various constants and initial values required
 * by the chess engine like FEN string for starting position, declarations
 * for bitboards for various pieces, etc.
 */

#ifndef __CHESS_H__
#define __CHESS_H__	1

#include <stdbool.h>	// for bool

#define VERSION "0.1"
#define AUTHOR "Manavendra Nath Manav"
#define URL "https://github.com/mnm-sys/tezdhar.git"

#define MAX_FEN_LEN 88		// 87 plus 1 for Null terminator
#define MAX_INPUT_LEN 128	// max user input length

/* Initial Forsyth–Edwards Notation (FEN) of a chess game */
#define INITIAL_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/* This assumes you are using C99 (the variable argument list notation
 * is not supported in earlier versions). The do { ... } while (0) idiom
 * ensures that the code acts like a statement (function call). The 
 * unconditional use of the code ensures that the compiler always checks 
 * that your debug code is valid — but the optimizer will remove the code 
 * when DEBUG is 0. "fmt" is a printf format string. "..." is whatever 
 * extra arguments fmt requires (possibly nothing). GCC allows you to omit
 * the comma that would normally appear after the last 'fixed' argument to 
 * the macro. It also allows you to use ##__VA_ARGS__ in the macro 
 * replacement text, which deletes the comma preceding the notation if, 
 * but only if, the previous token is a comma.
 */
#ifdef DEBUG
#define dbg_print(fmt, ...) do { fprintf(stderr, "%s:%d:%s(): " fmt, \
		__FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif

/* type of chess player */
enum player {
	AI = 0,		// this chess engine
	HUMAN = 1,	// human player
	REMOTE = 2	// remote chess server
};


/* After each move the engine returns the current game status */
enum game_status {
	WHITE_TURN		= 0,	// White to play and win
	BLACK_TURN		= 1,	// Black to play and win

	WHITE_WINS		= 2,	// White wins by Checkmate
	BLACK_WINS		= 3,	// Black wins by Checkmate
	WHITE_WINS_BY_TIMEOUT	= 4,	// White wins by Timeout
	BLACK_WINS_BY_TIMEOUT	= 5,	// Black wins by Timeout

	DRAW_BY_AGREEMENT 	= 6,	// Game draw by mutual agreement
	DRAW_BY_3FOLD_REP 	= 7,	// Game draw by 3 fold repetition
	DRAW_BY_50_MOVES 	= 8,	// Game draw by 50 moves rule
	DRAW_BY_INSUF_MATERIAL	= 9,	// Draw due to Insufficient material
	DRAW_BY_STALEMATE 	= 10,	// Game draw by stalemate

	GAME_ADJOURNED		= 11,	// Game adjourned by refree
	GAME_ABANDONED  	= 12	// Game abandoned midway
};


/* types of chessmen present on board */
enum chessmen {
	KING = 0,
	QUEEN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	PAWN = 5,
	EP = 6	// en-passant capture mapping
};


/* pieces present on each square of board */
enum pieces {
	EMPTY_SQR = 0,
	BLACK_ROOK = 1,
	BLACK_KNIGHT = 2,
	BLACK_BISHOP = 3,
	BLACK_QUEEN = 4,
	BLACK_KING = 5,
	BLACK_PAWN = 6,
	WHITE_ROOK = 7,
	WHITE_KNIGHT = 8,
	WHITE_BISHOP = 9,
	WHITE_QUEEN = 10,
	WHITE_KING = 11,
	WHITE_PAWN = 12
};


/* King and queen side castling rights */
enum castling_rights {
	WHITE_KS = 0,
	WHITE_QS = 1,
	BLACK_KS = 2,
	BLACK_QS = 3
};


/* File numbers on board */
enum files {
        A_file = 0,
        B_file = 1,
        C_file = 2,
        D_file = 3,
        E_file = 4,
        F_file = 5,
        G_file = 6,
        H_file = 7
};


/* Rank numbers on board */
enum rank {
        RANK_8 = 0,
        RANK_7 = 1,
        RANK_6 = 2,
        RANK_5 = 3,
        RANK_4 = 4,
        RANK_3 = 5,
        RANK_2 = 6,
        RANK_1 = 7
};


/* move struct encodes the piece to be moved
 * from source square to destination square
 * along with other information to make the
 * engine decide whether its a best move or not */
struct move {
	enum pieces piece;	// piece to be moved
        enum chessmen prom;	// type of promoted piece, if any
	char baseSqr;		// source square
	char targetSqr;		// destination square
	int flags;		// promotion, enpassant flags
	int eval;		// move evaluation score
};

/* all current board information necessary to make
 * the next move is encapsulated in board struct.
 * If the number of half moves reaches 50, the game
 * is a draw by 50 move rule */
struct board
{
	char fen[MAX_FEN_LEN];			// FEN representing board
	enum player whitePlayer, blackPlayer;	// player information
	enum game_status status;		// current game status
	enum pieces sqr[8][8];			// pieces on each square
	bool castling[4];			// current castling rights
	char enpassant;				// en-passant square number
	unsigned int halfmove;			// number of half moves
	unsigned int fullmove;			// number of full moves
};

/* Function prototypes */
int print_fen_str(struct board *board);
bool init_board(char *fen, struct board *board, enum player w, enum player b);
void print_board(struct board *board);
void print_board_status(struct board *board);

#endif	/* __CHESS_H__ */

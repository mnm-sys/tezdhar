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
#define dbg_print(fmt, ...) do {} while (0)
#endif

/* type of chess player */
enum player {
	AI	= 0,	// this chess engine
	HUMAN	= 1,	// human player
	REMOTE	= 2	// remote chess server
};


/* After each move the engine returns the current game status */
enum game_status {
	/* game in progress - player turn */
	WHITE_TURN,			// White to play and win
	BLACK_TURN,			// Black to play and win
	WHITE_UNDER_CHECK,		// White to move out of check
	BLACK_UNDER_CHECK,		// Black to move out of check
	WHITE_CAN_ACCEPT_DRAW,		// White can accept or decline draw
	BLACK_CAN_ACCEPT_DRAW,		// Black can accept or decline draw

	/* game over - player wins */
	WHITE_WINS_BY_CHECKMATE,	// White wins by Checkmate
	BLACK_WINS_BY_CHECKMATE,	// Black wins by Checkmate
	WHITE_WINS_BY_TIMEOUT,		// White wins by Timeout
	BLACK_WINS_BY_TIMEOUT,		// Black wins by Timeout

	/* game over - result draw */
	DRAW_BY_AGREEMENT, 		// Game draw by mutual agreement
	DRAW_BY_3FOLD_REP, 		// Game draw by 3 fold repetition
	DRAW_BY_5FOLD_REP, 		// Game draw by 5 fold repetition
	DRAW_BY_50_MOVES_RULE, 		// Game draw by 50 moves rule
	DRAW_BY_75_MOVES_RULE, 		// Game draw by 75 moves rule
	DRAW_BY_INSUF_MATERIAL,		// Draw due to insufficient material
	DRAW_BY_STALEMATE, 		// Game draw by stalemate

	/* game terminated - various reasons */
	GAME_ABANDONED,  		// game abandoned midway
	GAME_ADJOURNED,			// postponed and to be resumed later
	GAME_ADJUDICATED,		// decided by third party adjudication
	GAME_PLAYER_DEAD,		// one or both players are dead
	GAME_EMERGENCY,			// game terminaed due to emergency
	GAME_RULES_INFRACTION,		// terminated due to rules infraction
	GAME_TIME_FORFEIT,		// terminated due to time forfeit
	GAME_UNTERMINATED		// game result unknown
};


/* types of chessmen present on board */
enum chessmen {
	KING	= 0,
	QUEEN	= 1,
	KNIGHT	= 2,
	BISHOP	= 3,
	ROOK	= 4,
	PAWN	= 5,
	EMPTY	= 6
};


/* pieces present on each square of board */
enum pieces {
	EMPTY_SQR	= 0,
	BLACK_ROOK	= 1,
	BLACK_KNIGHT	= 2,
	BLACK_BISHOP	= 3,
	BLACK_QUEEN	= 4,
	BLACK_KING	= 5,
	BLACK_PAWN	= 6,
	WHITE_ROOK	= 7,
	WHITE_KNIGHT	= 8,
	WHITE_BISHOP	= 9,
	WHITE_QUEEN	= 10,
	WHITE_KING	= 11,
	WHITE_PAWN	= 12
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
        A_FILE	= 0,
        B_FILE	= 1,
        C_FILE	= 2,
        D_FILE	= 3,
        E_FILE	= 4,
        F_FILE	= 5,
        G_FILE	= 6,
        H_FILE	= 7
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
void print_fen_str(struct board *board);
bool init_board(char *fen, struct board *board, enum player w, enum player b);
void print_board(struct board *board);
void print_board_status(struct board *board);
void parse_san_input(char *input);

#endif	/* __CHESS_H__ */

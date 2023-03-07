/* @file:	tezdhar/src/chess.h
 * @url:	https://github.com/mnm-sys/tezdhar/blob/main/src/chess.h
 * @author:	Manavendra Nath Manav (mnm.kernel@gmail.com)
 * @created:	Oct. 2022
 * @license:	GPLv3
 * @desc:	This header file contains various constants and initial values
 * 		required by the chess engine like FEN string for starting
 * 		position, declarations for bitboards for various pieces, etc.
 */

#ifndef __CHESS_H__
#define __CHESS_H__	1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>	// for uint8_t
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>	// for fprintf
#endif

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#define VERSION "0.1"
#define AUTHOR "Manavendra Nath Manav"
#define URL "https://github.com/mnm-sys/tezdhar.git"

#define MAX_FEN_LEN 88		// 87 plus 1 for Null terminator
#define MAX_INPUT_LEN 128	// max user input length
#define MAX_MOVE_LEN 16		// max move lenght for SAN, UCI or ICCF format

/* Initial Forsyth–Edwards Notation (FEN) of a chess game */
#define INITIAL_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TRICKY_POS "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

/* Macro definition for debug print */
#ifdef DEBUG
#define debug_print(fmt, ...) do { \
	fprintf(stderr, "[%s:%d:%s()] -------> " fmt "%s", \
			__FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)
#define dbg_print(...) debug_print(__VA_ARGS__, "")
#else
#define debug_print(fmt, ...) while(0) { \
	fprintf(stderr, "[%s:%d:%s()] " fmt "%s", \
			__FILE__, __LINE__, __func__, __VA_ARGS__); }
#define dbg_print(...) debug_print(__VA_ARGS__, "")
#endif


/* typedef for boolean variables */
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
#ifndef HAVE__BOOL
#ifdef __cplusplus
typedef bool _Bool;
#else
#define _Bool signed char
#endif
#endif
#define bool _Bool
#define false 0
#define true 1
#define __bool_true_false_are_defined 1
#endif


/* typedef for unsigned 64-bit bitboards */
#ifdef HAVE_STDINT_H
typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8;
#elif defined HAVE_UNSIGNED_LONG_LONG_INT
typedef unsigned long long U64;
typedef unsigned int	   U32;
typedef short unsigned int U16;
typedef unsigned char	   U8;
#endif

/* color of chess pieces */
enum color {
	WHITE	= 0,
	BLACK	= 1
};


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
	A_FILE		= 0,
	B_FILE		= 1,
	C_FILE		= 2,
	D_FILE		= 3,
	E_FILE		= 4,
	F_FILE		= 5,
	G_FILE		= 6,
	H_FILE		= 7,
	MAX_FILE	= 8
};


/* Rank numbers on board */
enum rank {
	RANK_1		= 0,
	RANK_2		= 1,
	RANK_3		= 2,
	RANK_4		= 3,
	RANK_5		= 4,
	RANK_6		= 5,
	RANK_7		= 6,
	RANK_8		= 7,
	RANK_MAX	= 8
};



/* Chess board's rank-file based square mapping to bitboard's bit index.
 * Bit 0 starts from A1 square, with H1 square being bit 7, and goes upto
 * H8 square which is bit 63 on the bitboard representation */
enum square {
        A1, B1, C1, D1, E1, F1, G1, H1, //  0 ..  7
        A2, B2, C2, D2, E2, F2, G2, H2, //  8 .. 15
        A3, B3, C3, D3, E3, F3, G3, H3, // 16 .. 23
        A4, B4, C4, D4, E4, F4, G4, H4, // 24 .. 31
        A5, B5, C5, D5, E5, F5, G5, H5, // 32 .. 39
        A6, B6, C6, D6, E6, F6, G6, H6, // 40 .. 47
        A7, B7, C7, D7, E7, F7, G7, H7, // 48 .. 55
        A8, B8, C8, D8, E8, F8, G8, H8, // 56 .. 63
        SQ_NB                           // 64
};


/* square to co-ordinates */
static const char sqr_to_coords[64][3] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};


struct move {
	char movetext[MAX_MOVE_LEN];	// move text in SAN, UCI or ICCF format
	enum chessmen chessman;		// piece type irrespective of color
	enum chessmen promoted;		// promoted piece type

	int8_t from_file;		// file number of move's from-square
	int8_t from_rank;		// rank number of move's from-square
	int8_t to_file;			// file number of move's to-square
	int8_t to_rank;			// rank number of move's to-square

	bool castle_ks;			// is move king side castling
	bool castle_qs;			// is move queen side castling
	bool null;			// is null move
	bool invalid;			// is valid or invalid move
	bool draw_offered;		// does player offers draw in this move
	bool ep;			// move claims to be en-passant move
	bool capture;			// move claims to capture opponent piece
	bool check;			// move claims to give check
	bool checkmate;			// move claims to give checkmate
};


/* A minimum of 12 bitboards are required to fully represent a chess board */
struct bitboards {
	uint64_t wKing, bKing;
	uint64_t wQueen, bQueen;
	uint64_t wBishop, bBishop;
	uint64_t wKnight, bKnight;
	uint64_t wRook, bRook;
	uint64_t wPawn, bPawn;
};


/* All current board information necessary to make the next move is
 * encapsulated in board struct. If the number of half moves reaches 50,
 * the game is a draw by 50 moves rule. The members of the struct are
 * arranged in decreasing order of their size */
struct board
{
	enum pieces sqr[8][8];		// pieces on each square
	struct bitboards bb;		// struct containing 12 bitboards
	char fen[MAX_FEN_LEN];		// FEN representing board
	enum player whitePlayer;	// white player information
	enum player blackPlayer;	// black player information
	enum game_status status;	// current game status
	enum color turn;		// which side turn to move
	bool castling[4];		// current castling rights
	uint16_t halfMoves;		// number of half moves
	uint16_t fullMoves;		// number of full moves
	int8_t enpassant;		// en-passant square number
};


/* Function prototypes */
void print_fen_str(struct board *brd);
bool init_board(char *fen, struct board *brd, enum player w, enum player b);
void print_board(struct board *brd);
void print_board_struct_info(struct board *brd);
void parse_san_input(char *input);
char *input_user_move(char * const buf, const struct board * const brd);
struct move parse_input_move(char * const movetext);
void print_move_struct_info(const char *f, int l, const char *func, struct move *m);
bool parse_fen_record(char *fen, struct board *brd);
void clear_castling_rights(struct board *brd);
int count_bits(const uint64_t bb);
void setup_move_struct(const char * const movetext, struct move *move);
bool update_bitboards(struct board * const brd);
void print_bitboard(const uint64_t bb);
void dbg_print_all_bitboards(const struct bitboards * const bb);
uint64_t bishop_attacks_on_the_fly(const enum square sq, const uint64_t blockers);
uint64_t get_bishop_attacks(const enum square sq, uint64_t occu);
uint64_t rook_attacks_on_the_fly(const enum square sq, const uint64_t blockers);
void init_random_seed(void);
uint64_t find_magic_number(const enum chessmen piece, const enum square sq, const uint64_t attack_mask, const int relv_bits);
uint64_t set_occupancy(const int index, const int bits_in_mask, uint64_t attack_mask);
void init_pawn_attacks(void);
void init_knight_attacks(void);
void init_king_attacks(void);
void init_magic_numbers(void);
bool init_bishop_magic(const bool);
void init_bishop_attacks(void);
bool init_rook_magic(const bool);
void init_rook_attacks(void);
void init_leaper_attacks(void);
void init_slider_attacks(void);


#endif	/* __CHESS_H__ */


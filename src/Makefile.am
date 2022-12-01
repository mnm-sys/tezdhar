# program name
bin_PROGRAMS = tezdhar

# specify which source files get built into an executable
tezdhar_SOURCES = bishop.c	\
		  bitboard.h	\
		  bitboard.c	\
		  board.c	\
		  chess.h	\
		  chess.c	\
		  king.c	\
		  knight.c	\
		  parse.c	\
		  pawn.c	\
		  rook.c	\
		  ui.c

tezdhar_CFLAGS =	-fdata-sections			\
			-fdelete-null-pointer-checks	\
			-fexceptions			\
			-ffunction-sections		\
			-fno-common			\
			-fno-omit-frame-pointer		\
			-fPIE				\
			-fsanitize=address		\
			-fsanitize-recover=address	\
			-fsanitize-address-use-after-scope	\
			-fsanitize=undefined		\
			-fstack-protector-all		\
			-fstack-usage 			\
			-pie				\
			-Wall 				\
			-Walloca			\
			-Wattributes			\
			-Wbad-function-cast		\
			-Wbuiltin-macro-redefined	\
			-Wcast-qual 			\
			-Wchar-subscripts		\
			-Wconversion			\
			-Wcpp				\
			-Wdangling-else			\
			-Wdate-time			\
			-Wdeclaration-after-statement	\
			-Wdeprecated			\
			-Wdeprecated-declarations	\
			-Wdisabled-optimization		\
			-Wdiv-by-zero			\
			-Wdouble-promotion 		\
			-Wendif-labels			\
			-Werror=implicit		\
			-Wextra 			\
			-Wfloat-equal			\
			-Wformat-extra-args		\
			-Wformat-zero-length		\
			-Wformat=2 			\
			-Wignored-qualifiers		\
			-Wincompatible-pointer-types	\
			-Winit-self 			\
			-Winline			\
			-Wint-conversion		\
			-Wint-in-bool-context		\
			-Wint-to-pointer-cast		\
			-Winvalid-pch			\
			-Wmain				\
			-Wmemset-transposed-args	\
			-Wmissing-declarations		\
			-Wmissing-include-dirs		\
			-Wmissing-prototypes		\
			-Wmultichar			\
			-Wnarrowing			\
			-Wnested-externs		\
			-Wnull-dereference		\
			-Wodr				\
			-Wold-style-definition		\
			-Woverflow			\
			-Wpacked			\
			-Wparentheses			\
			-Wpedantic 			\
			-Wpointer-to-int-cast		\
			-Wpragmas			\
			-Wredundant-decls 		\
			-Wreturn-type			\
			-Wsequence-point		\
			-Wshadow 			\
			-Wshift-count-negative		\
			-Wshift-count-overflow		\
			-Wsizeof-array-argument		\
			-Wstack-protector		\
			-Wstrict-overflow=5 		\
			-Wstrict-prototypes		\
			-Wswitch-bool			\
			-Wswitch-default 		\
			-Wswitch			\
			-Wundef 			\
			-Wuninitialized			\
			-Wunknown-pragmas		\
			-Wunused-const-variable		\
			-Wunused-macros			\
			-Wvarargs			\
			-Wvla				\
			-Wwrite-strings

#removed CFLAGS: -v -Wpadded
#-fsanitize=hwaddress
#-fsanitize=memory
#-fsanitize-memory-track-origins

#AM_LDFLAGS = --gc-sections --print-gc-sections

ACLOCAL_AMFLAGS = -I ./../build-aux/m4

clean-local:
	-rm -f *.su


# Ensure that a recent enough version of Autoconf is being used. If the
# version of Autoconf being used to create configure is earlier than version,
# print an error message to the standard error output and exit with failure
# (exit status is 63).
AC_PREREQ([2.61])


# AC_INIT (package, version, [bug-report], [tarname], [url])
AC_INIT([Tezdhar Chess Engine],[0.1],[mnm.kernel@gmail.com],[tezdhar],[https://github.com/mnm-sys/tezdhar.git])


AC_CONFIG_MACRO_DIR([build-aux/m4])

# Automake will look for various helper scripts, such as install-sh, in
# the directory named in this macro invocation. (The full list of scripts
# is: ar-lib, config.guess, config.sub, depcomp, compile, install-sh,
# ltmain.sh, mdate-sh, missing, mkinstalldirs, py-compile, test-driver,
# texinfo.tex, ylwrap.) Not all scripts are always searched for; some
# scripts will only be sought if the generated Makefile.in requires them.
#
# If AC_CONFIG_AUX_DIR is used, it must be given before the call to
# AM_INIT_AUTOMAKE; Automake will warn about this if it is not so. All
# other AC_CONFIG_... macros are conventionally called after AM_INIT_AUTOMAKE,
# though they may or may not work in other locations, with or without warnings.
# If AC_CONFIG_AUX_DIR is not given, the scripts are looked for in their
# standard locations. 
AC_CONFIG_AUX_DIR([build-aux])

# The macros to create a Makefile is AM_INIT_AUTOMAKE. -Wall and -Werror
# ask automake to turn on all warnings and report them as errors. The
# foreign option tells Automake that this package will not follow the GNU
# Standards. GNU packages should always distribute additional files such
# as ChangeLog, AUTHORS, etc. We do not want automake to complain about
# these missing files in our codebase.
AM_INIT_AUTOMAKE([-Wall foreign])


# Add a macro to account for the compiler your project needs. If your
# project is written in C++, the appropriate macro is AC_PROG_CXX,
# while a project written in C requires AC_PROG_CC, and so on.
AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS

# checks for programs
#AC_PROG_CXX
AM_PROG_AR
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_LN_S
AC_STRUCT_TM
#AM_PROG_LEX
AC_PROG_MKDIR_P

# checks for libraries
AC_CHECK_LIB([c],[printf])
#ACX_PTHREAD()
#AC_CHECK_LIB([curses],[tgetent])
#AC_CHECK_LIB([ncurses],[tgetent])

# checks for header files
AC_INCLUDES_DEFAULT
#AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_ASSERT
#AC_HEADER_TIME
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS(stdio.h stdlib.h stdint.h stdbool.h string.h strings.h ctype.h)
AC_CHECK_HEADERS(inttypes.h langinfo.h locale.h wchar.h errno.h)
AC_CHECK_HEADERS(time.h sys/time.h unistd.h fcntl.h)

# checks for types
AC_CHECK_SIZEOF([size_t])
AC_CHECK_SIZEOF([time_t])

AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_INTMAX_T
AC_TYPE_UINTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

AC_TYPE_LONG_LONG_INT
AC_TYPE_UNSIGNED_LONG_LONG_INT
AC_TYPE_LONG_DOUBLE
AC_TYPE_LONG_DOUBLE_WIDER

AC_TYPE_MBSTATE_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T

# checks for structures
AC_STRUCT_TM
AC_STRUCT_TIMEZONE

# checks for compiler characteristics
AC_CHECK_SIZEOF([int *])
AC_C_BIGENDIAN
AC_C_CONST
AC_C_RESTRICT
AC_C_VOLATILE
AC_C_INLINE
AC_C_CHAR_UNSIGNED
AC_C_STRINGIZE
AC_C_FLEXIBLE_ARRAY_MEMBER
AC_C_VARARRAYS
AC_C_TYPEOF
AC_C_PROTOTYPES
AC_PROG_GCC_TRADITIONAL

# checks for library functions
#AC_TYPE_SIGNAL
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS(mrand48 rand random arc4random srand srand48 sranddev srandomdev getpid)
AC_CHECK_FUNCS(strdup strndup strpbrk strchr strrchr strspn strcspn strstr strtol strerror)
AC_CHECK_FUNCS(nl_langinfo setlocale ffsll clock time gettimeofday memmove memset bzero)

# checks for system services
AC_PATH_X
AC_PATH_XTRA
AC_SYS_INTERPRETER
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES
AC_SYS_POSIX_TERMIOS


# check for compiler builtins

# You may use __builtin_expect to provide the compiler with branch prediction
# information. In general, you should prefer to use actual profile feedback for
# this (-fprofile-arcs). For the purposes of branch prediction optimizations,
# the probability that a __builtin_expect expression is true is controlled by
# GCC’s builtin-expect-probability parameter, which defaults to 90%. You can
# also use __builtin_expect_with_probability to explicitly assign a probability
# value to individual expressions. If the built-in is used in a loop construct,
# the provided probability will influence the expected number of iterations made
# by loop optimizations.
AX_GCC_BUILTIN([__builtin_expect])

# This function has the same semantics as __builtin_expect, but the caller
# provides the expected probability that exp == c. The last argument,
# probability, is a floating-point value in the range 0.0 to 1.0, inclusive.
# The probability argument must be constant floating-point expression.
AX_GCC_BUILTIN([__builtin_expect_with_probability])

# This function causes the program to exit abnormally. GCC implements this
# function by using a target-dependent mechanism (such as intentionally
# executing an illegal instruction) or by calling abort. The mechanism
# used may vary from release to release so you should not rely on any
# particular implementation.
AX_GCC_BUILTIN([__builtin_trap])

# If control flow reaches the point of the __builtin_unreachable, the program
# is undefined. It is useful in situations where the compiler cannot deduce
# the unreachability of the code.
AX_GCC_BUILTIN([__builtin_unreachable])

# This function is used to minimize cache-miss latency by moving data into
# a cache before it is accessed. You can insert calls to __builtin_prefetch
# into code for which you know addresses of data in memory that is likely to
# be accessed soon. If the target supports them, data prefetch instructions
# are generated. If the prefetch is done early enough before the access
# then the data will be in the cache by the time it is accessed.
AX_GCC_BUILTIN([__builtin_prefetch])

# Returns the size of an object pointed to by ptr.
AX_GCC_BUILTIN([__builtin_object_size])

# Returns one plus the index of the least significant 1-bit of x,
# or if x is zero, returns zero.
AX_GCC_BUILTIN([__builtin_ffs])
AX_GCC_BUILTIN([__builtin_ffsl])
AX_GCC_BUILTIN([__builtin_ffsll])

# Returns the number of leading 0-bits in x, starting at the most
# significant bit position. If x is 0, the result is undefined.
AX_GCC_BUILTIN([__builtin_clz])
AX_GCC_BUILTIN([__builtin_clzl])
AX_GCC_BUILTIN([__builtin_ctzll])

# Returns the number of trailing 0-bits in x, starting at the least
# significant bit position. If x is 0, the result is undefined.
AX_GCC_BUILTIN([__builtin_ctz])
AX_GCC_BUILTIN([__builtin_ctzl])
AX_GCC_BUILTIN([__builtin_ctzll])

# Returns the number of leading redundant sign bits in x, i.e. the
# number of bits following the most significant bit that are identical
# to it. There are no special cases for 0 or other values.
AX_GCC_BUILTIN([__builtin_clrsb])
AX_GCC_BUILTIN([__builtin_clrsbl])
AX_GCC_BUILTIN([__builtin_clrsbll])

# Returns the number of 1-bits in x.
AX_GCC_BUILTIN([__builtin_popcount])
AX_GCC_BUILTIN([__builtin_popcountl])
AX_GCC_BUILTIN([__builtin_popcountll])
if test "ax_cv_have___builtin_popcountll" != "xno"; then
	LDFLAGS="$LDFLAGS -mpopcnt"
fi

# Returns the parity of x, i.e. the number of 1-bits in x modulo 2.
AX_GCC_BUILTIN([__builtin_parity])
AX_GCC_BUILTIN([__builtin_parityl])
AX_GCC_BUILTIN([__builtin_parityll])

# Returns the first argument raised to the power of the second. Unlike
# the pow function no guarantees about precision and rounding are made.
AX_GCC_BUILTIN([__builtin_powi])
AX_GCC_BUILTIN([__builtin_powif])
AX_GCC_BUILTIN([__builtin_powil])

# Returns x with the order of the bytes reversed; for example,
# 0xaabb becomes 0xbbaa. Byte here always means exactly 8 bits.
AX_GCC_BUILTIN([__builtin_bswap16])
AX_GCC_BUILTIN([__builtin_bswap32])
AX_GCC_BUILTIN([__builtin_bswap64])
AX_GCC_BUILTIN([__builtin_bswap128])

# On targets where the user visible pointer size is smaller than the size
# of an actual hardware address this function returns the extended user
# pointer. Targets where this is true included ILP32 mode on x86_64 or
# Aarch64. This function is mainly useful when writing inline assembly code.
AX_GCC_BUILTIN([__builtin_extend_pointer])


# The AC_CONFIG_HEADERS([config.h]) invocation causes the configure script
# to create a config.h file gathering ‘#define’s defined by other macros in
# configure.ac. 
AC_CONFIG_HEADERS([config.h])


# Distinguish this package’s source directory from other source directories
# that might happen to exist in the file system. unique-file-in-source-dir
# should name a file that is unique to this package. configure will verify
# that this file exists in srcdir, before it runs any other checks. Use of
# this macro is strongly recommended. It protects against people accidentally
# specifying the wrong directory with --srcdir.
AC_CONFIG_SRCDIR(src/chess.c)

# The AC_CONFIG_FILES macro declares the list of files that configure should
# create from their *.in templates. Automake also scans this list to find the
# Makefile.am files it must process. (This is important to remember: when
# adding a new directory to your project, you should add its Makefile to this
# list, otherwise Automake will never process the new Makefile.am you wrote
# in that directory.)
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])


# The AC_OUTPUT line is a closing command that actually produces the part of
# the script in charge of creating the files registered with AC_CONFIG_HEADERS
# and AC_CONFIG_FILES. This macro generates config.status and launches it. Call
# this macro once, at the end of configure.ac.
AC_OUTPUT


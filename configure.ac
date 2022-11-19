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
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_LN_S

# checks for libraries
AC_CHECK_LIB([c],[printf])
AC_CHECK_LIB([curses],[tgetent])
AC_CHECK_LIB([ncurses],[tgetent])


# checks for header files
AC_INCLUDES_DEFAULT
AC_CHECK_HEADERS([langinfo.h])
AC_CHECK_HEADER_STDBOOL
AC_HEADER_STDBOOL

# checks for types
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_LONG_DOUBLE
AC_TYPE_LONG_DOUBLE_WIDER
AC_TYPE_LONG_LONG_INT
AC_TYPE_MBSTATE_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_TYPE_UINTPTR_T
AC_TYPE_UNSIGNED_LONG_LONG_INT


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
AC_CHECK_FUNCS([nl_langinfo])
AC_CHECK_FUNCS([setlocale])

# checks for system services
AC_PATH_X
AC_PATH_XTRA
AC_SYS_INTERPRETER
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES
AC_SYS_POSIX_TERMIOS

# check for compiler builtins
AX_GCC_BUILTIN([__builtin_popcountll])

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



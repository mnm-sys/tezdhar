# **Tezdhar**
A bitboard based educational chess engine written in C with code commentary [ -- currently under development ]

```
FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

  ┏━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┓
8 ┃ ♜ ┃ ♞ ┃ ♝ ┃ ♛ ┃ ♚ ┃ ♝ ┃ ♞ ┃ ♜ ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
7 ┃ ♟ ┃ ♟ ┃ ♟ ┃ ♟ ┃ ♟ ┃ ♟ ┃ ♟ ┃ ♟ ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
6 ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
5 ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
4 ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
3 ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃   ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
2 ┃ ♙ ┃ ♙ ┃ ♙ ┃ ♙ ┃ ♙ ┃ ♙ ┃ ♙ ┃ ♙ ┃
  ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫
1 ┃ ♖ ┃ ♘ ┃ ♗ ┃ ♕ ┃ ♔ ┃ ♗ ┃ ♘ ┃ ♖ ┃
  ┗━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┛
    a   b   c   d   e   f   g   h

Your move (White): 
```

## Build and Installation Instructions
```
$ git clone https://github.com/mnm-sys/tezdhar.git
$ cd tezdhar
$ ./configure
$ make
$ make install
$ tezdhar
```
If you don't want to install, you can run it locally after `make`
```
$ ./src/tezdhar
```
To make debug build for development, use
```
$ ./configure CFLAGS="-g3 -DDEBUG"
$ make
```
To make tags and cscope for code navigation, use
```
$ make ctags
$ make cscope
$ cscope
```
## Features Implemented till Now:
- [x] Board display with UTF-8 pieces
- [x] SAN Parser
- [ ] UCI and ICCF Notation Parser
- [ ] PGN Reader and Parser
- [ ] Move Validation w.r.t. board position
- [ ] Draw Detection
- [ ] Move Generation
- [ ] Move Evaluation
- [ ] Best Move Search
- [ ] ... TBD ...
```
 *
 *                                  ^
 *                             Rank |
 *                                  -  Little-endian Rank-File Mapping
 *                                  |  for Bitboard Square Layout
 *                                  -  Sqr bitIndex = rank * 8 + file
 *                                  |
 *                                  -
 *                                  |
 *                                8 -  a8  b8  c8  d8  e8  f8   .  h8
 *                                  |
 *                                7 -  a7   .  c7   .   .  f7   .  h7
 *                                  |
 *                                6 -   .  b6   .   .   .  f6  g6   .
 *                                  |
 *                                5 -   .   .   .  d5  e5   .  g5   .
 *                                  |
 *                                4 -   .   .   .  d4  e4   .   .   .
 *                                  |
 *                                3 -   .   .  c3   .   .  f3   .   . 
 *                                  |
 *                                2 -  a2  b2   .   .   .   .  g2  h2
 *                                  |
 *                                1 -  a1  b1   .  d1  e1   .  g1  h1
 *                                  |
 *  <---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|-->
 *                                  |   1   2   3   4   5   6   7   8       File
 *                                  -   a   b   c   d   e   f   g   h
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  -
 *                                  |
 *                                  v
 *
```
## Contributors and Collaborators are Welcome
- [x] Issues: Open, Fix, Resolve, Close, Discuss
- [x] Code: Develop, Test, Enhance, Review, Fix bugs
- [x] Unit Testing: Write Test cases, Multi-platform testing
- [x] AI: New Neural Network engine with ML
- [x] Documentation: Wiki Pages, Code Commentary, API Manuals

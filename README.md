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

## Contributors and Collaborators are Welcome
- [x] Issues: Open, Fix, Resolve, Close, Discuss
- [x] Code: Develop, Test, Enhance, Review, Fix bugs
- [x] Unit Testing: Write Test cases, Multi-platform testing
- [x] AI: New Neural Network engine with ML
- [x] Documentation: Wiki Pages, Code Commentary, API Manuals

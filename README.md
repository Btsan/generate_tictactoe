# generate_tictactoe
Tic-Tac-Toe board state generator (along with some heuristic calculations)

###

Recursively generates all 5478 legal game states for tic-tac-toe.

e.g. let X be 1 and O be 0

     then the board:
     
     1 | 0 | 0
     --+---+--
     1 | 1 | 0
     --+---+--
     1 | 1 | 1
     
     is represented as:
     
     1 0 0 1 1 0 1 1 1
     
## Heuristics

The provided C++ code has <strong>commented out</strong> heuristic calculation code, used to augment the generated data for use in machine learning programs.

## Tool(s)

Just C++ 11 and the g++ compiler.

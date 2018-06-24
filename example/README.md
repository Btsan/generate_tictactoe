## Example

Usage of C++ generated data for implementing AI Tic-Tac-Toe agent.

---

### Data

5478 unique Tic-Tac-Toe game states, represented as the raw board state (9 dimensions) and heuristic components (5 dimensions) for a total of 14 dimensions.

Heuristics:

1) \# pieces placed by 1

2) \# pieces placed by -1

3) \# immediately winning moves by 1

4) \# immediately winning moves by -1

5) winner (-1, 0, or 1)

e.g.

     then the board:
     
      1 |  0 |  0
     ---+----+---
     -1 | -1 |  0
     ---+----+---
      1 |  1 | -1
     
     is represented as:
     
     1 0 0 -1 -1 0 1 1 -1 3 3 0 1 0

### Weakly Supervised Learning

An multi-layer perceptron neural network model (MLP) is trained from weak labels, calculated from the generated data heuristics.

1) A shallow MLP, the generative model, is trained to target the weak label, given only the heuristic components.

2) The errors of the generative model are used to calculate a confidence score for each weak label.

3) A deeper MLP, the discriminative model, is trained to target the weak labels, given the labels' confidence score.

### Tools

Models were implemented with [Keras](keras.io), the deep learning framework for Python.

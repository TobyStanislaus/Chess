# Chess Engine

A C++ chess engine combining classical search algorithms with an
AlphaZero-style neural network.



## Installation

### Windows

Download the latest Windows installer from
[GitHub Releases](https://github.com/TobyStanislaus/Chess/releases/tag/v1.1).

The installer contains the application and required runtime
dependencies; no development environment is required.


## Features
- Interactive chess application built with C++ and SFML
- Human vs Human and Human vs AI gameplay
- Minimax Bot with alpha-beta pruning
- Neural-network-guided Monte Carlo Tree Search Bot
- Random Bot for baseline comparison
- Object-oriented piece architecture using inheritance and polymorphism
- Legal move generation including castling, en passant and promotion
- AlphaZero-style neural-network training pipeline
- Self-play generation of 100k+ training positions
- ONNX Runtime integration for neural-network inference

## Technologies

- C++
- SFML
- PyTorch
- ONNX Runtime
- CMake
- MSVC

## Overview

This project is a chess engine written in C++ with multiple AI
implementations. The project also includes a Python machine-learning
pipeline for training an AlphaZero-style neural network through
self-play.

## AI

### Minimax

The Minimax Bot recursively searches possible moves, alternating between maximising and minimising the evaluation of resulting positions. Positions are currently evaluated using a material-based scoring system, assigning values of 100 to pawns, 900 to queens and 20,000 to kings, with corresponding values for the other pieces.

At each depth, the bot assumes that both players will choose the move most favourable to themselves. Alpha-beta pruning eliminates branches that cannot affect the final decision, reducing the number of positions that need to be evaluated.

This bot can be greatly improved with rewarding controlling the middle of the board, yet I find it sufficient to beat rookie chess players (such as myself and my friends).

### Monte Carlo Tree Search

The MCTS Bot uses a neural network to guide both the exploration and evaluation of the search tree. At each expanded position, the network provides a policy over possible moves and a value estimate for the position. Illegal moves are masked from the policy and the remaining probabilities are renormalised to form the prior probabilities used by the search.

Nodes are selected using a PUCT-based score, balancing the estimated value of a position with exploration of moves favoured by the network. Each simulation expands an unexplored position, evaluates it with the neural network and backpropagates the resulting value through the search path.

The bot performs 200 simulations per move and selects the move with the highest visit count. Dirichlet noise is also added to the root node to encourage exploration during self-play.

### AlphaZero-style Neural Network

The project uses a scaled-down AlphaZero-style policy/value network implemented in PyTorch. Board positions are encoded using 17 input planes and passed through a shared convolutional trunk containing four residual blocks.

The network has two output heads. The policy head produces 4,672 move logits corresponding to the 8×8×73 move encoding, while the value head produces a scalar evaluation in the range [-1, 1] representing the position's value for the side to move.

The policy logits are subsequently masked to legal moves and converted into probabilities for use as priors during MCTS. The value output is used to evaluate positions during tree search.

## Machine Learning Pipeline

Training data is generated through self-play using the MCTS Bot. Each position is stored alongside the MCTS policy distribution and the eventual game outcome, providing training targets for both the policy and value heads of the network.

The training process loads the previous network version when available and trains the model using mini-batches of 256 positions for 10 epochs. The loss combines a policy loss based on cross-entropy with the MCTS policy distribution and mean-squared error between the predicted and actual game outcome.

After training, the updated PyTorch model is saved as a new version and exported to ONNX for inference within the C++ chess engine.

## Architecture

The C++ application is organised around a Game controller and Board representation. Game manages the current game state, turn handling, player selection and user interaction, while Board manages the chess position and legal move generation.

Piece provides a common interface for chess pieces, with Pawn, Knight, Bishop, Rook, Queen and King overriding movement and type behaviour through virtual functions. This allows the board to interact with different piece types through base-class pointers while retaining piece-specific behaviour through polymorphism.

The class Move stores the state required to execute and fully undo special moves, including castling, en passant, promotion and captured-piece state.

Player types include human, random, Minimax and MCTS players. The MCTS player uses NNEvaluator to run the trained neural network through ONNX Runtime.

## Building

### Requirements

- Windows x64
- Visual Studio 2026 with **Desktop development with C++**
- CMake 3.25+
- vcpkg
- ONNX Runtime (Windows x64)

### Setup

1. Install Visual Studio 2026 with the **Desktop development with C++** workload.

2. Install [vcpkg](https://github.com/microsoft/vcpkg) and set the `VCPKG_ROOT` environment variable to your vcpkg installation directory.

3. Download the **Windows x64 CPU** release of [ONNX Runtime](https://github.com/microsoft/onnxruntime/releases).

4. Clone the repository:
```text
git clone https://github.com/TobyStanislaus/Chess.git
cd Chess
```

5. Extract ONNX Runtime into the project at:

```text
third_party/onnxruntime/
```


6. Configure + Build the Project:
```text
cmake --preset release
cmake --build --preset release
```
The executable will be created in build-release/Debug

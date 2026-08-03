"""
Generates a small batch of self-play games and saves the resulting training
data to disk. Run this to test the pipeline end-to-end and get a feel for
timing before committing to a full-scale run.
"""

import os
import time
import numpy as np
import torch
from pathlib import Path

from network import ChessNet
from find_version import find_current_version
from self_play import play_one_game

NUM_GAMES = 350
NUM_SIMULATIONS = 100   # AlphaZero-scale runs use 400-800; kept low here for a quick test
MAX_MOVES = 200

def main():
    torch.manual_seed(0)
    net = ChessNet()
    ROOT = Path(__file__).resolve().parent.parent
    curr_version = find_current_version()
    MODEL_FILE = ROOT / "data" / f"version{curr_version}" / "network.pt"

    if os.path.exists(MODEL_FILE):
        print("Loading previous network...")
        net.load_state_dict(torch.load(MODEL_FILE, map_location="cpu"))

    net.eval()

    all_boards = []
    all_policies = []
    all_outcomes = []

    results = []
    game_lengths = []
    game_times = []

    for game_num in range(NUM_GAMES):
        start = time.time()
        examples, result = play_one_game(net, num_simulations=NUM_SIMULATIONS, max_moves=MAX_MOVES)
        elapsed = time.time() - start

        for board_tensor, policy, outcome in examples:
            all_boards.append(board_tensor)
            all_policies.append(policy)
            all_outcomes.append(outcome)

        results.append(result)
        game_lengths.append(len(examples))
        game_times.append(elapsed)

        print(f"Game {game_num + 1}/{NUM_GAMES}: {len(examples)} positions, "
              f"result={result}, took {elapsed:.1f}s")

    all_boards = np.array(all_boards, dtype=np.float32)
    all_policies = np.array(all_policies, dtype=np.float32)
    all_outcomes = np.array(all_outcomes, dtype=np.float32)

    np.savez_compressed(
        ROOT / "data" / f"version{curr_version+1}" / "self_play_data.npz",
        boards=all_boards,
        policies=all_policies,
        outcomes=all_outcomes,
    )

    print()
    print("=" * 50)
    print(f"Total games: {NUM_GAMES}")
    print(f"Total positions recorded: {len(all_boards)}")
    print(f"Results breakdown: {results}")
    print(f"Avg game length: {np.mean(game_lengths):.1f} plies")
    print(f"Avg time per game: {np.mean(game_times):.1f}s "
          f"({np.mean(game_times) / np.mean(game_lengths):.2f}s per move)")
    print(f"Saved to self_play_data.npz")
    print(f"  boards shape: {all_boards.shape}")
    print(f"  policies shape: {all_policies.shape}")
    print(f"  outcomes shape: {all_outcomes.shape}")




if __name__ == "__main__":
    main()
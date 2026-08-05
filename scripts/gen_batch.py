"""
Generates a small batch of self-play games and saves the resulting training
data to disk. Run this to test the pipeline end-to-end and get a feel for
timing before committing to a full-scale run.
"""
from multiprocessing import Pool
import os
import time
import numpy as np
import torch
from pathlib import Path

from network import ChessNet
from find_version import find_current_version
from self_play import play_one_game

NUM_GAMES = 1900
NUM_SIMULATIONS = 150   # AlphaZero-scale runs use 400-800; kept low here for a quick test
MAX_MOVES = 200
NUM_WORKERS = 14

_worker_net = None

def init_worker(state_dict):
    global _worker_net
    torch.set_num_threads(1)
    _worker_net = ChessNet()
    _worker_net.load_state_dict(state_dict)
    _worker_net.eval()

def play_game_worker(game_num):
    start = time.time()
    examples, result = play_one_game(_worker_net, num_simulations=NUM_SIMULATIONS, max_moves=MAX_MOVES)
    elapsed = time.time() - start
    return examples, result, elapsed

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

    state_dict = net.state_dict()
    tasks = [(i, state_dict) for i in range(NUM_GAMES)]

    with Pool(processes=NUM_WORKERS, initializer=init_worker, initargs=(state_dict,)) as pool:
        for game_num, (examples, result, elapsed) in enumerate(pool.imap(play_game_worker, range(NUM_GAMES))):
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

    save_dir = ROOT / "data" / f"version{curr_version+1}"
    save_dir.mkdir(parents=True, exist_ok=True)

    np.savez_compressed(
        save_dir / "self_play_data.npz",
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
import os
import numpy as np
import torch
import torch.nn.functional as F
from torch.utils.data import TensorDataset, DataLoader
import convert_to_cpp
from network import ChessNet
from pathlib import Path
from find_version import find_current_version

# ----------------------------
# Settings
# ----------------------------
ROOT = Path(__file__).resolve().parent.parent

DATA_FILE = ROOT / "data" / f"version{find_current_version()}" / "self_play_data.npz"
OLD_MODEL_FILE = ROOT / "data" / f"version{find_current_version()-1}" / "network.pt"
CURR_MODEL_FILE = ROOT / "data" / f"version{find_current_version()}" / "network.pt"

BATCH_SIZE = 256
EPOCHS = 10
LEARNING_RATE = 1e-3

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


def policy_loss_fn(logits, target_policy):
    """
    AlphaZero policy loss.

    target_policy is a probability distribution produced by MCTS.
    """

    log_probs = F.log_softmax(logits, dim=1)
    return -(target_policy * log_probs).sum(dim=1).mean()


def value_loss_fn(predicted_value, target_value):
    return F.mse_loss(predicted_value.squeeze(1), target_value)


def main():

    print("Loading self-play data...")

    data = np.load(DATA_FILE)

    boards = torch.tensor(data["boards"], dtype=torch.float32)
    policies = torch.tensor(data["policies"], dtype=torch.float32)
    outcomes = torch.tensor(data["outcomes"], dtype=torch.float32)

    print(f"{len(boards)} training positions")

    dataset = TensorDataset(
        boards,
        policies,
        outcomes
    )

    loader = DataLoader(
        dataset,
        batch_size=BATCH_SIZE,
        shuffle=True
    )

    net = ChessNet().to(device)

    if os.path.exists(OLD_MODEL_FILE):
        print("Loading previous network...")
        net.load_state_dict(torch.load(OLD_MODEL_FILE, map_location=device))

    optimizer = torch.optim.Adam(
        net.parameters(),
        lr=LEARNING_RATE
    )

    print("Training...\n")

    for epoch in range(EPOCHS):

        net.train()

        total_loss = 0
        total_policy_loss = 0
        total_value_loss = 0

        for boards, target_policy, target_value in loader:

            boards = boards.to(device)
            target_policy = target_policy.to(device)
            target_value = target_value.to(device)

            optimizer.zero_grad()

            policy_logits, value = net(boards)

            policy_loss = policy_loss_fn(
                policy_logits,
                target_policy
            )

            value_loss = value_loss_fn(
                value,
                target_value
            )

            loss = policy_loss + value_loss

            loss.backward()

            optimizer.step()

            total_loss += loss.item()
            total_policy_loss += policy_loss.item()
            total_value_loss += value_loss.item()

        n = len(loader)

        print(
            f"Epoch {epoch+1}/{EPOCHS} | "
            f"Loss {total_loss/n:.4f} | "
            f"Policy {total_policy_loss/n:.4f} | "
            f"Value {total_value_loss/n:.4f}"
        )

    print("\nSaving network...")

    torch.save(
        net.state_dict(),
        CURR_MODEL_FILE
    )
    print("Exporting to ONNX...")
    convert_to_cpp.convert_to_cpp()
    print("Done!")


if __name__ == "__main__":
    main()
"""
A small policy+value network. Architecture is a scaled-down version of
AlphaZero's: a few conv layers as a shared "trunk", then two heads -
a policy head (probability over all 4672 possible moves) and a value head
(single scalar in [-1, 1] estimating how good the position is for the side
to move).

This is intentionally small (compared to real AlphaZero's ~20 residual
blocks) so it trains fast enough to experiment with on a normal machine.
Widen/deepen later once the pipeline is proven to work end-to-end.
"""

import torch
import torch.nn as nn
import torch.nn.functional as F

from board_encoding import NUM_PLANES
from move_encoding import POLICY_SIZE


class ResidualBlock(nn.Module):
    def __init__(self, channels):
        super().__init__()
        self.conv1 = nn.Conv2d(channels, channels, 3, padding=1)
        self.bn1 = nn.BatchNorm2d(channels)
        self.conv2 = nn.Conv2d(channels, channels, 3, padding=1)
        self.bn2 = nn.BatchNorm2d(channels)

    def forward(self, x):
        residual = x
        out = F.relu(self.bn1(self.conv1(x)))
        out = self.bn2(self.conv2(out))
        return F.relu(out + residual)


class ChessNet(nn.Module):
    def __init__(self, channels=64, num_res_blocks=4):
        super().__init__()
        self.input_conv = nn.Conv2d(NUM_PLANES, channels, 3, padding=1)
        self.input_bn = nn.BatchNorm2d(channels)

        self.res_blocks = nn.ModuleList([ResidualBlock(channels) for _ in range(num_res_blocks)])

        # Policy head
        self.policy_conv = nn.Conv2d(channels, 32, 1)
        self.policy_bn = nn.BatchNorm2d(32)
        self.policy_fc = nn.Linear(32 * 8 * 8, POLICY_SIZE)

        # Value head
        self.value_conv = nn.Conv2d(channels, 8, 1)
        self.value_bn = nn.BatchNorm2d(8)
        self.value_fc1 = nn.Linear(8 * 8 * 8, 64)
        self.value_fc2 = nn.Linear(64, 1)

    def forward(self, x):
        x = F.relu(self.input_bn(self.input_conv(x)))
        for block in self.res_blocks:
            x = block(x)

        policy = F.relu(self.policy_bn(self.policy_conv(x)))
        policy = policy.view(policy.size(0), -1)
        policy_logits = self.policy_fc(policy)  # raw logits - apply softmax/masking outside

        value = F.relu(self.value_bn(self.value_conv(x)))
        value = value.view(value.size(0), -1)
        value = F.relu(self.value_fc1(value))
        value = torch.tanh(self.value_fc2(value))  # squashed to [-1, 1]

        return policy_logits, value

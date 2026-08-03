import chess
import torch
import numpy as np

from board_encoding import encode_board, NUM_PLANES
from move_encoding import move_to_index, index_to_move, POLICY_SIZE
from network import ChessNet


def test_board_encoding_shape():
    board = chess.Board()
    encoded = encode_board(board)
    assert encoded.shape == (NUM_PLANES, 8, 8)
    print("board encoding shape OK:", encoded.shape)


def test_move_roundtrip_on_position(board, label):
    failures = []
    for move in board.legal_moves:
        idx = move_to_index(move)
        decoded = index_to_move(idx, board)
        if decoded != move:
            failures.append((move, decoded, idx))

    if failures:
        print(f"[{label}] {len(failures)} / {board.legal_moves.count()} moves FAILED to round-trip:")
        for original, decoded, idx in failures[:10]:
            print(f"    original={original} decoded={decoded} idx={idx}")
    else:
        print(f"[{label}] all {board.legal_moves.count()} legal moves round-tripped correctly")

    return len(failures) == 0


def test_network_forward_pass():
    board = chess.Board()
    encoded = encode_board(board)
    tensor = torch.from_numpy(encoded).unsqueeze(0)  # add batch dimension -> (1, 17, 8, 8)

    net = ChessNet()
    net.eval()
    with torch.no_grad():
        policy_logits, value = net(tensor)

    assert policy_logits.shape == (1, POLICY_SIZE)
    assert value.shape == (1, 1)
    print("network forward pass OK: policy shape", policy_logits.shape, "value", value.item())


if __name__ == "__main__":
    test_board_encoding_shape()
    print()

    all_ok = True

    all_ok &= test_move_roundtrip_on_position(chess.Board(), "starting position")

    # A position with captures, checks, and castling rights still available
    midgame = chess.Board("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4")
    all_ok &= test_move_roundtrip_on_position(midgame, "midgame position")

    # A position with pawns one move from promotion (tests underpromotion + queen promotion planes)
    promotion_test = chess.Board("4k3/P7/8/8/8/8/7p/4K3 w - - 0 1")
    all_ok &= test_move_roundtrip_on_position(promotion_test, "promotion position")

    print()
    test_network_forward_pass()

    print()
    print("ALL TESTS PASSED" if all_ok else "SOME TESTS FAILED - see above")

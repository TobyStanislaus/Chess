import torch
from pathlib import Path
from network import ChessNet
from find_version import find_current_version

def convert_to_cpp():
    ROOT = Path(__file__).resolve().parent.parent
    version = find_current_version()

    model = ChessNet()
    model.load_state_dict(torch.load(ROOT / "data" / f"version{str(version)}" / "network.pt", map_location="cpu"))
    model.eval()

    dummy_input = torch.randn(1, 17, 8, 8)

    torch.onnx.export(
        model,
        dummy_input,
        str(ROOT / "assets" / "network.onnx"),
        input_names=["board_input"],
        output_names=["policy", "value"],
        dynamic_axes={
            "board_input": {0: "batch_size"},
            "policy": {0: "batch_size"},
            "value": {0: "batch_size"},
        },
        opset_version=17,
        dynamo=False,
    )
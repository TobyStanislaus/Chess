import torch
from network import ChessNet

model = ChessNet()
model.load_state_dict(torch.load("network.pt", map_location="cpu"))
model.eval()

dummy_input = torch.randn(1, 17, 8, 8)  # your 17-plane board encoding

torch.onnx.export(
    model,
    dummy_input,
    "network.onnx",
    input_names=["board_input"],
    output_names=["policy", "value"],
    dynamic_axes={
        "board_input": {0: "batch_size"},
        "policy": {0: "batch_size"},
        "value": {0: "batch_size"},
    },
    opset_version=17,
)
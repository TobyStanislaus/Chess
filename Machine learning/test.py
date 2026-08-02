import onnxruntime as ort
import numpy as np

sess = ort.InferenceSession("network.onnx")

dummy_input = np.random.randn(1, 17, 8, 8).astype(np.float32)  # dtype matters, see below
out = sess.run(None, {"board_input": dummy_input})
print(out[0].shape, out[1].shape)  # expect (1, 4672) and (1, 1)
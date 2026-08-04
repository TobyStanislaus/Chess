#include "NNEvaluator.hpp"
#include "NNEncoder.hpp"
#include "Board.hpp" // safe here - no cycle, this is a .cpp not a header

#include <locale>
#include <codecvt>

// convert narrow string -> wide string for Windows ONNX Runtime API
#include <windows.h>

std::wstring toWideString(const std::string& s)
{
    if (s.empty())
        return {};

    int size_needed = MultiByteToWideChar(
        CP_UTF8,
        0,
        s.data(),
        static_cast<int>(s.size()),
        nullptr,
        0
    );

    std::wstring result(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        s.data(),
        static_cast<int>(s.size()),
        result.data(),
        size_needed
    );

    return result;
}

NNEvaluator::NNEvaluator(const std::string& modelPath)
    : env(ORT_LOGGING_LEVEL_WARNING, "chess_nn"),
      session(env, toWideString(modelPath).c_str(), Ort::SessionOptions{nullptr}) {}

NNOutput NNEvaluator::run(Board& board, bool blackTurn) {
    float input[TENSOR_SIZE];
    encodeBoard(board, blackTurn, input);

    std::array<int64_t, 4> shape{1, NUM_PLANES, BOARD_SIZE, BOARD_SIZE};
    Ort::MemoryInfo memInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memInfo, input, TENSOR_SIZE, shape.data(), shape.size());

    const char* inputNames[]  = {"board_input"};
    const char* outputNames[] = {"policy", "value"};

    auto outputs = session.Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1,
                                outputNames, 2);

    NNOutput out;
    std::memcpy(out.policyLogits.data(), outputs[0].GetTensorData<float>(),
                POLICY_SIZE * sizeof(float));
    out.value = outputs[1].GetTensorData<float>()[0];
    return out;
}
// NNEvaluator.hpp
#pragma once
#include <onnxruntime_cxx_api.h>
#include <array>
#include <string>
#include "PolicyEncoder.hpp" // for POLICY_SIZE

class Board; // forward declaration - we only need Board& here, not the full class

struct NNOutput {
    std::array<float, POLICY_SIZE> policyLogits;
    float value;
};

class NNEvaluator {
public:
    NNEvaluator(const std::string& modelPath);
    NNOutput run(Board& board, bool blackTurn);

private:
    Ort::Env env;
    Ort::Session session;
};
#pragma once
#include "Board.hpp"
#include "NNEvaluator.hpp"
#include <memory>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

constexpr float C_PUCT = 1.5f;

struct MCTSNode {
    float prior = 0.0f;
    int visitCount = 0;
    float valueSum = 0.0f;
    std::vector<std::pair<Move, std::unique_ptr<MCTSNode>>> children;

    float value() const { return visitCount == 0 ? 0.0f : valueSum / visitCount; }
    bool expanded() const { return !children.empty(); }
};

inline float ucbScore(const MCTSNode& parent, const MCTSNode& child) {
    float u = C_PUCT * child.prior * std::sqrt((float)parent.visitCount) / (1 + child.visitCount);
    float q = -child.value(); // child's value is from the opponent's perspective
    return q + u;
}

// Expands `node` with one child per legal move, using the network's policy as
// priors (masked + renormalized over legal moves only - mirrors mcts.py's _evaluate).
// Returns the value estimate for this position, from the mover's perspective.
inline float evaluateNode(MCTSNode& node, Board& board, bool blackTurn, NNEvaluator& nn) {
    std::vector<Move> legalMoves = board.getAllLegalMoves(blackTurn);

    if (legalMoves.empty()) {
        return board.checkIfImInCheck(blackTurn) ? -1.0f : 0.0f;
    }

    NNOutput out = nn.run(board, blackTurn);

    std::vector<float> logits(legalMoves.size());
    for (size_t i = 0; i < legalMoves.size(); i++) {
        int idx = moveToIndex(legalMoves[i]);
        logits[i] = out.policyLogits[idx];
    }
    float maxLogit = *std::max_element(logits.begin(), logits.end());
    float sumExp = 0.0f;
    std::vector<float> priors(legalMoves.size());
    for (size_t i = 0; i < legalMoves.size(); i++) {
        priors[i] = std::exp(logits[i] - maxLogit);
        sumExp += priors[i];
    }
    for (auto& p : priors) p /= sumExp;

    for (size_t i = 0; i < legalMoves.size(); i++) {
        auto child = std::make_unique<MCTSNode>();
        child->prior = priors[i];
        node.children.emplace_back(legalMoves[i], std::move(child));
    }

    return out.value;
}

inline void addRootNoise(MCTSNode& root, float alpha = 0.3f, float frac = 0.25f) {
    static std::mt19937 gen(std::random_device{}());
    std::gamma_distribution<float> gammaDist(alpha, 1.0f);

    std::vector<float> noise(root.children.size());
    float sum = 0.0f;
    for (auto& n : noise) { n = gammaDist(gen); sum += n; }
    for (auto& n : noise) n /= sum;

    for (size_t i = 0; i < root.children.size(); i++)
        root.children[i].second->prior =
            root.children[i].second->prior * (1 - frac) + noise[i] * frac;
}

// Mutates `board` via makeMove/undoMove during search (like your existing
// minimax), restoring it fully before returning.
inline MCTSNode runMCTS(Board& board, bool blackTurn, NNEvaluator& nn,
                         int numSimulations, bool addNoise) {
    MCTSNode root;
    root.prior = 1.0f;
    evaluateNode(root, board, blackTurn, nn);

    if (addNoise && !root.children.empty()) addRootNoise(root);

    for (int sim = 0; sim < numSimulations; sim++) {
        MCTSNode* node = &root;
        bool currentTurn = blackTurn;
        std::vector<MCTSNode*> path{node};
        std::vector<bool> moverStack;

        while (node->expanded()) {
            float bestScore = -1e18f;
            size_t bestIdx = 0;
            for (size_t i = 0; i < node->children.size(); i++) {
                float score = ucbScore(*node, *node->children[i].second);
                if (score > bestScore) { bestScore = score; bestIdx = i; }
            }

            Move& chosen = node->children[bestIdx].first;
            bool mover = currentTurn;
            board.makeMove(chosen, mover);
            moverStack.push_back(mover);

            currentTurn = !currentTurn;
            node = node->children[bestIdx].second.get();
            path.push_back(node);
        }

        float value = evaluateNode(*node, board, currentTurn, nn);

        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            (*it)->valueSum += value;
            (*it)->visitCount += 1;
            value = -value;
        }

        for (int i = (int)moverStack.size() - 1; i >= 0; i--) {
            board.undoMove(moverStack[i]);
        }
    }

    return root;
}

// temperature=0 -> most-visited child (used for real play)
inline Move selectMove(const MCTSNode& root, float temperature = 0.0f) {
    if (temperature <= 0.0f) {
        int bestVisits = -1;
        size_t bestIdx = 0;
        for (size_t i = 0; i < root.children.size(); i++) {
            if (root.children[i].second->visitCount > bestVisits) {
                bestVisits = root.children[i].second->visitCount;
                bestIdx = i;
            }
        }
        return root.children[bestIdx].first;
    }
    // Proportional sampling (self-play use) - add if/when you build the training loop
    return root.children[0].first;
}

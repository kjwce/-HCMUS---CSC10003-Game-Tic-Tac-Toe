#pragma once
#include "move.h"
#include "board.h"
#include <vector>
#include <utility>

class robot {
public:
    // Xử lý chung
    std::pair<int, int> countConsecutive(std::vector<std::vector<int>>& board, int x, int y, int dx, int dy, int player);
    int evaluateBoard(std::vector<std::vector<int>> board, int x, int y, int dx, int dy, int player);
    int evaluatePosition(std::vector<std::vector<int>> board, int x, int y, int player);
    std::vector<Move> generateMoves(std::vector<std::vector<int>>& board);
    Move minimax(std::vector<std::vector<int>>& board, int depth, int alpha, int beta, std::pair<int, int> lastXY, bool isMaximizing);
    bool checkCreateOpenFour(std::vector<std::vector<int>>& board, int x, int y, int player);
    bool checkWin(std::vector<std::vector<int>>& board, int x, int y, int co);
    bool checkOverallWin(std::vector<std::vector<int>>& board);

    // Overload cho BoardAI và BombAI
    Move findBestMove(BoardAI& board, int depth);
    Move findBestMove(BombAI& board, int depth);
    Move botMove(BoardAI& board, int depth);
    Move botMove(BombAI& board, int depth);
    void HowToRobotMove(BoardAI& board, int m);
    void HowToRobotMove(BombAI& board, int m);
};

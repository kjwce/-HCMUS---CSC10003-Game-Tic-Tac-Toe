#include "bot.h"
#include "move.h"
#include "GameEngine.h"
#include <climits>
#include <cmath>
using namespace std;

bool robot::checkWin(vector<vector<int>>& board, int x, int y, int co) {
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (int n = 0; n < 4; n++) {
        int count = 1;
        for (int i = 1; i < 5; i++) {
            int nx = x + i * directions[n][0];
            int ny = y + i * directions[n][1];
            if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && board[nx][ny] == co) count++;
            else break;
        }
        for (int i = 1; i < 5; i++) {
            int nx = x - i * directions[n][0];
            int ny = y - i * directions[n][1];
            if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && board[nx][ny] == co) count++;
            else break;
        }
        if (count >= 5) return true;
    }
    return false;
}

bool robot::checkOverallWin(vector<vector<int>>& board) {
    for (int x = 0; x < 20; x++)
        for (int y = 0; y < 20; y++)
            if (board[x][y] != 0 && checkWin(board, x, y, board[x][y])) return true;
    return false;
}

pair<int, int> robot::countConsecutive(vector<vector<int>>& board, int x, int y, int dx, int dy, int player) {
    int left_count = 0, right_count = 0;
    for (int i = 1; i < 5; i++) {
        int nx = x - i * dx, ny = y - i * dy;
        if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && board[nx][ny] == player) left_count++;
        else break;
    }
    for (int i = 1; i < 5; i++) {
        int nx = x + i * dx, ny = y + i * dy;
        if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && board[nx][ny] == player) right_count++;
        else break;
    }
    return make_pair(left_count, right_count);
}

int robot::evaluateBoard(vector<vector<int>> board, int x, int y, int dx, int dy, int player) {
    pair<int, int> consecutive = countConsecutive(board, x, y, dx, dy, player);
    int total = consecutive.first + consecutive.second + 1;

    int before_nx = x - (consecutive.first + 1) * dx;
    int before_ny = y - (consecutive.first + 1) * dy;
    int after_nx = x + (consecutive.second + 1) * dx;
    int after_ny = y + (consecutive.second + 1) * dy;

    bool canBefore = (before_nx >= 0 && before_nx < 20 && before_ny >= 0 && before_ny < 20 && board[before_nx][before_ny] == 0);
    bool canAfter  = (after_nx >= 0 && after_nx < 20 && after_ny >= 0 && after_ny < 20 && board[after_nx][after_ny] == 0);

    if (total >= 5) return (player == BOT) ? 10000000 : -10000000;
    if (total == 4) return (player == BOT) ? (canBefore || canAfter ? 5000000 : 1000) : (canBefore || canAfter ? -5000000 : -1000);
    if (total == 3) return (player == BOT) ? (canBefore && canAfter ? 100000 : (canBefore || canAfter ? 10000 : 100)) : (canBefore && canAfter ? -100000 : (canBefore || canAfter ? -10000 : -100));
    if (total == 2) return (player == BOT) ? (canBefore && canAfter ? 1000 : (canBefore || canAfter ? 200 : 20)) : (canBefore && canAfter ? -1000 : (canBefore || canAfter ? -200 : -20));
    if (total == 1) return (player == BOT) ? (canBefore && canAfter ? 100 : (canBefore || canAfter ? 20 : 2)) : (canBefore && canAfter ? -100 : (canBefore || canAfter ? -20 : -2));
    return 0;
}

int robot::evaluatePosition(vector<vector<int>> board, int x, int y, int player) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int score = 0;
    for (int i = 0; i < 4; i++) score += evaluateBoard(board, x, y, dirs[i][0], dirs[i][1], player);
    int centerBonus = max(0, 20 - (abs(x - 10) + abs(y - 10)));
    return score + (player == BOT ? centerBonus : -centerBonus);
}

vector<Move> robot::generateMoves(vector<vector<int>>& board) {
    vector<Move> moves;
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            if (board[x][y] != 0) continue;
            bool near = false;
            for (int dx = -2; dx <= 2 && !near; dx++)
                for (int dy = -2; dy <= 2 && !near; dy++) {
                    if (dx==0 && dy==0) continue;
                    int nx = x+dx, ny = y+dy;
                    if (nx>=0 && nx<20 && ny>=0 && ny<20 && board[nx][ny]!=0) near = true;
                }
            if (near) moves.push_back(Move(x, y, 0));
        }
    }
    if (moves.empty()) moves.push_back(Move(10,10,0));
    return moves;
}

Move robot::minimax(vector<vector<int>>& board, int depth, int alpha, int beta, pair<int,int> lastXY, bool maxing) {
    if (depth == 0 || checkOverallWin(board)) {
        if (lastXY.first != -1) {
            int player = board[lastXY.first][lastXY.second];
            int score = evaluatePosition(board, lastXY.first, lastXY.second, player);
            return Move(lastXY.first, lastXY.second, score);
        }
        return Move(-1,-1,0);
    }
    vector<Move> moves = generateMoves(board);
    Move best; best.setScore(maxing ? INT_MIN : INT_MAX);

    for (auto m : moves) {
        auto [x,y] = m.getXY();
        board[x][y] = maxing ? BOT : PLAYER;
        Move cur = minimax(board, depth-1, alpha, beta, {x,y}, !maxing);
        board[x][y] = 0;

        if (maxing) {
            if (cur.getScore() > best.getScore()) best.setMove(x,y,cur.getScore());
            alpha = max(alpha, cur.getScore());
        } else {
            if (cur.getScore() < best.getScore()) best.setMove(x,y,cur.getScore());
            beta = min(beta, cur.getScore());
        }
        if (beta <= alpha) break;
    }
    return best;
}

bool robot::checkCreateOpenFour(vector<vector<int>>& board, int x, int y, int player) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (auto& d : dirs) {
        auto consec = countConsecutive(board, x, y, d[0], d[1], player);
        int total = consec.first + consec.second + 1;
        if (total >= 4) {
            int bx = x - (consec.first+1)*d[0], by = y - (consec.first+1)*d[1];
            int ax = x + (consec.second+1)*d[0], ay = y + (consec.second+1)*d[1];
            bool canB = (bx>=0 && bx<20 && by>=0 && by<20 && board[bx][by]==0);
            bool canA = (ax>=0 && ax<20 && ay>=0 && ay<20 && board[ax][ay]==0);
            if (canB && canA) return true;
        }
    }
    return false;
}

// --- BoardAI ---
Move robot::findBestMove(BoardAI& board, int depth) {
    auto copy = board.getBoard();
    return minimax(copy, depth, INT_MIN, INT_MAX, {-1,-1}, true);
}
Move robot::botMove(BoardAI& board, int depth) {
    auto tmp = board.getBoard();
    auto moves = generateMoves(tmp);
    if (moves.empty()) return Move(10,10,0);
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = BOT;
        if (checkWin(tmp,x,y,BOT)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = PLAYER;
        if (checkWin(tmp,x,y,PLAYER)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = BOT;
        if (checkCreateOpenFour(tmp,x,y,BOT)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = PLAYER;
        if (checkCreateOpenFour(tmp,x,y,PLAYER)) return m;
        tmp[x][y] = 0;
    }
    return findBestMove(board, depth);
}
void robot::HowToRobotMove(BoardAI& board, int m) {
    auto best = botMove(board, 3);
    auto [x,y] = best.getXY();
    board(x,y) = m;
}

// --- BombAI ---
Move robot::findBestMove(BombAI& board, int depth) {
    auto copy = board.getBoard();
    return minimax(copy, depth, INT_MIN, INT_MAX, {-1,-1}, true);
}
Move robot::botMove(BombAI& board, int depth) {
    auto tmp = board.getBoard();
    auto moves = generateMoves(tmp);
    if (moves.empty()) return Move(10,10,0);
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = BOT;
        if (checkWin(tmp,x,y,BOT)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = PLAYER;
        if (checkWin(tmp,x,y,PLAYER)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = BOT;
        if (checkCreateOpenFour(tmp,x,y,BOT)) return m;
        tmp[x][y] = 0;
    }
    for (auto m : moves) {
        auto [x,y] = m.getXY();
        tmp[x][y] = PLAYER;
        if (checkCreateOpenFour(tmp,x,y,PLAYER)) return m;
        tmp[x][y] = 0;
    }
    return findBestMove(board, depth);
}
void robot::HowToRobotMove(BombAI& board, int m) {
    auto best = botMove(board, 3);
    auto [x,y] = best.getXY();
    board(x,y) = m;
}

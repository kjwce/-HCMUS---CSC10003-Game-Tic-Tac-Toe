#pragma once
#include <utility>

using namespace std;
class Move {
    int x, y;
    int score;
public:
    Move();
    Move(int x, int y, int score);
    Move& operator=(const Move&);
    void setScore(int newScore);
    int getScore();
    pair<int, int> getXY();
    void setMove(int x, int y, int score);

};
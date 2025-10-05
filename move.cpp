#include "bot.h"

Move::Move() : x(-1), y(-1), score(0) {}

Move& Move::operator=(const Move& move) {
    x = move.x;
    y = move.y;
    score = move.score;

    return *this;
}

Move::Move(int x, int y, int score) : x(x), y(y), score(score) {}

void Move::setScore(int newScore) {
    score = newScore;
}

int Move::getScore() {
    return score;
}

pair<int, int> Move::getXY() {
    pair<int, int> xy;
    xy.first = x;
    xy.second = y;

    return xy;
}

void Move::setMove(int x, int y, int score) {
    this->x = x;
    this->y = y;
    this->score = score;
}




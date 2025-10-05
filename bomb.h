#pragma once

#include "board.h"

class Bomb {
private:
    static std::mt19937 rng;
    static const int SIZE = 15;
    static const int maxBombs = 3;

    
public:
    // Khởi tạo random seed
    static void initRandom();
    
    // Tạo danh sách các ô có thể nổ (ô đã có quân cờ)
    static std::vector<Position> getOccupiedCells(const std::vector<std::vector<int>>& board);
    
    // Chọn ngẫu nhiên các ô để nổ
    static std::vector<Position> selectBombPositions(const std::vector<Position>& occupiedCells, int maxBombs = 3);
    
    // Thực hiện nổ bomb và trả về danh sách vị trí bị nổ
    static std::vector<Position> explode(std::vector<std::vector<int>>& board, std::vector<Explosion>& explosions);
    
    // Kiểm tra xem có nên kích hoạt bomb không (dựa trên countdown)
    static bool shouldExplode(int countdown);
    
    // Tính toán số lượng bomb sẽ nổ dựa trên số ô đã chiếm
    static int calculateBombCount(int occupiedCells);
};

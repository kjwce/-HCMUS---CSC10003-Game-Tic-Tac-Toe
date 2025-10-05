#include "bomb.h"

// Khởi tạo static member
std::mt19937 Bomb::rng;

void Bomb::initRandom() {
    rng.seed(static_cast<unsigned int>(std::time(nullptr)));
}

std::vector<Position> Bomb::getOccupiedCells(const std::vector<std::vector<int>>& board) {
    std::vector<Position> occupiedCells;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                occupiedCells.push_back(Position(i, j));
            }
        }
    }
    
    return occupiedCells;
}

std::vector<Position> Bomb::selectBombPositions(const std::vector<Position>& occupiedCells, int maxBombs) {
    std::vector<Position> selectedPositions;
    
    if (occupiedCells.empty()) {
        return selectedPositions;
    }
    
    // Tính số bomb sẽ nổ (1-3 bomb, không vượt quá số ô có quân)
    int bombCount = std::min(maxBombs, static_cast<int>(occupiedCells.size()));
    bombCount = std::max(1, bombCount);
    
    // Random số bomb từ 1 đến bombCount
    std::uniform_int_distribution<int> bombCountDist(1, bombCount);
    int finalBombCount = bombCountDist(rng);
    
    // Tạo copy của danh sách để không làm thay đổi original
    std::vector<Position> availablePositions = occupiedCells;
    
    // Chọn ngẫu nhiên các vị trí
    for (int i = 0; i < finalBombCount && !availablePositions.empty(); i++) {
        std::uniform_int_distribution<int> posDist(0, availablePositions.size() - 1);
        int randomIndex = posDist(rng);
        
        selectedPositions.push_back(availablePositions[randomIndex]);
        availablePositions.erase(availablePositions.begin() + randomIndex);
    }
    
    return selectedPositions;
}

std::vector<Position> Bomb::explode(std::vector<std::vector<int>>& board, std::vector<Explosion>& explosions) {
    // Lấy danh sách các ô có quân cờ
    std::vector<Position> occupiedCells = getOccupiedCells(board);
    
    // Chọn các vị trí để nổ
    std::vector<Position> bombPositions = selectBombPositions(occupiedCells);
    
    // Thực hiện nổ
    for (const Position& pos : bombPositions) {
        // Xóa quân cờ tại vị trí này
        board[pos.row][pos.col] = 0;
        
        // Thêm hiệu ứng nổ
        explosions.push_back(Explosion(pos.row, pos.col));
    }
    
    return bombPositions;
}

bool Bomb::shouldExplode(int countdown) {
    return countdown <= 0;
}

int Bomb::calculateBombCount(int occupiedCells) {
    if (occupiedCells <= 5) return 1;
    if (occupiedCells <= 15) return 2;
    return 3;
}
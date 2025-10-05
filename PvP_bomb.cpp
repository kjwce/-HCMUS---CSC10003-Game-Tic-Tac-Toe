#include "board.h"
#include "bomb.h"
#include <iomanip>

bombBoard::bombBoard(sf::RenderWindow* win, GameState& currentState, GameState& nextState) 
    : Board(win, currentState, nextState),  // Gọi constructor của class cha
      bombElapsedBeforePause(0.0f),
      hasFirstMove(false),
      bombCountdown(BOMB_TRIGGER_SECONDS)
{
    // Chỉ cần khởi tạo các thành phần riêng của bombBoard
    // Các thành phần chung đã được khởi tạo trong Board constructor
    
    // Override frameDuration cho bomb board (nhanh hơn)
    frameDuration = 0.05f;
    
    // Khởi tạo bomb-specific components
    Bomb::initRandom();
    bombTimerClock.restart();
    
    // Reset timing cho bomb mode
    isTurnTiming = false;
}

void bombBoard::checkWin() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j <= SIZE - 5; ++j) {
            if (board[i][j] != 0 &&
                board[i][j] == board[i][j + 1] &&
                board[i][j] == board[i][j + 2] &&
                board[i][j] == board[i][j + 3] &&
                board[i][j] == board[i][j + 4]) {
                popup.show((board[i][j] == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
            if (board[j][i] != 0 &&
                board[j][i] == board[j + 1][i] &&
                board[j][i] == board[j + 2][i] &&
                board[j][i] == board[j + 3][i] &&
                board[j][i] == board[j + 4][i]) {
                popup.show((board[j][i] == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
        }
    }

    for (int i = 0; i <= SIZE - 5; ++i) {
        for (int j = 0; j <= SIZE - 5; ++j) {
            if (board[i][j] != 0 &&
                board[i][j] == board[i + 1][j + 1] &&
                board[i][j] == board[i + 2][j + 2] &&
                board[i][j] == board[i + 3][j + 3] &&
                board[i][j] == board[i + 4][j + 4]) {
                popup.show((board[i][j] == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
            if (board[i + 4][j] != 0 &&
                board[i + 4][j] == board[i + 3][j + 1] &&
                board[i + 4][j] == board[i + 2][j + 2] &&
                board[i + 4][j] == board[i + 1][j + 3] &&
                board[i + 4][j] == board[i][j + 4]) {
                popup.show((board[i + 4][j] == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
        }
    }
}

void bombBoard::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
    if (menu->isOpened()) {
        menu->handleEvent(event);
        return;
    }

    for (auto* btn : buttons) {
        btn->update(window, event);

        if (btn->wasClicked(window, event)) {
            auto* slideBtn = dynamic_cast<SlideButton*>(btn);
            if (slideBtn) {
                if (slideBtn->getTargetSlide() == GameState::PLAY_BOARD) {
                    isPaused = true;
                    isTurnTiming = false;
                    elapsedBeforePause += clock.getElapsedTime().asSeconds();
                    bombElapsedBeforePause += bombTimerClock.getElapsedTime().asSeconds();
                    hasPausedOnce = true;
                    menu->open();
                    return;
                } else {
                    currentState = slideBtn->getTargetSlide();
                    return;
                }
            }
        }
    }

    if (popup.isVisible()) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            PopupAction action = popup.handleClick(mousePos);

            if (ignoreNextClick) {
                ignoreNextClick = false;
                return;
            }

            if (action == PopupAction::REPLAY) {
                resetBoard();
                popup.hide();
                ignoreNextClick = true;
                return;
            } else if (action == PopupAction::HOME) {
                currentState = GameState::MAIN_MENU;
                resetBoard();
                popup.hide();
                ignoreNextClick = true;
                return;
            }
        }
        return;
    }

    if (gameOver) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        int x = (mousePos.x - offsetX) / CELL_SIZE;
        int y = (mousePos.y - offsetY) / CELL_SIZE;

        if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[y][x] == 0) {
            board[y][x] = turnX ? 1 : 2;
            // Đặt lại thời gian cho mỗi nước đi mới
            hasFirstMove = true; // Đánh dấu đã có nước đi
            elapsedBeforePause = 0.f; // Reset thời gian trước khi pause
            bombElapsedBeforePause = 0.f; // Reset thời gian bom
            clock.restart(); // Restart đồng hồ cho lượt mới
            bombTimerClock.restart(); // Restart đồng hồ bom
            isTurnTiming = true; // Kích hoạt đếm ngược thời gian

            turnX = !turnX;
            updateArrowPosition();
            checkWin();
        }
    }
}

void bombBoard::draw(sf::RenderWindow& window) {
    Board::draw(window);
    drawBombCountdown();
    drawExplosions();
}



void bombBoard::update() {
    if (!hasFirstMove || isPaused || gameOver) return;

    float totalElapsed = elapsedBeforePause + clock.getElapsedTime().asSeconds();
    if (totalElapsed >= turnTimeLimit) {
        popup.show((turnX ? "X" : "O") + std::string(" timed out!\n") +
                   (turnX ? "O" : "X") + " wins!");
        gameOver = true;
        isTurnTiming = false;
        return;
    }

    if (totalFrames > 0 && frameClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        backgroundSprite.setTexture(backgroundFrames[currentFrameIndex]);
        frameClock.restart();
    }

    float bombElapsed = bombElapsedBeforePause + bombTimerClock.getElapsedTime().asSeconds();
    if (bombElapsed >= 1.0f) {
        bombCountdown--;
        bombTimerClock.restart();
        bombElapsedBeforePause = 0;
    }

    if (Bomb::shouldExplode(bombCountdown)) {
        std::cout << "BOMB EXPLOSION ACTIVATED!\n";
        activeExplosions.clear();
        Bomb::explode(board, activeExplosions);
        bombCountdown = BOMB_TRIGGER_SECONDS;
        bombTimerClock.restart();
        bombElapsedBeforePause = 0;
        explosionAnimationClock.restart();
        checkWin();
    }

    if (!activeExplosions.empty() && explosionAnimationClock.getElapsedTime().asSeconds() >= EXPLOSION_DISPLAY_DURATION) {
        activeExplosions.clear();
    }

    if (isTurnTiming) {
        float elapsed = clock.getElapsedTime().asSeconds();
        if (elapsed >= turnTimeLimit) {
            std::cout << "Time's up!\n";
            turnX = !turnX;
            updateArrowPosition();
            clock.restart();
            isTurnTiming = false;
        }
    }
}


void bombBoard::drawBombCountdown() {
    sf::Text bombText;
    bombText.setFont(font);
    bombText.setString("Bomb in: " + std::to_string(bombCountdown) + "s");
    bombText.setCharacterSize(40);
    bombText.setFillColor(sf::Color::White);
    bombText.setOutlineColor(sf::Color::Black);
    bombText.setOutlineThickness(2);
    bombText.setPosition(1050, 650);
    window->draw(bombText);
}

void bombBoard::drawExplosions() {
    if (activeExplosions.empty()) return;

    float elapsed = explosionAnimationClock.getElapsedTime().asSeconds();
    float progress = elapsed / EXPLOSION_DISPLAY_DURATION;
    sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1.0f - progress));

    for (const auto& exp : activeExplosions) {
        sf::CircleShape explosionCircle(CELL_SIZE / 2.5f);
        explosionCircle.setOrigin(explosionCircle.getRadius(), explosionCircle.getRadius());
        explosionCircle.setPosition(
            offsetX + exp.col * CELL_SIZE + CELL_SIZE / 2.0f,
            offsetY + exp.row * CELL_SIZE + CELL_SIZE / 2.0f
        );
        explosionCircle.setFillColor(sf::Color(255, 100, 0, alpha));
        explosionCircle.setOutlineThickness(2);
        explosionCircle.setOutlineColor(sf::Color(255, 255, 0, alpha));
        window->draw(explosionCircle);
    }
}

void bombBoard::initUI(GameState& currentState, GameState& nextState) {
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Cannot load font\n";
        exit(-1);
    }
    if (!arrowTexture.loadFromFile("asset/Picture14.png")) {
        std::cout << "Can't load Picture14.png (arrow)\n";
        return;
    }
    arrowSprite.setTexture(arrowTexture);
    arrowSprite.setScale(0.15f, 0.15f);

    player1Text.setFont(font);
    player1Text.setString("Player X");
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setStyle(sf::Text::Bold);
    player1Text.setOutlineColor(sf::Color::Black);
    player1Text.setOutlineThickness(2);

    player2Text.setFont(font);
    player2Text.setString("Player O");
    player2Text.setCharacterSize(30);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setStyle(sf::Text::Bold);
    player2Text.setOutlineColor(sf::Color::Black);
    player2Text.setOutlineThickness(2);

    player1Text.setPosition(60, 305);
    player2Text.setPosition(60, 405);

    picture2();
    updateArrowPosition();

    isPaused = false;
    isTurnTiming = false;
    clockOffset = 0;
    pausedElapsedTime = 0;
    clock.restart();

    auto* btnMenu = new SlideButton(GameState::PLAY_BOARD);
    btnMenu->loadTextures("asset/Picture11.png", "asset/Picture111.png");
    btnMenu->setScale(0.2f, 0.2f);
    btnMenu->setPosition(50, 20);
    buttons.push_back(btnMenu);

    updateArrowPosition();

    menu->onResumeCallback = [this]() {
        bool gameStarted = false;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board[i][j] != 0) {
                    gameStarted = true;
                    break;
                }
            }
            if (gameStarted) break;
        }

        if (gameStarted && isPaused && hasPausedOnce) {
            elapsedBeforePause += pauseStartTime.asSeconds();
            bombElapsedBeforePause += bombTimerClock.getElapsedTime().asSeconds();
            clock.restart();
            bombTimerClock.restart();
            isPaused = false;
            isTurnTiming = true;
            hasPausedOnce = false;
        } else {
            isPaused = false;
            isTurnTiming = false;
            hasPausedOnce = false;
            clock.restart();
            bombTimerClock.restart();
            elapsedBeforePause = 0.f;
            bombElapsedBeforePause = 0.f;
        }
        ignoreNextClick = true;
        menu->close();
    };

    menu->onNewGameCallback = [this]() {
        resetBoard();
        ignoreNextClick = true;
        menu->close();
    };

    menu->onQuitCallback = [this, &currentState, &nextState]() {
        isPaused = false;
        isTurnTiming = false;
        nextState = GameState::MAIN_MENU;
        resetBoard();
        menu->close();
    };
}

void bombBoard::drawUI() {
    Board::drawUI();

    if (popup.isVisible()) {
        popup.draw(*window);
    }
}

void bombBoard::resetBoard() {
    // Gọi phiên bản của lớp cha trước
    Board::resetBoard();

    // Sau đó reset các thành phần riêng của bombBoard
    bombElapsedBeforePause = 0.f;
    bombTimerClock.restart();
    activeExplosions.clear();
    bombCountdown = BOMB_TRIGGER_SECONDS;
    hasFirstMove = false; 
}
#include "board.h"
#include "bot.h"
#include "move.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>

BoardAI::BoardAI(sf::RenderWindow* win, GameState& currentState, GameState& nextState)
    : Board(win, currentState, nextState)  
{
    currentPlayer = PLAYER;
    turnX = true;
    gameOver = false;
    botThinking = false;
    updateArrowPosition();

    // Thời gian giới hạn cho AI
    turnTimeLimit = 30.0f;
    clock.restart();

    // Animation frame
    frameClock.restart();
    hasFirstMove = false;  // Thêm nếu chưa có
    botClock.restart();    // Reset botClock
    stopBot = false;
}

bool BoardAI::isWinner() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int player = board[i][j];
            if (player == 0) continue;

            if (j + 4 < SIZE &&
                player == board[i][j + 1] &&
                player == board[i][j + 2] &&
                player == board[i][j + 3] &&
                player == board[i][j + 4]) {
                return true;
            }

            if (i + 4 < SIZE &&
                player == board[i + 1][j] &&
                player == board[i + 2][j] &&
                player == board[i + 3][j] &&
                player == board[i + 4][j]) {
                return true;
            }

            if (i + 4 < SIZE && j + 4 < SIZE &&
                player == board[i + 1][j + 1] &&
                player == board[i + 2][j + 2] &&
                player == board[i + 3][j + 3] &&
                player == board[i + 4][j + 4]) {
                return true;
            }

            if (i - 4 >= 0 && j + 4 < SIZE &&
                player == board[i - 1][j + 1] &&
                player == board[i - 2][j + 2] &&
                player == board[i - 3][j + 3] &&
                player == board[i - 4][j + 4]) {
                return true;
            }
        }
    }
    return false;
}

void BoardAI::checkWin() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j <= SIZE - 5; ++j) {
            if (board[i][j] != 0 &&
                board[i][j] == board[i][j + 1] &&
                board[i][j] == board[i][j + 2] &&
                board[i][j] == board[i][j + 3] &&
                board[i][j] == board[i][j + 4]) {
                popup.show((board[i][j] == 1 ? "\t You Win" : "\tYou lose"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
            if (board[j][i] != 0 &&
                board[j][i] == board[j + 1][i] &&
                board[j][i] == board[j + 2][i] &&
                board[j][i] == board[j + 3][i] &&
                board[j][i] == board[j + 4][i]) {
                popup.show((board[j][i] == 1 ? "\t You Win" : "\tYou lose"));
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
                popup.show((board[i][j] == 1 ? "\t You Win" : "\tYou lose"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
            if (board[i + 4][j] != 0 &&
                board[i + 4][j] == board[i + 3][j + 1] &&
                board[i + 4][j] == board[i + 2][j + 2] &&
                board[i + 4][j] == board[i + 1][j + 3] &&
                board[i + 4][j] == board[i][j + 4]) {
                popup.show((board[i + 4][j] == 1 ? "\t You Win" : "\tYou lose"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
        }
    }
}

void BoardAI::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
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
                    hasPausedOnce = true;
                    menu->open();
                    return;
                }
                currentState = slideBtn->getTargetSlide();
                return;
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
                resetBoard();
                currentState = MAIN_MENU;
                popup.hide();
                ignoreNextClick = true;
                return;
            }
            if (popup.isVisible()) return;
        }
        return;
    }

    if (gameOver || botThinking) return;

    if (currentPlayer == PLAYER &&
        event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = window.mapPixelToCoords(
            {event.mouseButton.x, event.mouseButton.y}
        );

        if (ignoreNextClick) {
            ignoreNextClick = false;
            return;
        }

        int j = (mousePos.x - offsetX) / CELL_SIZE;
        int i = (mousePos.y - offsetY) / CELL_SIZE;

        if (i >= 0 && i < SIZE && j >= 0 && j < SIZE && board[i][j] == 0) {
            board[i][j] = currentPlayer;
            if (!hasFirstMove) {
                hasFirstMove = true;
                clock.restart();
                isTurnTiming = true;
            } else {
                clock.restart();
                isTurnTiming = true;
            }

            checkWin();
            if (!gameOver) {
                currentPlayer = BOT;
                turnX = false;
                updateArrowPosition();
                botClock.restart();
            }
        }
    }
}


int& BoardAI::operator()(int x, int y) {
    return board[x][y];
}

vector<vector<int>> BoardAI::getBoard() {
    vector<vector<int>> newBoard(20, vector<int>(20, 0));
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            newBoard[x][y] = board[x][y];
        }
    }
    return newBoard;
}

void BoardAI::startBotThread() {
    if (botThinking || gameOver) return;

    botThinking = true;
    std::thread([this]() {
        robot bot;
        bot.HowToRobotMove(*this, currentPlayer);
        checkWin();
        if (!gameOver) {
            currentPlayer = PLAYER;
            turnX = true;
            updateArrowPosition();
            clock.restart();
        }
        botThinking = false;
    }).detach();
}


void BoardAI::initUI(GameState& currentState, GameState& nextState) {
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
    player1Text.setString("Player");
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setStyle(sf::Text::Bold);
    player1Text.setOutlineColor(sf::Color::Black);
    player1Text.setOutlineThickness(2);

    player2Text.setFont(font);
    player2Text.setString("AI bot");
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
            elapsedBeforePause += clock.getElapsedTime().asSeconds();
            clock.restart();
            isPaused = false;
            isTurnTiming = true;
            hasPausedOnce = false;
        } else {
            isPaused = false;
            isTurnTiming = false;
            hasPausedOnce = false;
            elapsedBeforePause = 0.f;
            clock.restart();
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

void BoardAI::draw(sf::RenderWindow& window) {
    Board::draw(window);
}

void BoardAI::drawUI() {
    Board::drawUI();
}

void BoardAI::update() {
    if (!hasFirstMove || isPaused || gameOver) return;

    float elapsed = clock.getElapsedTime().asSeconds();

    if (elapsed >= turnTimeLimit && !gameOver) {
        popup.show("\tTimed out!\n \tYou lose");
        gameOver = true;
        isTurnTiming = false;
        return;
    }

    if (totalFrames > 0 && frameClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        backgroundSprite.setTexture(backgroundFrames[currentFrameIndex]);
        frameClock.restart();
    }

    if (currentPlayer == BOT && !botThinking && botClock.getElapsedTime().asSeconds() >= 1.0f) {
        if (botClock.getElapsedTime().asSeconds() >= 0.5f) {
            startBotThread();
        }
    }

    if (currentPlayer == PLAYER && isTurnTiming) {
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

void BoardAI::resetBoard() {
    Board::resetBoard();

    currentPlayer = PLAYER;
    botThinking = false;
    hasFirstMove = false;
}

BoardAI::~BoardAI() {
    stopBot = true;
    if (botThread.joinable()) {
        botThread.join();
    }
    delete menu;
}
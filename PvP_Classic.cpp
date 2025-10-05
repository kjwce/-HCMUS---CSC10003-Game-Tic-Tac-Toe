#include "board.h"
#include <iomanip>
#include "Slidebutton.h"

Board::Board(sf::RenderWindow* win, GameState& currentState, GameState& nextState) : Page("asset/background.png"), window(win), board(SIZE, std::vector<int>(SIZE, 0)) {
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "cant load font\n";
        exit(-1);
    }

    sf::Vector2u winSize = window->getSize();
    offsetX = (winSize.x - CELL_SIZE * SIZE) / 2.f;
    offsetY = (winSize.y - CELL_SIZE * SIZE) / 2.f;

    clock.restart();

    menu = new Menu2(window);
    initUI(currentState, nextState);

    currentFrameIndex = 0;
    frameDuration = 1.0f / 30.0f;
    totalFrames = 0;
    loadBackgroundFrames();

    if (!backgroundFrames.empty()) {
        backgroundSprite.setTexture(backgroundFrames[0]);
        backgroundSprite.setScale(
            static_cast<float>(winSize.x) / backgroundFrames[0].getSize().x,
            static_cast<float>(winSize.y) / backgroundFrames[0].getSize().y
        );
    }
}

void Board::loadBackgroundFrames() {
    backgroundFrames.clear(); 
    std::string path = "asset/frames/ezgif-frame-";

    for (int i = 1; ; ++i) {
        std::stringstream ss;
        ss << path << std::setw(3) << std::setfill('0') << i << ".jpg";
        std::string filename = ss.str();

        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            if (i == 1) { 
                std::cout << "Error: No background frames found at " << filename << "\n";
            } else {
                std::cout << "Loaded " << (i - 1) << " background frames.\n"; 
            }
            break; 
        }
        texture.setSmooth(true);
        backgroundFrames.push_back(texture);
    }
    totalFrames = backgroundFrames.size();

    if (totalFrames > 0) {
        backgroundSprite.setTexture(backgroundFrames[0]); 
        backgroundSprite.setScale(
            (float)window->getSize().x / backgroundFrames[0].getSize().x,
            (float)window->getSize().y / backgroundFrames[0].getSize().y
        );
    } else {
        std::cout << "Warning: No background frames loaded. Background will be empty.\n";
    }
}

void Board::drawCell(int i, int j) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
    cell.setPosition(offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE);
    cell.setFillColor(sf::Color::White);
    cell.setOutlineThickness(1);
    cell.setOutlineColor({43,23,00});
    window->draw(cell);

    if (board[i][j] != 0) {
        sf::Text text;
        text.setFont(font);
        text.setString(board[i][j] == 1 ? "X" : "O");
        text.setCharacterSize(CELL_SIZE - 4);
        if (board[i][j] == 1) {
            text.setFillColor(sf::Color(128,0,78)); // Red for X
        } else {
            text.setFillColor(sf::Color(42,72,143)); // Blue for O
        }

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(offsetX + j * CELL_SIZE + CELL_SIZE / 2.0f,
                         offsetY + i * CELL_SIZE + CELL_SIZE / 2.0f);

        window->draw(text);
    }
}

void Board::checkWin() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int player = board[i][j];
            if (player == 0) continue;

            if (j + 4 < SIZE &&
                player == board[i][j + 1] &&
                player == board[i][j + 2] &&
                player == board[i][j + 3] &&
                player == board[i][j + 4]) {
                popup.show((player == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }

            if (i + 4 < SIZE &&
                player == board[i + 1][j] &&
                player == board[i + 2][j] &&
                player == board[i + 3][j] &&
                player == board[i + 4][j]) {
                popup.show((player == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }

            if (i + 4 < SIZE && j + 4 < SIZE &&
                player == board[i + 1][j + 1] &&
                player == board[i + 2][j + 2] &&
                player == board[i + 3][j + 3] &&
                player == board[i + 4][j + 4]) {
                popup.show((player == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }

            if (i - 4 >= 0 && j + 4 < SIZE &&
                player == board[i - 1][j + 1] &&
                player == board[i - 2][j + 2] &&
                player == board[i - 3][j + 3] &&
                player == board[i - 4][j + 4]) {
                popup.show((player == 1 ? "Player X" : "Player O") + std::string(" wins!"));
                gameOver = true;
                isTurnTiming = false;
                return;
            }
        }
    }
}

void Board::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
    // B1: Ưu tiên xử lý các nút SlideButton
    for (auto* btn : buttons) {
        btn->update(window, event);

        if (btn->wasClicked(window, event)) {
            auto* slideBtn = dynamic_cast<SlideButton*>(btn);
            if (slideBtn) {
                if (slideBtn->getTargetSlide() == GameState::PLAY_BOARD) {
                    isPaused = true;
                    isTurnTiming = false;
                    pauseStartTime = clock.getElapsedTime();
                    hasPausedOnce = true;
                    menu->open();
                    return; // Stop processing further events
                }
                currentState = slideBtn->getTargetSlide();
                return; // Stop processing further events
            }
        }
    }

    // B2: Xử lý popup nếu đang hiển thị
    if (popup.isVisible()) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            PopupAction action = popup.handleClick(mousePos);
            if (ignoreNextClick) {
                ignoreNextClick = false;
                return; // Ignore stray clicks
            }

            if (action == PopupAction::REPLAY) {
                resetBoard();
                popup.hide();
                ignoreNextClick = true;
                return; // Stop processing to prevent board click
            } else if (action == PopupAction::HOME) {
                resetBoard();
                currentState = GameState::MAIN_MENU;
                popup.hide();
                ignoreNextClick = true;
                return; // Stop processing
            }
            return; 
        }
        return; 
    }

    // B3: Xử lý menu nếu đang mở
    if (menu->isOpened()) {
        menu->handleEvent(event);
        return; // Stop processing further events
    }

    // B4: Xử lý click chơi game
    if (gameOver || isPaused) return;
    window.setView(window.getDefaultView());
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (ignoreNextClick) {
            ignoreNextClick = false;
            return;
        }

        if (bestsprite.size() > 0 && bestsprite[0].getGlobalBounds().contains(mousePos)) {
            std::cout << "Clicked on menu arrow! Opening menu...\n";
            menu->open();
            return;
        }

        int x = (mousePos.x - offsetX) / CELL_SIZE;
        int y = (mousePos.y - offsetY) / CELL_SIZE;

        if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[y][x] == 0) {
            board[y][x] = turnX ? 1 : 2;
            checkWin();
            if (!gameOver) {
                turnX = !turnX;
                updateArrowPosition();
                elapsedBeforePause = 0;
                clock.restart();
                isTurnTiming = true; // Start timer on first move
                isPaused = false;
            }
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    if (!backgroundFrames.empty())
        window.draw(backgroundSprite);

    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            drawCell(i, j);
        }
    }

    drawUI();
    for (auto* btn : buttons)
        btn->draw(window);

    if (menu->isOpened()) {
        menu->draw();
    }
    drawTime();
    popup.draw(window);
}

void Board::picture1() {
    sf::Sprite s;
    if (!texture1.loadFromFile("asset/Picture11.png")) {
        std::cout << "can't create picture 11\n";
        return;
    }
    s.setTexture(texture1);
    s.setScale(0.15f, 0.15f);
    s.setPosition(50, 20);
    bestsprite.push_back(s);
}

void Board::picture2() {
    sf::Sprite s;
    if (!texture2.loadFromFile("asset/Picture12.png")) {
        std::cout << "can't create picture 12\n";
        return;
    }
    s.setTexture(texture2);
    s.setScale(0.2f, 0.2f);
    s.setPosition(1010, 20);
    bestsprite.push_back(s);
}

void Board::initUI(GameState& currentState, GameState& nextState) {
    resetBoard();
    ignoreNextClick = true; // Prevent stray clicks on initialization

    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Cannot load font\n";
        exit(-1);
    }

    if (!arrowTexture.loadFromFile("asset/Picture14.png")) {
        std::cout << "Can't create arrow picture\n";
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
            clock.restart();
            isPaused = false;
            isTurnTiming = true;
            hasPausedOnce = false;
        } else {
            isPaused = false;
            isTurnTiming = false; // Wait for first move to start timer
            hasPausedOnce = false;
            clock.restart();
            elapsedBeforePause = 0;
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
        resetBoard();
        isPaused = false;
        isTurnTiming = false;
        nextState = GameState::MAIN_MENU;
        menu->close();
    };
}

void Board::update() {
    if (isPaused || !isTurnTiming || gameOver) return;
    float totalElapsed = elapsedBeforePause + clock.getElapsedTime().asSeconds();

    if (totalElapsed >= turnTimeLimit) {
        popup.show((turnX ? "X" : "O") + std::string(" timed out!\n") + (turnX ? "O" : "X") + " wins!"); 
        gameOver = true;
        isTurnTiming = false;
        return;
    }

    if (totalFrames > 0 && frameClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        backgroundSprite.setTexture(backgroundFrames[currentFrameIndex]);
        frameClock.restart();
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

void Board::drawTime() {
    if (!isTurnTiming) return;
    float elapsed = elapsedBeforePause;
    if (!isPaused) {
        elapsed += clock.getElapsedTime().asSeconds();
    }

    int timeLeft = std::max(0, static_cast<int>(turnTimeLimit - elapsed));

    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;

    std::stringstream ss;
    ss << minutes << ":";
    if (seconds < 10)
        ss << "0";
    ss << seconds;

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString(ss.str());
    timeText.setCharacterSize(50);
    timeText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = timeText.getLocalBounds();
    timeText.setPosition(1100,20);
    timeText.setOutlineColor(sf::Color::Black);
    timeText.setOutlineThickness(2);

    window->draw(timeText);
}

void Board::drawUI() {
    for (auto* btn : buttons) {
        btn->draw(*window);
    }
    for (const auto& sprite : bestsprite) {
        window->draw(sprite);
    }

    window->draw(arrowSprite);
    window->draw(player1Text);
    window->draw(player2Text);
    // if (popup.isVisible()) {
    //     popup.draw(*window);
    // }
}

void Board::updateArrowPosition() {
    if (turnX) {
        arrowSprite.setPosition(20, 300); // Player X
    } else {
        arrowSprite.setPosition(20, 400); // Player O
    }
}

void Board::resetBoard() {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            board[i][j] = 0;

    gameOver = false;
    turnX = true;
    isPaused = false; // Allow clicks immediately
    isTurnTiming = false;
    clockOffset = 0;
    pausedElapsedTime = 0;
    elapsedBeforePause = 0;
    hasPausedOnce = false;
    ignoreNextClick = true;
    updateArrowPosition();
    clock.restart();
    popup.hide();
}
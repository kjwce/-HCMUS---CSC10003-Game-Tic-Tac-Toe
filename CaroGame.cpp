#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameEngine.h"
#include "board.h"
#include "Page.h"
#include "bomb.h"

// Hàm hiệu ứng chuyển slide (fade)
void fadeTransition(sf::RenderWindow& window, Page* fromSlide, Page* toSlide) {
    const float duration = 0.3f;
    sf::Clock clock;
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color::Black);

    while (clock.getElapsedTime().asSeconds() < duration) {
        float alpha = (clock.getElapsedTime().asSeconds() / duration) * 255;
        if (alpha > 255) alpha = 255;
        window.clear();
        fromSlide->draw(window);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
        window.draw(fadeRect);
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tic Tac Toe");
    window.setFramerateLimit(60);

    sf::Clock deltaClock;

    GameState currentState = MAIN_MENU;
    GameState nextState = currentState;

    // Tạo SoundManager
    SoundManager sound;

    // Tạo các slide
    Menu1* sharedMenu = new Menu1(&window);
    Home home(&sound);
    Tutorial Tutorial(&window, sharedMenu);
    Classic Classic(&window, sharedMenu);
    BombModeMenu BombModeMenu(&window, sharedMenu);
    Board* boardSlide = new Board(&window, currentState, nextState);
    boardSlide->initUI(currentState, nextState);
    bombBoard* bombGame = new bombBoard(&window, currentState, nextState);
    bombGame->initUI(currentState, nextState);
    BoardAI* gameBoard = new BoardAI(&window, currentState, nextState);
    gameBoard->initUI(currentState, nextState);
    BombAI* bombai = new BombAI(&window, currentState, nextState);
    bombai->initUI(currentState, nextState);

    // Mảng slide
    std::vector<Page*> slides(9);
    slides[MAIN_MENU] = &home;
    slides[CLASSIC_MODE] = &Classic;
    slides[BOMB_MODE] = &BombModeMenu;
    slides[HOW_TO_PLAY] = &Tutorial;
    slides[PLAY_BOARD] = boardSlide;
    slides[BOMB_BOARD] = bombGame;
    slides[AI_BOARD] = gameBoard;
    slides[AI_BOMB] = bombai;

    // Gọi setup cho mỗi slide
    for (auto* slide : slides) {
        if (slide) {
            slide->setup();
        }
    }

    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        float dt = deltaClock.restart().asSeconds();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Xử lý sự kiện cho các slide
            if (currentState == BOMB_BOARD) {
                bombGame->handleEvent(window, event, nextState);
            } else if (currentState == AI_BOARD) {
                gameBoard->handleEvent(window, event, nextState);
            } else if (currentState == AI_BOMB) {
                bombai->handleEvent(window, event, nextState);
            } else if (slides[currentState]) {
                slides[currentState]->handleEvent(window, event, nextState);
            }

            // Xử lý click cho PLAY_BOARD
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (currentState == PLAY_BOARD) {
                    PopupAction action = boardSlide->checkPopupClick(mousePos);
                    if (action == PopupAction::REPLAY) {
                        boardSlide->resetBoard();
                    } else if (action == PopupAction::HOME) {
                        nextState = MAIN_MENU;
                    }
                }
            }
        }

        // Xử lý chuyển slide
        if (nextState != currentState) {
            std::cout << "Chuyển từ trạng thái " << (int)currentState << " sang " << (int)nextState << "\n";
            if (currentState == PLAY_BOARD && nextState != PLAY_BOARD) {
                boardSlide->resetBoard();
            } else if (currentState == AI_BOARD && nextState != AI_BOARD) {
                gameBoard->resetBoard();
            } else if (currentState == BOMB_BOARD && nextState != BOMB_BOARD) {
                bombGame->resetBoard();
            } else if (currentState == AI_BOMB && nextState != AI_BOMB) {
                bombai->resetBoard();
            }

            if (slides[currentState] && slides[nextState]) {
                fadeTransition(window, slides[currentState], slides[nextState]);
            } else {
                std::cerr << "Lỗi: Không thể chuyển slide, currentState=" << (int)currentState
                          << ", nextState=" << (int)nextState << "\n";
            }

            // Xóa hàng đợi sự kiện và đặt ignoreNextClick khi chuyển về MAIN_MENU
            if (nextState == MAIN_MENU) {
                while (window.pollEvent(event)) {} // Xóa tất cả sự kiện đang chờ
                Home* home = dynamic_cast<Home*>(slides[MAIN_MENU]);
                if (home) {
                    home->setIgnoreNextClick(true);
                    std::cout << "Đặt ignoreNextClick = true cho Home sau khi chuyển về MAIN_MENU\n";
                }
            }

            currentState = nextState;
        }

        window.clear();
        if (currentState == PLAY_BOARD) {
            boardSlide->update();
            boardSlide->draw(window);
            boardSlide->drawUI();
        } else if (currentState == BOMB_BOARD) {
            bombGame->update();
            bombGame->draw(window);
            bombGame->drawUI();
        } else if (currentState == AI_BOARD) {
            gameBoard->update();
            gameBoard->draw(window);
            gameBoard->drawUI();
        } else if (currentState == AI_BOMB) {
            bombai->update();
            bombai->draw(window);
            bombai->drawUI();
        } else {
            if (slides[currentState]) {
                slides[currentState]->update(dt, nextState);
                slides[currentState]->draw(window);
            }
        }
        window.display();

        std::cout << "Trạng thái hiện tại: " << (int)currentState << ", nextState: " << (int)nextState << "\n";
    }

    delete sharedMenu;
    delete boardSlide;
    delete bombGame;
    delete gameBoard;
    delete bombai;

    return 0;
}
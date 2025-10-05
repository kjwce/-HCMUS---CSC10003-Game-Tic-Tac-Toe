#pragma once
#include "GameEngine.h"
#include "Page.h"

// Position struct for Bomb class
struct Position {
    int row, col;
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// Explosion struct to store details about an explosion for drawing
struct Explosion {
    int row, col;
    Explosion(int r, int c) : row(r), col(c) {}
};

// Base Board class
class Board : public Page {
protected:
    Menu2* menu;
    Popup popup;
    static const int SIZE = 20;
    static const int CELL_SIZE = 30;
    static const int Board_SIZE = SIZE * CELL_SIZE;

    sf::RenderWindow* window;
    
    std::vector<std::vector<int>> board;
    bool turnX = true;

    sf::Font font;
    sf::Clock clock;
    float turnTimeLimit = 30.0f;
    bool gameOver = false;
    
    float offsetX;
    float offsetY;

    sf::Text player1Text;
    sf::Text player2Text;

    std::vector<sf::Texture> backgroundFrames;
    sf::Sprite backgroundSprite;
    sf::Clock frameClock;

    float pausedElapsedTime = 0;
    float clockOffset = 0;
    bool isPaused = false;
    bool isTurnTiming = true;
    float elapsedBeforePause = 0.f;
    sf::Time pauseStartTime;
    bool hasPausedOnce = false;
    bool ignoreNextClick = false;

    int currentFrameIndex;
    float frameDuration;
    int totalFrames;
    Menu2* getMenu() { return menu; }

    std::vector<SlideButton*> buttons;

    virtual void drawCell(int i, int j);
    virtual void loadBackgroundFrames();

    sf::Texture texture1;
    sf::Texture texture2;

    sf::Texture arrowTexture;
    sf::Sprite arrowSprite;

public:
    std::vector<sf::Sprite> bestsprite;

    Board(sf::RenderWindow* win, GameState& currentState, GameState& nextState);
    virtual ~Board() { delete menu; }

    void setup() override {}
    virtual void draw(sf::RenderWindow& window) override;
    virtual void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void drawTime();
    virtual void initUI(GameState& currentState, GameState& nextState);
    virtual void update();
    void updateArrowPosition();
    virtual void drawUI();
    virtual void resetBoard();
    PopupAction checkPopupClick(sf::Vector2f mousePos) {
        if (popup.isVisible()) {
            return popup.handleClick(mousePos);
        }
        return PopupAction::NONE;
    }
    virtual void checkWin();
    void picture1();
    void picture2();
};

// bombBoard class
class bombBoard : public Board {
private:
    float bombElapsedBeforePause = 0.f;
    bool hasFirstMove = false;
    sf::Text message;

    int bombCountdown;
    sf::Clock bombTimerClock;
    static const int BOMB_TRIGGER_SECONDS = 13;
    std::vector<Explosion> activeExplosions;
    sf::Clock explosionAnimationClock;
    const float EXPLOSION_DISPLAY_DURATION = 0.5f;

public:
    bombBoard(sf::RenderWindow* win, GameState& currentState, GameState& nextState);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void update() override;
    void drawUI() override;
    void initUI(GameState& currentState, GameState& nextState) override;
    void draw(sf::RenderWindow& window) override;
    void resetBoard() override;   
    void drawBombCountdown();
    void drawExplosions();
    void checkWin() override;     
};

// BoardAI class
class BoardAI : public Board {
protected:
    int currentPlayer = 1;
    sf::Clock botClock;
    std::atomic<bool> botThinking{false};

    std::thread botThread;
    std::atomic<bool> stopBot{false};

    bool hasFirstMove = false;
    sf::Text message;

public:
    BoardAI(sf::RenderWindow* win, GameState& currentState, GameState& nextState);
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void startBotThread();
    void checkWin() override;
    bool isWinner() ;   
    int& operator()(int x, int y);
    vector<vector<int>> getBoard();
    void initUI(GameState& currentState, GameState& nextState) override;
    void drawUI() override;
    void update() override;
    void resetBoard() override;   
    ~BoardAI();
};

// BombAI class
class BombAI : public BoardAI {
private:
    float bombElapsedBeforePause = 0.f;
    sf::Clock botClock;
    std::atomic<bool> botThinking{false};

    bool hasFirstMove = false;
    sf::Text message;

    int bombCountdown;
    sf::Clock bombTimerClock;
    static const int BOMB_TRIGGER_SECONDS = 13;
    std::vector<Explosion> activeExplosions;
    sf::Clock explosionAnimationClock;
    const float EXPLOSION_DISPLAY_DURATION = 0.5f;

public:
    BombAI(sf::RenderWindow* win, GameState& currentState, GameState& nextState);
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void checkWin() override;
    bool isWinner();     
    void initUI(GameState& currentState, GameState& nextState) override;
    void drawUI() override;
    void update() override;
    void drawBombCountdown();
    void drawExplosions();
    void resetBoard() override;
};

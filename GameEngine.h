#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Slidebutton.h"
#include "GameState.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include <iomanip>
#include <random>
#include <ctime>
#include <thread>
#include <atomic>

using namespace sf;
using namespace std;

#define PLAYER 1
#define BOT 2

class SoundManager {
private:
    sf::Music backgroundMusic;

public:
    void playBackground(const std::string& path) {
        if (!backgroundMusic.openFromFile(path)) {
            std::cout << "Failed to load background music: " << path << "\n";
            return;
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
        std::cout << "Playing background music: " << path << "\n";
    }

    void stopBackground() {
        backgroundMusic.stop();
        std::cout << "Background music stopped\n";
    }

    void pauseBackground() {
        backgroundMusic.pause();
        std::cout << "Background music paused\n";
    }

    void resumeBackground() {
        backgroundMusic.play();
        std::cout << "Background music resumed\n";
    }
};

class Game {
    RenderWindow* window;
    void initWindow();

public:
    Game();
    virtual ~Game();
    void run();
    void update();
    void render();
};

class Player {
public:
    Player() {}
    virtual ~Player() {}
    void update();
    void render();
};

class Menu {
public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual bool isOpened() const = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void draw() = 0;
    virtual ~Menu() {}
};

class Menu1 : public Menu {
private:
    sf::RenderWindow* window;
    sf::Font font;
    std::vector<sf::Text> items;
    bool isOpen;
    sf::RectangleShape background;
    float menuWidth;
    float slideSpeed;
    float currentX;
    int activeIndex;

public:
    Menu1(sf::RenderWindow* win);
    int getActiveIndex() const { return activeIndex; }
    void open() override;
    void close() override;
    bool isOpened() const override;
    void handleEvent(const sf::Event& event) override;
    void draw() override;
    void update(float dt);
    int getItemCount() const { return items.size(); }
    sf::FloatRect getItemBounds(int index) const {
        if (index >= 0 && index < items.size()) {
            return items[index].getGlobalBounds();
        }
        return sf::FloatRect();
    }
};

class Menu2 : public Menu {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Text resumeText, quitText, newText;
    bool isOpen;
    sf::Texture panelTexture;
    sf::Sprite panelSprite;

public:
    Menu2(sf::RenderWindow* win);
    std::function<void()> onResumeCallback;
    std::function<void()> onNewGameCallback;
    std::function<void()> onQuitCallback;
    void open() override;
    void close() override;
    bool isOpened() const override;
    void handleEvent(const sf::Event& event) override;
    void draw() override;
};

enum class PopupAction {
    NONE,
    REPLAY,
    HOME
};

class Popup {
private:
    sf::Texture textureScroll, textureReplay, textureExit;
    sf::Font font;
    sf::Text message;
    std::vector<sf::Sprite> bestsprite;
    bool visible = false;

public:
    Popup();
    void show(const std::string& text);
    void hide();
    void draw(sf::RenderWindow& window);
    bool isVisible() { return visible; }
    PopupAction handleClick(const sf::Vector2f& mousePos);
};

class Page {
protected:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string backgroundPath;
    std::vector<SlideButton*> buttons;

public:
    Page(const std::string& bgPath = "asset/background.png");
    virtual ~Page() {
        for (auto* btn : buttons) delete btn;
        buttons.clear();
    }
    void updateTexture(const std::string& path);
    virtual void setup();
    virtual void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState);
    virtual void draw(sf::RenderWindow& window);
    virtual void update(float dt, GameState& currentState) {}
};



#pragma once
#include "GameEngine.h"

class Home : public Page {
private:
    sf::Font font;
    sf::Texture texture1, texture2, texture3;
    std::vector<sf::Text> text;
    std::vector<sf::Sprite> bestsprite;
    bool isMuted;
    SlideButton* muteButton;
    SoundManager* soundManager;
    bool wasClickedLastFrame; // Thêm để tránh bấm nhiều lần
	
	bool ignoreNextClick;
public:
    Home(SoundManager* sm);
    void setup() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
	void setIgnoreNextClick(bool value);
    void text1();
    void text2();
    void text3();
    void text4();
    void picture1();
    void picture2();
    void picture3v1();
    void picture3v2();
    void picture3v3();
    ~Home() { delete muteButton; }
};

// Tutorial class
class Tutorial : public Page {
private:
    sf::Font font;
    sf::Texture texture1, texture2, texture3, texture4;
    std::vector<sf::Text> text;
    std::vector<sf::Sprite> bestsprite;
    sf::FloatRect picture3Rect;
    sf::RenderWindow* window = nullptr;
    Menu1* menu1;
    GameState pendingState;
    bool shouldChangeState = false;

public:
    Tutorial(sf::RenderWindow* win, Menu1* m);
    void setup() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void text1();
    void text2();
    void picture1();
    void picture2();
    void picture3();
    void picture4();
    void update(float dt, GameState& currentState) override {
        if (menu1) menu1->update(dt);
        if (shouldChangeState && !menu1->isOpened()) {
            currentState = pendingState;
            shouldChangeState = false;
        }
    }
};

// Classic class
class Classic : public Page {
private:
    sf::Font font;
    sf::Texture texture1, texture2, texture3, texture4, texture5;
    std::vector<sf::Text> text;
    std::vector<sf::Sprite> bestsprite;
    sf::RenderWindow* window = nullptr;
    Menu1* menu1;
    GameState pendingState;
    bool shouldChangeState = false;

public:
    Classic(sf::RenderWindow* win, Menu1* m);
    void setup() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void text1();
    void text2();
    void text3();
    void picture1();
    void picture2();
    void picture3v1();
    void picture3v2();
    void picture4();
    void picture5();
    void update(float dt, GameState& currentState) override {
        if (menu1) menu1->update(dt);
        if (shouldChangeState && !menu1->isOpened()) {
            currentState = pendingState;
            shouldChangeState = false;
        }
    }
};

// BombModeMenu class
class BombModeMenu : public Page {
private:
    sf::Font font;
    sf::Texture texture1, texture2, texture3, texture4, texture5;
    std::vector<sf::Text> text;
    std::vector<sf::Sprite> bestsprite;
    sf::RenderWindow* window = nullptr;
    Menu1* menu1;
    GameState pendingState;
    bool shouldChangeState = false;

public:
    BombModeMenu(sf::RenderWindow* win, Menu1* m);
    void setup() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) override;
    void text1();
    void text2();
    void text3();
    void picture1();
    void picture2();
    void picture3v1();
    void picture3v2();
    void picture4();
    void picture5();
    void update(float dt, GameState& currentState) override {
        if (menu1) menu1->update(dt);
        if (shouldChangeState && !menu1->isOpened()) {
            currentState = pendingState;
            shouldChangeState = false;
        }
    }
};

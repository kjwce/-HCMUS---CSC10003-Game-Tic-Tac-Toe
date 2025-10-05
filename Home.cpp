#include "Page.h"
#include "Slidebutton.h"

Home::Home(SoundManager* sm) : Page("asset/background.png"), isMuted(false), muteButton(nullptr), soundManager(sm), ignoreNextClick(true) {
    // Không gọi playBackground ở đây, chuyển vào setup
}

void Home::setup() {
    Page::setup();
    buttons.clear();

    auto* btnHowTo = new SlideButton(HOW_TO_PLAY);
    btnHowTo->loadTextures("asset/Picture3.png", "asset/Picture8.png");
    btnHowTo->setPosition(500, 250);
    btnHowTo->setScale(0.2f, 0.2f);
    buttons.push_back(btnHowTo);

    auto* btnClassic = new SlideButton(CLASSIC_MODE);
    btnClassic->loadTextures("asset/Picture3.png", "asset/Picture8.png");
    btnClassic->setPosition(500, 400);
    btnClassic->setScale(0.2f, 0.2f);
    buttons.push_back(btnClassic);

    auto* btnBomb = new SlideButton(BOMB_MODE);
    btnBomb->loadTextures("asset/Picture3.png", "asset/Picture8.png");
    btnBomb->setPosition(500, 550);
    btnBomb->setScale(0.2f, 0.2f);
    buttons.push_back(btnBomb);

    muteButton = new SlideButton(MAIN_MENU);
    muteButton->loadTextures(isMuted ? "asset/mute.png" : "asset/unmute.png",
                            isMuted ? "asset/mute.png" : "asset/unmute.png"); 
    muteButton->setPosition(1100, 0);
    muteButton->setScale(0.15f, 0.15f);
    buttons.push_back(muteButton);
    std::cout << "Home: muteButton created at (1100, 0)\n";

    text1();
    text2();
    text3();
    text4();
    picture1();
    picture3v1();
    picture3v2();
    picture3v3();

    // Phát nhạc sau khi setup slide
    if (soundManager && !isMuted) {
        soundManager->playBackground("asset/background_music.ogg");
    }

    ignoreNextClick = true; // Đảm bảo bỏ qua click đầu tiên
    std::cout << "Home::setup hoàn tất, ignoreNextClick = " << ignoreNextClick << "\n";
}

void Home::draw(sf::RenderWindow& window) {
    std::cout << "Home::draw được gọi, vẽ main menu, ignoreNextClick = " << ignoreNextClick << "\n";
    Page::draw(window);
    for (auto& sprite : bestsprite) window.draw(sprite);
    for (auto btn : buttons) btn->draw(window);
    for (auto& text : text) window.draw(text);
}

void Home::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::MouseButtonReleased) {
        sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        std::cout << "Mouse released at: (" << mousePos.x << ", " << mousePos.y << ")\n";
        if (ignoreNextClick) {
            std::cout << "Bỏ qua sự kiện click vì ignoreNextClick = true\n";
            ignoreNextClick = false;
            return;
        }
    }
    for (auto* btn : buttons) {
        btn->update(window, event);
        if (btn->wasClicked(window, event)) {
            if (btn == muteButton) {
                isMuted = !isMuted;
                if (isMuted) {
                    soundManager->stopBackground();
                    muteButton->loadTextures("asset/mute.png", "asset/mute_hover.png");
                } else {
                    soundManager->playBackground("asset/background_music.ogg");
                    muteButton->loadTextures("asset/unmute.png", "asset/unmute_hover.png");
                }
                std::cout << "Mute button clicked, isMuted=" << isMuted << "\n";
            } else {
                currentState = btn->getTargetSlide();
                std::cout << "Nút được nhấn, chuyển sang trạng thái: " << (int)currentState << "\n";
                ignoreNextClick = true; // Bỏ qua click tiếp theo
            }
        }
    }
}

void Home::setIgnoreNextClick(bool value) {
    ignoreNextClick = value;
    std::cout << "Home::setIgnoreNextClick được gọi, ignoreNextClick = " << ignoreNextClick << "\n";
}

void Home::text1() {
    sf::Text GameName;
    if (!font.loadFromFile("asset/font/Font.otf")) {
        std::cout << "Failed to load font: asset/font/Font.otf\n";
        return;
    }
    GameName.setFont(font);
    GameName.setPosition(380, 80);
    GameName.setFillColor(sf::Color::Magenta);
    GameName.setCharacterSize(100); 
    GameName.setString("TIC TAC TOE");
    GameName.setOutlineColor({128, 0, 78});
    GameName.setOutlineThickness(5);
    sf::FloatRect bounds = GameName.getGlobalBounds();
    std::cout << "text1 bounds: (" << bounds.left << ", " << bounds.top << ", " 
              << bounds.width << ", " << bounds.height << ")\n";
    text.push_back(GameName);
}

void Home::picture1() {
    sf::Sprite s;
    if (!texture1.loadFromFile("asset/Picture1.png")) {
        std::cout << "can't create picture 1\n";
        return;
    }
    s.setTexture(texture1);
    s.setScale(0.2f, 0.2f);
    s.setPosition(100, 0);
    bestsprite.push_back(s);
}

void Home::picture2() {
    // Không cần thêm sprite vào bestsprite vì đã dùng muteButton
}

void Home::picture3v1() {
    sf::Sprite s;
    if (!texture3.loadFromFile("asset/Picture3.png")) {
        std::cout << "can't create picture 3\n";
        return;
    }
    s.setTexture(texture3);
    s.setScale(0.2f, 0.2f);
    s.setPosition(500, 250);
    bestsprite.push_back(s);
}

void Home::picture3v2() {
    sf::Sprite s;
    if (!texture3.loadFromFile("asset/Picture3.png")) {
        std::cout << "can't create picture 3\n";
        return;
    }
    s.setTexture(texture3);
    s.setScale(0.2f, 0.2f);
    s.setPosition(500, 400);
    bestsprite.push_back(s);
}

void Home::picture3v3() {
    sf::Sprite s;
    if (!texture3.loadFromFile("asset/Picture3.png")) {
        std::cout << "can't create picture 3\n";
        return;
    }
    s.setTexture(texture3);
    s.setScale(0.2f, 0.2f);
    s.setPosition(500, 550);
    bestsprite.push_back(s);
}

void Home::text2() {
    sf::Text name;
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
    }
    name.setFont(font);
    name.setPosition(530, 270);
    name.setFillColor({43, 23, 0});
    name.setCharacterSize(45);
    name.setString("How to play");
    text.push_back(name);
}

void Home::text3() {
    sf::Text name;
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
    }
    name.setFont(font);
    name.setPosition(520, 420);
    name.setFillColor({43, 23, 0});
    name.setCharacterSize(45);
    name.setString("Classic mode");
    text.push_back(name);
}

void Home::text4() {
    sf::Text name;
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
    }
    name.setFont(font);
    name.setPosition(540, 570);
    name.setFillColor({43, 23, 0});
    name.setCharacterSize(45);
    name.setString("Bomb mode");
    text.push_back(name);
}
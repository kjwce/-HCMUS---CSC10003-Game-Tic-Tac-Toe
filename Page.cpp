#include "GameEngine.h"
#include "Slidebutton.h"


Page::Page(const std::string& bgPath) : backgroundPath(bgPath) {}

void Page::updateTexture(const std::string& path) {
    if (!backgroundTexture.loadFromFile(path)) {
        std::cout << "Failed to load background: " << std::endl;
        return;
    }

    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = 1280.0f / backgroundTexture.getSize().x;
    float scaleY = 720.0f / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);
}

void Page::setup() {
    updateTexture(backgroundPath);
}

void Page::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    for (Button* btn : buttons)
        btn->draw(window);
}

void Page::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
    for (auto* btn : buttons) {
        btn->update(window, event);
        if (btn->wasClicked(window, event)) {
            auto* SlideBtn = dynamic_cast<SlideButton*>(btn);
            if (SlideBtn)
                currentState = static_cast<GameState>(SlideBtn->getTargetSlide());
        }
    }
}


#include "GameEngine.h"

Menu1::Menu1(sf::RenderWindow* win)
    : window(win), isOpen(false), menuWidth(250), slideSpeed(2000), currentX(-250), activeIndex(-1) {

    font.loadFromFile("asset/font/Pixelify.ttf");

    background.setSize({menuWidth, (float)window->getSize().y});
    background.setFillColor(sf::Color(10, 20, 30, 230));
    background.setPosition(currentX, 0);

    std::vector<std::string> labels = {"Home", "How to play", "Classic mode", "Bomb mode"};
    float y = 100;

    for (int i = 0; i < labels.size(); ++i) {
        sf::Text text(labels[i], font, 30);
        text.setFillColor(sf::Color::White);
        text.setPosition(currentX + 30, y);
        items.push_back(text);
        y += 60;
    }
}

void Menu1::open() { 
    isOpen = true; 
    if (currentX < -menuWidth) currentX = -menuWidth;
}
void Menu1::close() { 
    isOpen = false;
    currentX = -menuWidth;  
}
bool Menu1::isOpened() const { return isOpen; }

void Menu1::handleEvent(const sf::Event& event) {
    if (!isOpened()) return;

    // Đảm bảo tọa độ chuột đúng trong fullscreen
    window->setView(window->getDefaultView());

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

        for (int i = 0; i < items.size(); ++i) {
            if (items[i].getGlobalBounds().contains(mousePos)) {
                activeIndex = i;
                std::cout << "Clicked: " << items[i].getString().toAnsiString() << "\n";
            }
        }
    }
}


void Menu1::draw() {
    background.setPosition(currentX, 0);
    window->draw(background);

    for (int i = 0; i < items.size(); ++i) {
        items[i].setPosition(currentX + 30, 100 + i * 60);
        if (i == activeIndex) {
            items[i].setStyle(sf::Text::Bold | sf::Text::Underlined);
        } else {
            items[i].setStyle(sf::Text::Regular);
        }
        window->draw(items[i]);
    }
}

void Menu1::update(float dt) {
    float targetX = isOpen ? 0.0f : -menuWidth;

    if (currentX < targetX) {
        currentX += slideSpeed * dt;
        if (currentX > targetX) currentX = targetX;
    } else if (currentX > targetX) {
        currentX -= slideSpeed * dt;
        if (currentX < targetX) currentX = targetX;
    }

    background.setPosition(currentX, 0);
    for (int i = 0; i < items.size(); ++i) {
        items[i].setPosition(currentX + 30, 100 + i * 60);
    }
}

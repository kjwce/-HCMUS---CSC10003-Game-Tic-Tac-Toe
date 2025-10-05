#pragma once
#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "GameState.h"
#include <iostream>

class Button {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual bool wasClicked(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual ~Button() = default;
};

class SlideButton : public Button {
private:
    sf::Texture normalTexture;
    sf::Texture pressedTexture;
    sf::Sprite sprite;
    bool isHovered = false;
    GameState targetSlide;

public:
    SlideButton(GameState target) : targetSlide(target) {}

    void loadTextures(const std::string& normal, const std::string& pressed) {
        if (!normalTexture.loadFromFile(normal)) {
            std::cout << "Failed to load normal texture: " << normal << "\n";
        } else {
            std::cout << "Loaded normal texture: " << normal << "\n";
        }
        if (!pressedTexture.loadFromFile(pressed)) {
            std::cout << "Failed to load pressed texture: " << pressed << "\n";
        } else {
            std::cout << "Loaded pressed texture: " << pressed << "\n";
        }
        sprite.setTexture(normalTexture);
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setScale(float x, float y) {
        sprite.setScale(x, y);
    }

    void update(sf::RenderWindow& window, const sf::Event& event) override {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        isHovered = sprite.getGlobalBounds().contains(mousePos);
        sprite.setTexture(isHovered ? pressedTexture : normalTexture);
    }

    bool wasClicked(sf::RenderWindow& window, const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            sf::FloatRect bounds = sprite.getGlobalBounds();
            if (bounds.contains(mousePos)) {
                std::cout << "Button clicked at: (" << sprite.getPosition().x << ", " << sprite.getPosition().y
                          << "), bounds: (" << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << ")\n";
                return true;
            }
        }
        return false;
    }

    GameState getTargetSlide() const {
        return targetSlide;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }
};









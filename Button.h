#pragma once
#include <SFML/Graphics.hpp>

class Button {
protected:
    sf::Sprite sprite;
    sf::Texture normalTexture, pressedTexture;
    bool isPressed = false;

public:
    virtual ~Button() {}

    virtual void loadTextures(const std::string& normal, const std::string& pressed) {
        normalTexture.loadFromFile(normal);
        pressedTexture.loadFromFile(pressed);
        sprite.setTexture(normalTexture);
    }

    virtual void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    virtual void setScale(float sx, float sy) {
        sprite.setScale(sx, sy);
    }

    virtual void update(const sf::RenderWindow& window, const sf::Event& event) {
        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
        bool hover = sprite.getGlobalBounds().contains(mousePos);

        if (hover && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            isPressed = true;
        } else {
            isPressed = false;
        }

        sprite.setTexture(isPressed ? pressedTexture : normalTexture);
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    virtual bool wasClicked(const sf::RenderWindow& window, const sf::Event& event) {
        if (isPressed && event.type == sf::Event::MouseButtonReleased &&
            sprite.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
            return true;
        }
        return false;
    }
};

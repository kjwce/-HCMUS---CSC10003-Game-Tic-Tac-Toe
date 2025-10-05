// Transition.h
#pragma once
#include <SFML/Graphics.hpp>

class Transition {
private:
    sf::RectangleShape overlay;
    float alpha = 0.0f;
    bool fadingOut = true;
    bool done = false;

public:
    Transition(sf::Vector2u windowSize) {
        overlay.setSize(sf::Vector2f(windowSize));
        overlay.setFillColor(sf::Color(0, 0, 0, 0));
    }

    void start() {
        alpha = 0;
        fadingOut = true;
        done = false;
    }

    bool update() {
        if (done) return true;

        if (fadingOut) {
            alpha += 5.0f;
            if (alpha >= 255) {
                alpha = 255;
                fadingOut = false;
            }
        } else {
            alpha -= 5.0f;
            if (alpha <= 0) {
                alpha = 0;
                done = true;
            }
        }

        overlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
        return !fadingOut && done;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(overlay);
    }

    bool isDone() const { return done; }
};

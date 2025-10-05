#include "GameEngine.h"


Popup::Popup(){
    Sprite s1, s2, s3;

    font.loadFromFile("asset/font/Pixelify.ttf");
    
    if (!textureScroll.loadFromFile("asset/Picture15.png")) {
        std::cout << "Can't load Picture15.png\n";
    }
    if (!textureReplay.loadFromFile("asset/Picture16.png")) {
        std::cout << "Can't load Picture16.png\n";
    }
    if (!textureExit.loadFromFile("asset/Picture17.png")) {
        std::cout << "Can't load Picture17.png\n";
    }

    sf::Sprite scrollSprite;
    scrollSprite.setTexture(textureScroll);
    scrollSprite.setScale(0.2f, 0.2f); 
    scrollSprite.setPosition(440, 110); 
    bestsprite.push_back(scrollSprite); 

    sf::Sprite replaySprite;
    replaySprite.setTexture(textureReplay);
    replaySprite.setScale(0.2f, 0.2f); 
    replaySprite.setPosition(520, 350); 
    bestsprite.push_back(replaySprite); 

    sf::Sprite exitSprite;
    exitSprite.setTexture(textureExit);
    exitSprite.setScale(0.2f, 0.2f);
    exitSprite.setPosition(660, 350); 
    bestsprite.push_back(exitSprite); 

    message.setFont(font);
    message.setCharacterSize(40);
    message.setFillColor(sf::Color::Black);
    message.setPosition(525, 265);
}

void Popup::show(const std::string& text) {
    message.setString(text);
    visible = true;
}


void Popup::draw(sf::RenderWindow& window) {
    if (!visible) return;

    for (auto& sprite : bestsprite) {
        window.draw(sprite);
    }
    window.draw(message);
}

PopupAction Popup::handleClick(const sf::Vector2f& mousePos) {
    if (!visible) return PopupAction::NONE;

    if (bestsprite[1].getGlobalBounds().contains(mousePos)) {
        return PopupAction::REPLAY; // Picture16 = nút replay
    }

    if (bestsprite[2].getGlobalBounds().contains(mousePos)) {
        return PopupAction::HOME; // Picture17 = nút home/exit
    }

    return PopupAction::NONE;
}

void Popup::hide() {
    visible = false;
}


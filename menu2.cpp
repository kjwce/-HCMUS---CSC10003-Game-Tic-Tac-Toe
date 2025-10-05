#include"GameEngine.h"


Menu2::Menu2(sf::RenderWindow* win) : window(win), isOpen(false) {
    font.loadFromFile("asset/font/Pixelify.ttf");

    if (!panelTexture.loadFromFile("asset/Picture13.png")) {
        std::cout << "Can't load menu panel image\n";
    }


    panelSprite.setTexture(panelTexture);
    //panelSprite.setColor(sf::Color::Red);
    panelSprite.setPosition(285, 90);
    panelSprite.setScale(0.2f, 0.2f); 

    newText.setFont(font);
    newText.setString("New Game");
    newText.setCharacterSize(40);
    newText.setFillColor(sf::Color::Black);
    newText.setPosition(550, 220);

    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(40);
    resumeText.setFillColor(sf::Color::Black);
    resumeText.setPosition(570, 300);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(40);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(600, 390);
}

void Menu2::open() {
    isOpen = true;
    cout << "Menu opened\n";
}
void Menu2::close() { isOpen = false; }
bool Menu2::isOpened() const { return isOpen; }

void Menu2::handleEvent(const sf::Event& event) {
    if (!isOpen) return;

    // Reset view để tọa độ chuột đúng
    window->setView(window->getDefaultView());

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

        if (resumeText.getGlobalBounds().contains(mousePos)) {
            if (onResumeCallback) onResumeCallback();
            close();
        } else if (newText.getGlobalBounds().contains(mousePos)) {
            if (onNewGameCallback) onNewGameCallback();
            close();
        } else if (quitText.getGlobalBounds().contains(mousePos)) {
            if (onQuitCallback) onQuitCallback();
            close();
        }
    }
}


void Menu2::draw() {
    if (!isOpened()) {
        cout << "Menu draw skipped because isOpen=false\n";
        return;
    }

    cout << "Menu draw called\n";

    window->draw(panelSprite);
    window->draw(newText);
    window->draw(resumeText);
    window->draw(quitText);
}
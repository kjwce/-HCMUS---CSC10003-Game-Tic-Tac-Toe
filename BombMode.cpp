#include "Page.h"


BombModeMenu::BombModeMenu(sf::RenderWindow* win, Menu1* m) : Page("asset/background.png") {
	window = win;
    menu1 = m;
}

void BombModeMenu::setup() {
    Page::setup(); 

	auto* btnBack = new SlideButton(MAIN_MENU); // Trở về menu chính
    btnBack->loadTextures("asset/Picture51.png", "asset/Picture52.png");
    btnBack->setPosition(1180, 20);
    btnBack->setScale(0.15f, 0.15f);
    buttons.push_back(btnBack);

	SlideButton* btnMenu = new SlideButton(GameState::MAIN_MENU); // mục tiêu không đổi state
    btnMenu->loadTextures("asset/Picture4.png", "asset/Picture41.png");
    btnMenu->setScale(0.2f, 0.2f);
    btnMenu->setPosition(50, 20);
    buttons.push_back(btnMenu);

	SlideButton* btnBombPvP = new SlideButton(BOMB_BOARD);
	btnBombPvP->loadTextures("asset/Picture3.png", "asset/Picture8.png");
	btnBombPvP->setPosition(480, 330);
	btnBombPvP->setScale(0.2f, 0.2f);
	buttons.push_back(btnBombPvP);

	SlideButton* btnBombPvE = new SlideButton(AI_BOMB);
	btnBombPvE->loadTextures("asset/Picture3.png", "asset/Picture8.png");
	btnBombPvE->setPosition(480, 480);
	btnBombPvE->setScale(0.2f, 0.2f);
	buttons.push_back(btnBombPvE);

	text1();
	text2();
	text3();
	//picture1();
	//picture2();
	//picture3v1();
	//picture3v2();
	picture4();
	picture5();
}

void BombModeMenu::draw(sf::RenderWindow& window) {
    Page::draw(window);
    for (auto& sprite : bestsprite) window.draw(sprite);
	for (auto* btn : buttons) {
        btn->draw(window);
    }
	for (auto& text : text) window.draw(text);
	if (menu1 && menu1->isOpened())
        menu1->draw();
}

void BombModeMenu::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
    // 1. Nếu menu đang mở => chỉ xử lý menu
    if (menu1->isOpened()) {
        menu1->handleEvent(event);
		window.setView(window.getDefaultView());
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            for (int i = 0; i < menu1->getItemCount(); ++i) {
                if (menu1->getItemBounds(i).contains(mousePos)) {
                    // Chọn trang tương ứng
                    switch (i) {
                        case 0: currentState = GameState::MAIN_MENU; break;
                        case 1: currentState = GameState::HOW_TO_PLAY; break;
                        case 2: currentState = GameState::CLASSIC_MODE; break;
                        case 3: currentState = GameState::BOMB_MODE; break;
                    }
                    menu1->close();  // đóng menu sau khi chọn
                    return;
                }
            }

            // Nếu bấm ra ngoài menu thì đóng menu
            menu1->close();
        }

        return;  // không xử lý nút nào khác nếu menu đang mở
    }

    // 2. Nếu menu đang đóng => xử lý các nút như bình thường
    for (auto* btn : buttons) {
        btn->update(window, event);

        if (btn->wasClicked(window, event)) {
            auto* slideBtn = dynamic_cast<SlideButton*>(btn);

            if (slideBtn) {
                GameState target = slideBtn->getTargetSlide();

                if (target == GameState::MAIN_MENU && btn == buttons[1]) {
                    // Nút Picture4 → Mở menu
                    menu1->open();
                } else {
                    // Nút khác → chuyển slide
                    currentState = target;
                }

                return;  // chỉ xử lý 1 nút duy nhất
            }
        }
    }
}


void BombModeMenu::text1() {
	if (!font.loadFromFile("asset/font/Font.otf")) {

		std::cout << "Failed to load font!" << std::endl;
	}
	sf::Text name1;
	name1.setFont(font);
	name1.setPosition(370, 50);
	name1.setFillColor({ 255,102,196 });
	name1.setCharacterSize(100);
	name1.setString("You choose");
	text.push_back(name1);

	sf::Text name2;
	name2.setFont(font);
	name2.setPosition(380, 125);
	name2.setFillColor({ 255,102,196 });
	name2.setCharacterSize(100);
	name2.setString("bomb mode");
	text.push_back(name2);
}


void BombModeMenu::text2() {
	sf::Text name;
	if (!font.loadFromFile("asset/font/Font.otf")) {

		std::cout << "Failed to load font!" << std::endl;
	}
	name.setFont(font);
	name.setPosition(600, 350);
	name.setFillColor({43,23,0});
	name.setCharacterSize(50);
	name.setString("PvP");
	text.push_back(name);
}

void BombModeMenu::text3() {
	sf::Text name;
	if (!font.loadFromFile("asset/font/Pixelify.ttf")) {

		std::cout << "Failed to load font!" << std::endl;
	}
	name.setFont(font);
	name.setPosition(600, 500);
	name.setFillColor({43,23,0});
	name.setCharacterSize(50);
	name.setString("PvE");
	text.push_back(name);
}

void BombModeMenu::picture1() {
	Sprite s;
	if (!texture1.loadFromFile("asset/Picture4.png")) {
		cout << "can't crete picture 4";
		return;
	}
	s.setTexture(texture1);
	s.setScale(0.15f, 0.15f);
	s.setPosition(30, 20);
	bestsprite.push_back(s);
}

void BombModeMenu::picture2() {
	Sprite s;
	if (!texture2.loadFromFile("asset/Picture5.png")) {
		cout << "can't crete picture 5";
		return;
	}
	s.setTexture(texture2);
	s.setScale(0.1f, 0.1f);
	s.setPosition(1190, 20);
	bestsprite.push_back(s);
}

void BombModeMenu::picture3v1() {
	Sprite s;
	if (!texture3.loadFromFile("asset/Picture3.png")) {
		cout << "can't crete picture 3";
		return;
	}
	s.setTexture(texture3);
	s.setScale(0.2f, 0.2f);
	s.setPosition(480, 330);
	bestsprite.push_back(s);
}

void BombModeMenu::picture3v2() {
	Sprite s;
	if (!texture3.loadFromFile("asset/Picture3.png")) {
		cout << "can't crete picture 3";
		return;
	}
	s.setTexture(texture3);
	s.setScale(0.2f, 0.2f);
	s.setPosition(480, 480);
	bestsprite.push_back(s);
}

void BombModeMenu::picture4() {
	Sprite s;
	if (!texture4.loadFromFile("asset/Picture9.png")) {
		cout << "can't crete picture 4";
		return;
	}
	s.setTexture(texture4);
	s.setScale(0.2f, 0.2f);
	s.setPosition(850, 320);
	bestsprite.push_back(s);
}

void BombModeMenu::picture5() {
	Sprite s;
	if (!texture5.loadFromFile("asset/Picture10.png")) {
		cout << "can't crete picture 4";
		return;
	}
	s.setTexture(texture5);
	s.setScale(0.2f, 0.2f);
	s.setPosition(850, 460);
	bestsprite.push_back(s);
}


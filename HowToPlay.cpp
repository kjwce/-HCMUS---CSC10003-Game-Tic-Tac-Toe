#include "Page.h"


Tutorial::Tutorial(sf::RenderWindow* win, Menu1* m) : Page("asset/background.png") {
    window = win;
    menu1 = m;
}

void Tutorial::setup() {
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


    text1();
    text2();
    
    //picture1();
    //picture2();
    picture3();
	picture4();
}

void Tutorial::draw(sf::RenderWindow& window) {
    Page::draw(window);
    for (auto& sprite : bestsprite) window.draw(sprite);
    for (auto& text : text) window.draw(text);
    for (auto* btn : buttons) {
        btn->draw(window);
    }
    if (menu1 && menu1->isOpened())
        menu1->draw();
}

void Tutorial::handleEvent(sf::RenderWindow& window, const sf::Event& event, GameState& currentState) {
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



void Tutorial::text1() {
	sf::Text name;
	if (!font.loadFromFile("asset/font/Font.otf")) {

		std::cout << "Failed to load font!" << std::endl;
	}
	name.setFont(font);
	name.setPosition(360, 40);
	name.setFillColor({255,102,196});
	name.setCharacterSize(100);
	name.setString("How to play");
	text.push_back(name);
}

void Tutorial::text2() {
    if (!font.loadFromFile("asset/font/Pixelify.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        return;
    }

    // Classic Mode title
    sf::Text classicTitle;
    classicTitle.setFont(font);
    classicTitle.setPosition(215, 220);
    classicTitle.setFillColor({255,102,196}); 
    classicTitle.setCharacterSize(35);
    classicTitle.setString("Classic Mode");
    text.push_back(classicTitle);

    // Classic Mode description
    sf::Text classicDesc;
    classicDesc.setFont(font);
    classicDesc.setPosition(215, 260);
    classicDesc.setFillColor({43, 23, 0}); 
    classicDesc.setCharacterSize(30);
    classicDesc.setString(
        "Take turns placing X or O on the board. The first to \nconnect five in a row - across, down, or diagonally - wins!\n"
    );
    text.push_back(classicDesc);

    // Bomb Mode title
    sf::Text bombTitle;
    bombTitle.setFont(font);
    bombTitle.setPosition(215, 360);
    bombTitle.setFillColor({255,102,196}); 
    bombTitle.setCharacterSize(35);
    bombTitle.setString("Bomb Mode");
    text.push_back(bombTitle);

    // Bomb Mode description
    sf::Text bombDesc;
    bombDesc.setFont(font);
    bombDesc.setPosition(215, 400);
    bombDesc.setFillColor({43, 23, 0}); // Brown
    bombDesc.setCharacterSize(30);
    bombDesc.setString(
        "Play like Classic Mode... but be careful! Random bombs \nmay appear on the board, blocking or clearing tiles \nunexpectedly. Stay alert and adapt your strategy!\n\n"
        "Choose to play with a friend or challenge an AI opponent!\n"
        "Each player has 30 seconds to decide and make a move.\n"
    );
    text.push_back(bombDesc);

   
}

void Tutorial::picture1() {
	Sprite s;
	if (!texture1.loadFromFile("asset/Picture4.png")) {
		cout << "can't crete picture 4";
		return;
	}
	s.setTexture(texture1);
	s.setScale(0.15f, 0.15f);
	s.setPosition(40, 20);
	bestsprite.push_back(s);
}

void Tutorial::picture2() {
	Sprite s;
	if (!texture2.loadFromFile("asset/Picture5.png")) {
		cout << "can't crete picture 5";
		return;
	}
	s.setTexture(texture2);
	s.setScale(0.1f, 0.1f);
	s.setPosition(1180, 20);
	bestsprite.push_back(s);
}


void Tutorial::picture3() {
	Sprite s;
	if (!texture3.loadFromFile("asset/Picture7.png")) {
		cout << "can't crete picture 4";
		return;
	}
	s.setTexture(texture3);
	s.setScale(0.21f, 0.19f);
	s.setPosition(200, 180);
	picture3Rect = s.getGlobalBounds();
	bestsprite.push_back(s);
}

void Tutorial::picture4(){
	Sprite s;
	if (!texture4.loadFromFile("asset/Picture6.png")) {
		cout << "can't crete picture 5";
		return;
	}
	s.setTexture(texture4);
	s.setScale(0.2f, 0.2f);
	s.setPosition(1100, 400);
	bestsprite.push_back(s);
}



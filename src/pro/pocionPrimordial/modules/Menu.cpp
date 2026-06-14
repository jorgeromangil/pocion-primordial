#include "Menu.h"

Menu::Menu(sf::RenderWindow &window) {
    options[0] = "Jugar";
    options[1] = "Nosotros";
    options[2] = "Salir";

    selectedOption = 0;

    if (!font.loadFromFile("resources/AncientModernTales-a7Po.ttf")) {
        std::cerr << "Error al cargar la fuente!" << std::endl;
        exit(0);
    }

    if (!backgroundTexture.loadFromFile("resources/fondo.png")) {
    std::cerr << "Error al cargar la imagen de fondo!" << std::endl;
    exit(0);
}
backgroundSprite.setTexture(backgroundTexture);
backgroundSprite.setScale((float)window.getSize().x / backgroundTexture.getSize().x,
                          (float)window.getSize().y / backgroundTexture.getSize().y);


if (!creditsTexture.loadFromFile("resources/creditos.png")) {
    std::cerr << "Error cargando la imagen creditos.png" << std::endl;
    exit(0);
}

    for (int i = 0; i < 3; ++i) {
        text[i].setFont(font);
        text[i].setString(options[i]);
        text[i].setCharacterSize(45);
        text[i].setFillColor(sf::Color::White);
        text[i].setPosition(window.getSize().x / 2 - text[i].getGlobalBounds().width / 2,
                            window.getSize().y / 2 - 20 + i * OPTION_HEIGHT);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);
    
    for (int i = 0; i < 3; ++i) {
        text[i].setFillColor(i == selectedOption ? sf::Color::White : sf::Color(105, 77, 126));
        window.draw(text[i]);
    }
}

void Menu::navigate(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::W && selectedOption > 0)
        selectedOption--;
    else if (key == sf::Keyboard::S && selectedOption < 2)
        selectedOption++;
}

int Menu::getSelectedOption() const {
    return selectedOption;
}

void Menu::showCredits(sf::RenderWindow &window) {
    sf::Texture creditsTexture;
    if (!creditsTexture.loadFromFile("resources/creditos.png")) {
        std::cerr << "Error cargando la imagen creditos.png" << std::endl;
        return; // En caso de error, simplemente retornamos sin hacer nada
    }
    
    sf::Sprite creditsSprite(creditsTexture);

    // Escalar el sprite para que ocupe toda la ventana
    float scaleX = static_cast<float>(window.getSize().x) / creditsSprite.getLocalBounds().width;
    float scaleY = static_cast<float>(window.getSize().y) / creditsSprite.getLocalBounds().height;
    creditsSprite.setScale(scaleX, scaleY);

    window.clear();
    window.draw(creditsSprite);
    window.display();

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed) {
                return; // Salir de la función si se presiona una tecla o se cierra la ventana
            }
        }
    }
}


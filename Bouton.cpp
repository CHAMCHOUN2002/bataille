#include "Bouton.h"

Bouton::Bouton(sf::Vector2f taille, sf::Vector2f position, sf::Color couleur, const std::string& texte, sf::Font& font) {
    rect.setSize(taille);
    rect.setPosition(position);
    rect.setFillColor(couleur);

    label.setFont(font);
    label.setString(texte);
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::White);

    // Centrer le texte dans le bouton
    sf::FloatRect rectBounds = rect.getGlobalBounds();
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setPosition(
        rectBounds.left + (rectBounds.width - textBounds.width) / 2 - textBounds.left,
        rectBounds.top + (rectBounds.height - textBounds.height) / 2 - textBounds.top
    );
}

void Bouton::dessiner(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(label);
}

bool Bouton::estClique(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f souris(event.mouseButton.x, event.mouseButton.y);
        if (rect.getGlobalBounds().contains(souris)) {
            return true;
        }
    }
    return false;
}

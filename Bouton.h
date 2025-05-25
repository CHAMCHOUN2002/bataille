#ifndef BOUTON_H
#define BOUTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Bouton {
private:
    sf::RectangleShape rect;
    sf::Text label;
public:
    Bouton(sf::Vector2f taille, sf::Vector2f position, sf::Color couleur, const std::string& texte, sf::Font& font);
    void dessiner(sf::RenderWindow& window);
    bool estClique(const sf::Event& event);
};

#endif

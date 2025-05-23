#ifndef CARTE_H
#define CARTE_H

#include <string>
#include <SFML/Graphics.hpp>

class Carte {
private:
    std::string nom;
    std::string cheminImage;

    int valeur;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Carte(const std::string& nom, int valeur, const std::string& cheminImage);

    int getValeur() const;
    const std::string& getNom() const;
    const sf::Sprite& getSprite() const;
    std::string getCheminImage() const;  // AJOUTE cette ligne

};

#endif

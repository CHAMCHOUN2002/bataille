#include "Carte.h"
#include <iostream>

Carte::Carte(const std::string& nom, int valeur, const std::string& cheminImage)
    : nom(nom), valeur(valeur), cheminImage(cheminImage) {
    std::cout << "[Carte créée] " << nom << " - chemin : " << cheminImage << std::endl;
    if (!texture.loadFromFile(cheminImage)) {
        std::cerr << "Erreur chargement image: " << cheminImage << std::endl;
    }
    sprite.setTexture(texture);
}

int Carte::getValeur() const {
    return valeur;
}

const std::string& Carte::getNom() const {
    return nom;
}

const sf::Sprite& Carte::getSprite() const {
    return sprite;
}
std::string Carte::getCheminImage() const {
    return cheminImage;
}

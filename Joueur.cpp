#include "Joueur.h"
#include <iostream>

Joueur::Joueur(const std::string& nom) : nom(nom) {}
void Joueur::afficherCartes() const {
    std::cout << nom << " a les cartes : ";
    for (const auto& carte : cartes) {
        std::cout << carte.getNom() << " ";
    }
    std::cout << std::endl;
}
void Joueur::ajouterCarte(const Carte& carte) {
    cartes.push_back(carte);
}

Carte Joueur::retirerCarte() {
    if (cartes.empty()) throw std::runtime_error("Le joueur n'a plus de cartes.");
    Carte carte = cartes.front();
    cartes.pop_front();
    return carte;
}

bool Joueur::aDesCartes() const {
    return !cartes.empty();
}

int Joueur::nombreCartes() const {
    return static_cast<int>(cartes.size());
}

const std::string& Joueur::getNom() const {
    return nom;
}
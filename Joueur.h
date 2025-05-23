#ifndef JOUEUR_H
#define JOUEUR_H

#include <deque>
#include "Carte.h"

class Joueur {
private:
    std::deque<Carte> cartes;
    std::string nom;

public:
    Joueur(const std::string& nom);

    void ajouterCarte(const Carte& carte);
    Carte retirerCarte();
    bool aDesCartes() const;
    int nombreCartes() const;
    const std::string& getNom() const;
};

#endif

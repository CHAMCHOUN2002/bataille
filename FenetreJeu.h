#ifndef FENETRE_JEU_H
#define FENETRE_JEU_H

#include "Jeu.h"
#include <SFML/Graphics.hpp>

class FenetreJeu {
private:
    sf::RenderWindow window;
    Jeu jeu;

public:
    FenetreJeu();
    void bouclePrincipale();
};

#endif
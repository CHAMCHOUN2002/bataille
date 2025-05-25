#ifndef FENETRE_JEU_H
#define FENETRE_JEU_H

#include "Jeu.h"
#include <SFML/Graphics.hpp>

class FenetreJeu {
private:
    sf::RenderWindow window;
    Jeu jeu;

    sf::RectangleShape boutonQuitter;
    sf::Text texteQuitter;

    sf::RectangleShape boutonHistorique;
    sf::Text texteHistorique;

    sf::Font font;

    bool afficherPopupHistorique = false;
    int historiqueOffset = 0; // D�calage pour scroll historique

    // Boutons popup
    sf::RectangleShape boutonFermerPopup;
    sf::Text texteFermerPopup;

    sf::RectangleShape boutonFlecheHaut;
    sf::RectangleShape boutonFlecheBas;
    // m�thode priv�e pour dessiner la popup historique
  

public:
    FenetreJeu();
    void dessinerPopupHistorique(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& historique);
    void bouclePrincipale();
    void afficherCartesBataille(sf::RenderWindow& window, const std::vector<Carte*>& cartes, float posX);

};

#endif // FENETRE_JEU_H

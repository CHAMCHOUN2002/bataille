#ifndef JEU_H
#define JEU_H

#include "Joueur.h"
#include <vector>
#include <string>  // Pour std::string
#include <random>  // Pour std::default_random_engine

class Jeu {
private:
    Joueur joueur1;
    Joueur joueur2;
    std::vector<Carte> paquet;

    // Pointeurs vers les dernières cartes jouées (pour l'affichage dans l'interface)
    Carte* derniereCarteJ1 = nullptr;
    Carte* derniereCarteJ2 = nullptr;

    // Message pour décrire le résultat du tour
    std::string messageTour;

    // Méthodes internes
    void initialiserPaquet();
    void distribuerCartes();
    bool jeuTermine = false; // <== ici


public:
    Jeu();
    void jouerTour();
    // Accesseurs
    Carte* getDerniereCarteJoueur1() { return derniereCarteJ1; }
    Carte* getDerniereCarteJoueur2() { return derniereCarteJ2; }
    std::string getMessageTour() const { return messageTour; }
    //void jouerAutomatiquement(int nbTours);
	// Vérifie si la partie est finie (un joueur n'a plus de cartes)
    void jouerJusquaProchaineBataille();
    bool estDernierTourBataille() const;


    bool estFini() const;
    const Joueur& getGagnant() const;
    const Joueur& getJoueur1() const;
    const Joueur& getJoueur2() const;
};

#endif // JEU_H

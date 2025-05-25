#ifndef JEU_H
#define JEU_H

#include "Joueur.h"
#include "Carte.h"
#include <vector>
#include <string>
#include <random>

enum class EtatTour {
    DebutTour,
    BatailleCarteCachee,
    BatailleCarteVisible,
    FinBataille
};

class Jeu {
private: 
    Joueur joueur1;
    Joueur joueur2;
    std::vector<Carte> paquet;

   
    std::vector<Carte> pileBataille;

    EtatTour etatTour;

    Carte carte1;
    Carte carte2;

    Carte* derniereCarteJ1 = nullptr;
    Carte* derniereCarteJ2 = nullptr;

    std::string messageTour;
    bool jeuTermine = false;
    bool enBataille = false;

    std::vector<std::string> historique;

    void initialiserPaquet();
    void distribuerCartes();

public:
    Jeu();
    ~Jeu();

    void jouerEtape();

    Carte* getDerniereCarteJoueur1() const { return derniereCarteJ1; }
    Carte* getDerniereCarteJoueur2() const { return derniereCarteJ2; }

    std::string getMessageTour() const { return messageTour; }

    const Joueur& getGagnant() const;
    const Joueur& getJoueur1() const;
    const Joueur& getJoueur2() const;

    bool estFini() const;
    bool estEnBataille() const;
    bool estDernierTourBataille() const;
    const std::vector<Carte>& getPileBataille() const {
        return pileBataille;
    }
    std::vector<Carte*> cartesBatailleJ1;
    std::vector<Carte*> cartesBatailleJ2;
    const std::vector<Carte*>& getCartesBatailleJ1() const { return cartesBatailleJ1; }
    const std::vector<Carte*>& getCartesBatailleJ2() const { return cartesBatailleJ2; }
    void jouerJusquaProchaineBataille();


    void ajouterHistorique(const std::string& evenement) {
        historique.push_back(evenement);
    }
    const std::vector<std::string>& getHistorique() const {
        return historique;
    }

};

#endif // JEU_H

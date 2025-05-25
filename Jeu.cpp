#include "Jeu.h"
#include "Joueur.h"
#include "Carte.h"
#include <algorithm>
#include <ctime>
#include <iostream>

// Constructeur
Jeu::Jeu()
    : joueur1("Joueur 1"), joueur2("Joueur 2"),
    etatTour(EtatTour::DebutTour), jeuTermine(false), enBataille(false),
    derniereCarteJ1(nullptr), derniereCarteJ2(nullptr) {
    initialiserPaquet();
    distribuerCartes();
}

// Destructeur
Jeu::~Jeu() {
    delete derniereCarteJ1;
    delete derniereCarteJ2;
    for (Carte* c : cartesBatailleJ1) delete c;
    for (Carte* c : cartesBatailleJ2) delete c;
}

// Initialise un paquet de 52 cartes
void Jeu::initialiserPaquet() {
    std::vector<std::string> couleurs = { "clubs", "diamonds", "hearts", "spades" };
    std::vector<std::string> noms = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace" };

    for (const auto& couleur : couleurs) {
        for (size_t j = 0; j < noms.size(); ++j) {
            std::string chemin = "ressources/cards/" + noms[j] + "_of_" + couleur + ".png";
            paquet.emplace_back(noms[j], static_cast<int>(j + 2), chemin);
        }
    }

    std::shuffle(paquet.begin(), paquet.end(), std::default_random_engine(static_cast<unsigned>(time(nullptr))));
}

// Distribue les cartes alternativement
void Jeu::distribuerCartes() {
    for (size_t i = 0; i < paquet.size(); ++i) {
        (i % 2 == 0 ? joueur1 : joueur2).ajouterCarte(paquet[i]);
    }
}

// Joue une étape du jeu
void Jeu::jouerEtape() {
    switch (etatTour) {
    case EtatTour::DebutTour: {
        if (estFini()) return;

        if (!estDernierTourBataille()) {
            pileBataille.clear();

            for (Carte* c : cartesBatailleJ1) delete c;
            for (Carte* c : cartesBatailleJ2) delete c;
            cartesBatailleJ1.clear();
            cartesBatailleJ2.clear();
        }

        carte1 = joueur1.retirerCarte();
        carte2 = joueur2.retirerCarte();

        delete derniereCarteJ1;
        delete derniereCarteJ2;
        derniereCarteJ1 = new Carte(carte1);
        derniereCarteJ2 = new Carte(carte2);

        pileBataille.push_back(carte1);
        pileBataille.push_back(carte2);

        messageTour = joueur1.getNom() + " joue " + carte1.getNom() + " vs " + carte2.getNom();
        historique.push_back(messageTour);

        if (carte1.getValeur() == carte2.getValeur()) {
            enBataille = true;
            etatTour = EtatTour::BatailleCarteCachee;
            historique.push_back(" -> BATAILLE !");
        }
        else {
            etatTour = EtatTour::FinBataille;
        }
        break;
    }

    case EtatTour::BatailleCarteCachee: {
        if (joueur1.nombreCartes() < 2 || joueur2.nombreCartes() < 2) {
            jeuTermine = true;
            messageTour = "Famine ! " + (joueur1.nombreCartes() < 2 ? joueur2.getNom() : joueur1.getNom()) + " gagne.";
            historique.push_back(messageTour);
            return;
        }

        Carte carteCacheeJ1 = joueur1.retirerCarte();
        Carte carteCacheeJ2 = joueur2.retirerCarte();

        pileBataille.push_back(carteCacheeJ1);
        pileBataille.push_back(carteCacheeJ2);

        cartesBatailleJ1.push_back(new Carte(*derniereCarteJ1));
        cartesBatailleJ2.push_back(new Carte(*derniereCarteJ2));

        cartesBatailleJ1.push_back(new Carte("cachee", 0, "ressources/cards/back.png"));
        cartesBatailleJ2.push_back(new Carte("cachee", 0, "ressources/cards/back.png"));

        messageTour = "Cartes cachées posées.";
        historique.push_back(messageTour);

        etatTour = EtatTour::BatailleCarteVisible;
        break;
    }

    case EtatTour::BatailleCarteVisible: {
        carte1 = joueur1.retirerCarte();
        carte2 = joueur2.retirerCarte();

        delete derniereCarteJ1;
        delete derniereCarteJ2;
        derniereCarteJ1 = new Carte(carte1);
        derniereCarteJ2 = new Carte(carte2);

        pileBataille.push_back(carte1);
        pileBataille.push_back(carte2);

        cartesBatailleJ1.push_back(new Carte(carte1));
        cartesBatailleJ2.push_back(new Carte(carte2));

        if (carte1.getValeur() == carte2.getValeur()) {
            messageTour = "Encore une égalité ! Nouvelle bataille.";
            historique.push_back(messageTour);
            etatTour = EtatTour::BatailleCarteCachee;
        }
        else {
            etatTour = EtatTour::FinBataille;
        }
        break;
    }

    case EtatTour::FinBataille: {
        if (carte1.getValeur() > carte2.getValeur()) {
            for (const auto& c : pileBataille) joueur1.ajouterCarte(c);
            messageTour = joueur1.getNom() + " remporte les cartes !";
        }
        else {
            for (const auto& c : pileBataille) joueur2.ajouterCarte(c);
            messageTour = joueur2.getNom() + " remporte les cartes !";
        }
        historique.push_back(messageTour);

        enBataille = false;
        etatTour = EtatTour::DebutTour;
        break;
    }
    }
}

// Vérifie si le jeu est terminé
bool Jeu::estFini() const {
    return jeuTermine || !joueur1.aDesCartes() || !joueur2.aDesCartes();
}

// Renvoie le gagnant
const Joueur& Jeu::getGagnant() const {
    return joueur1.aDesCartes() ? joueur1 : joueur2;
}

const Joueur& Jeu::getJoueur1() const {
    return joueur1;
}

const Joueur& Jeu::getJoueur2() const {
    return joueur2;
}

bool Jeu::estEnBataille() const {
    return enBataille;
}

bool Jeu::estDernierTourBataille() const {
    return derniereCarteJ1 && derniereCarteJ2 &&
        derniereCarteJ1->getValeur() == derniereCarteJ2->getValeur();
}
void Jeu::jouerJusquaProchaineBataille() {
    while (!estFini()) {
        jouerEtape();
    }
}

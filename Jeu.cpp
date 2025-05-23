#include "Jeu.h"
#include <algorithm>
#include <ctime>
#include <iostream>

// Constructeur : initialise les joueurs et le paquet
Jeu::Jeu() : joueur1("Joueur 1"), joueur2("Joueur 2") {
    initialiserPaquet();
    distribuerCartes();
}

// Initialise un paquet de 52 cartes avec les bonnes images
void Jeu::initialiserPaquet() {
    std::vector<std::string> couleurs = { "clubs", "diamonds", "hearts", "spades" };
    std::vector<std::string> noms = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace" };

    for (const auto& couleur : couleurs) {
        for (size_t j = 0; j < noms.size(); ++j) {
            std::string chemin = "ressources/cards/" + noms[j] + "_of_" + couleur + ".png";
            std::cout << "Chargement de l'image : " << chemin << std::endl;
			// Vérification de l'existence du fichier (facultatif)
            paquet.emplace_back(noms[j], static_cast<int>(j + 2), chemin);
        }
    }

    // Mélange du paquet
    std::shuffle(paquet.begin(), paquet.end(), std::default_random_engine(static_cast<unsigned>(time(nullptr))));
}

// Distribue les cartes alternativement aux deux joueurs
void Jeu::distribuerCartes() {
    for (size_t i = 0; i < paquet.size(); ++i) {
        if (i % 2 == 0)
            joueur1.ajouterCarte(paquet[i]);
        else
            joueur2.ajouterCarte(paquet[i]);
    }
    std::cout << "Chargement de l'image est terminee !!!! " << std::endl;


}

void Jeu::jouerTour() {
    if (estFini()) return;

    std::vector<Carte> pileBataille;

    Carte carte1 = joueur1.retirerCarte();
    Carte carte2 = joueur2.retirerCarte();

    // Mémorise pour affichage
    derniereCarteJ1 = new Carte(carte1);
    derniereCarteJ2 = new Carte(carte2);

    pileBataille.push_back(carte1);
    pileBataille.push_back(carte2);

    while (carte1.getValeur() == carte2.getValeur()) {
        messageTour = "Égalité ! Bataille en cours...";

        // Vérifier si les joueurs peuvent continuer la bataille
        if (joueur1.nombreCartes() < 2 || joueur2.nombreCartes() < 2) {
            messageTour = "Famine ! " + (joueur1.nombreCartes() < 2 ? joueur2.getNom() : joueur1.getNom()) + " gagne la partie.";
            jeuTermine = true;
            return;
        }

        // Chaque joueur pose une carte face cachée
        pileBataille.push_back(joueur1.retirerCarte());
        pileBataille.push_back(joueur2.retirerCarte());

        // Chaque joueur pose une carte face visible
        carte1 = joueur1.retirerCarte();
        carte2 = joueur2.retirerCarte();

        // Mise à jour affichage
        derniereCarteJ1 = new Carte(carte1);
        derniereCarteJ2 = new Carte(carte2);

        pileBataille.push_back(carte1);
        pileBataille.push_back(carte2);
    }

    // Fin de la bataille : l’un gagne toutes les cartes
    if (carte1.getValeur() > carte2.getValeur()) {
        for (const Carte& c : pileBataille)
            joueur1.ajouterCarte(c);
        messageTour = joueur1.getNom() + " gagne ce tour !";
    }
    else {
        for (const Carte& c : pileBataille)
            joueur2.ajouterCarte(c);
        messageTour = joueur2.getNom() + " gagne ce tour !";
    }
}

// Vérifie si la partie est finie (un joueur n'a plus de cartes)
bool Jeu::estFini() const {
    return !joueur1.aDesCartes() || !joueur2.aDesCartes();
}

// Renvoie le gagnant (celui qui a encore des cartes)
const Joueur& Jeu::getGagnant() const {
    return joueur1.aDesCartes() ? joueur1 : joueur2;
}

const Joueur& Jeu::getJoueur1() const {
    return joueur1;
}

const Joueur& Jeu::getJoueur2() const {
    return joueur2;
}
/*void Jeu::jouerAutomatiquement(int nbTours) {
    for (int i = 0; i < nbTours && !estFini(); ++i) {
        jouerTour();
    }

    if (estFini()) {
        const Joueur& gagnant = getGagnant();
        messageTour = "?? Félicitations " + gagnant.getNom() + " ! Vous avez gagné la partie ! ??";
        jeuTermine = true;
    }
}*/
void Jeu::jouerJusquaProchaineBataille() {
    while (derniereCarteJ1 && derniereCarteJ2 && derniereCarteJ1->getValeur() != derniereCarteJ2->getValeur()) {
        jouerTour();

       
            std::cout << messageTour << std::endl;

            // Attente de l'entrée de l'utilisateur pour continuer
            char input;
            std::cin >> input;

            // Si l'utilisateur appuie sur 'Espace', continuer
            if (input == ' ') {
                continue;
            }
            else {
                break; // Arrêter si une autre touche est pressée
            }
        }
    }



bool Jeu::estDernierTourBataille() const {
    return derniereCarteJ1 && derniereCarteJ2 &&
        derniereCarteJ1->getValeur() == derniereCarteJ2->getValeur();
}

#include "FenetreJeu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

FenetreJeu::FenetreJeu() : window(sf::VideoMode(800, 600), "Jeu de la Bataille") {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // pour les feux d'artifice
}

void FenetreJeu::bouclePrincipale() {
    // Charger la police
    sf::Font font;
    if (!font.loadFromFile("ressources/fonts/arial.ttf")) {
        std::cerr << "Erreur : police non chargée\n";
        return;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                jeu.jouerTour(); // Tour manuel avec Espace
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
               // jeu.jouerAutomatiquement(1000); // Partie automatique
             std::cout << "Lancement auto jusqu'à la prochaine bataille...\n";
         //  jeu.jouerJusquaProchaineBataille();  
        }

        window.clear(sf::Color(0, 100, 0)); // fond vert foncé

        // Affichage du nombre de cartes pour chaque joueur
        sf::Text texte1(jeu.getJoueur1().getNom() + ": " + std::to_string(jeu.getJoueur1().nombreCartes()) + " cartes", font, 20);
        texte1.setPosition(50, 50);
        window.draw(texte1);

        sf::Text texte2(jeu.getJoueur2().getNom() + ": " + std::to_string(jeu.getJoueur2().nombreCartes()) + " cartes", font, 20);
        texte2.setPosition(50, 100);
        window.draw(texte2);

        // Affichage des cartes jouées
        Carte* carteJ1 = jeu.getDerniereCarteJoueur1(); // méthode à créer
        Carte* carteJ2 = jeu.getDerniereCarteJoueur2(); // méthode à créer

        if (carteJ1 && carteJ2) {
            sf::Texture texture1;
            if (texture1.loadFromFile(carteJ1->getCheminImage())) {
                sf::Sprite sprite1(texture1);
                sprite1.setScale(100.f / texture1.getSize().x, 145.f / texture1.getSize().y);
                sprite1.setPosition(200, 250);
                window.draw(sprite1);
            }
            else {
                sf::Text erreur("Image non trouvée (J1): " + carteJ1->getCheminImage(), font, 18);
                erreur.setFillColor(sf::Color::Red);
                erreur.setPosition(200, 200);
                window.draw(erreur);
            }

            sf::Texture texture2;
            if (texture2.loadFromFile(carteJ2->getCheminImage())) {
                sf::Sprite sprite2(texture2);
                sprite2.setScale(100.f / texture2.getSize().x, 145.f / texture2.getSize().y);
                sprite2.setPosition(500, 250);
                window.draw(sprite2);
            }
            else {
                sf::Text erreur("Image non trouvée (J2)", font, 18);
                erreur.setFillColor(sf::Color::Red);
                erreur.setPosition(500, 200);
                window.draw(erreur);
            }
        }

        // Message de tour ou de victoire
        std::string messageTour = jeu.getMessageTour(); // méthode à ajouter dans Jeu si pas encore faite
        sf::Text texteResultat(messageTour, font, 24);
        texteResultat.setFillColor(sf::Color::Yellow);
        texteResultat.setStyle(sf::Text::Bold);
        texteResultat.setPosition(100, 500);
        window.draw(texteResultat);

        // Feux d’artifice si la partie est finie
        if (jeu.estFini()) {
            for (int i = 0; i < 100; ++i) {
                sf::CircleShape firework(rand() % 5 + 2);
                firework.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                firework.setPosition(rand() % 800, rand() % 600);
                window.draw(firework);
            }
        }

        window.display();
    }
}

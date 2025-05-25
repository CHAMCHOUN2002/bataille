#include "FenetreJeu.h"
#include "Bouton.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// scrollOffset global pour le défilement dans la popup
float scrollOffset = 0.f;

void FenetreJeu::dessinerPopupHistorique(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& historique) {
    sf::RectangleShape fond(sf::Vector2f(600, 400));
    fond.setFillColor(sf::Color(0, 0, 0, 200));  // fond semi-transparent noir
    fond.setPosition(100, 100);
    window.draw(fond);

    sf::Text titre("Historique du jeu", font, 24);
    titre.setFillColor(sf::Color::Yellow);
    titre.setPosition(300, 110);
    window.draw(titre);

    float posY = 150.f - scrollOffset;  // appliquer le décalage scroll ici

    // hauteur visible = 360 (400 - marge), on limite l'affichage en fonction de posY
    for (const auto& ligne : historique) {
        if (posY > 120 && posY < 480) {  // afficher seulement dans la zone visible
            sf::Text texte(ligne, font, 18);
            texte.setFillColor(sf::Color::White);
            texte.setPosition(120, posY);
            window.draw(texte);
        }
        posY += 25.f;
    }
}

FenetreJeu::FenetreJeu() : window(sf::VideoMode(800, 600), "Jeu de la Bataille") {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // pour les feux d'artifice
}
void FenetreJeu::afficherCartesBataille(sf::RenderWindow& window,
    const std::vector<Carte*>& cartes,
    float posX) {
    float posY = 200; // Position de départ verticale
    float decalageY = 30; // Décalage entre chaque carte

    for (size_t i = 0; i < cartes.size(); ++i) {
        sf::Texture texture;
        if (cartes[i] != nullptr && texture.loadFromFile(cartes[i]->getCheminImage())) {
            sf::Sprite sprite(texture);
            sprite.setPosition(posX, posY + i * decalageY);
            sprite.setScale(0.5f, 0.5f);
            window.draw(sprite);
        }
    }
}

void FenetreJeu::bouclePrincipale() {
    // Charger la police
    sf::Font font;
    if (!font.loadFromFile("ressources/fonts/arial.ttf")) {
        std::cerr << "Erreur : police non chargée\n";
        return;
    }

    Bouton boutonQuitter({ 150, 50 }, { 630, 20 }, sf::Color::Red, "Quitter", font);
    Bouton boutonHistorique({ 150, 50 }, { 460, 20 }, sf::Color(50, 150, 250), "Historique", font);

    bool popupVisible = false;  // popup masqué au départ
    scrollOffset = 0.f;         // reset scroll

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Fermer popup avec Echap
            else if (popupVisible && event.key.code == sf::Keyboard::Escape) {
                popupVisible = false;
                scrollOffset = 0.f;  // reset scroll quand popup fermée
            }
            else if (popupVisible) {
                // Gestion du scroll molette
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        scrollOffset -= event.mouseWheelScroll.delta * 30;
                        if (scrollOffset < 0) scrollOffset = 0;

                        float hauteurContenu = jeu.getHistorique().size() * 25.f;
                        float maxScroll = hauteurContenu - 360.f;
                        if (maxScroll < 0) maxScroll = 0;

                        if (scrollOffset > maxScroll) scrollOffset = maxScroll;

                        std::cout << "Molette scrollOffset: " << scrollOffset << ", maxScroll: " << maxScroll << std::endl;
                    }
                }

                // Gestion scroll clavier (flèches haut/bas)
                else if (event.type == sf::Event::KeyPressed) {
                    float hauteurContenu = jeu.getHistorique().size() * 25.f;
                    float maxScroll = hauteurContenu - 360.f;
                    if (maxScroll < 0) maxScroll = 0;

                    if (event.key.code == sf::Keyboard::Up) {
                        scrollOffset -= 30;
                        if (scrollOffset < 0) scrollOffset = 0;
                        std::cout << "Clavier UP scrollOffset: " << scrollOffset << std::endl;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        scrollOffset += 30;
                        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
                        std::cout << "Clavier DOWN scrollOffset: " << scrollOffset << std::endl;
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::A) {
                std::cout << "Lancement auto jusqu'à la prochaine bataille...\n";
                jeu.jouerJusquaProchaineBataille();
            }
            else {
                // Gestion touches clavier quand popup fermée
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        jeu.jouerEtape();
                    }
                    
                }
                // Gestion clics sur boutons
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (boutonQuitter.estClique(event)) {
                        window.close();
                    }
                    else if (boutonHistorique.estClique(event)) {
                        popupVisible = true;
                        scrollOffset = 0.f;  // reset scroll à l'ouverture
                    }
                }
            }
        }

        // Effacer la fenêtre avec un fond vert foncé
        window.clear(sf::Color(0, 100, 0));

        // Afficher infos joueurs (nombre cartes)
        sf::Text texteJ1(jeu.getJoueur1().getNom() + ": " + std::to_string(jeu.getJoueur1().nombreCartes()) + " cartes", font, 20);
        texteJ1.setPosition(50, 50);
        window.draw(texteJ1);

        sf::Text texteJ2(jeu.getJoueur2().getNom() + ": " + std::to_string(jeu.getJoueur2().nombreCartes()) + " cartes", font, 20);
        texteJ2.setPosition(50, 100);
        window.draw(texteJ2);

        // Afficher les cartes jouées
        Carte* carteJ1 = jeu.getDerniereCarteJoueur1();
        Carte* carteJ2 = jeu.getDerniereCarteJoueur2();
        // Affichage des cartes en cas de bataille
       // Affichage des cartes en cas de bataille
        if (!jeu.getCartesBatailleJ1().empty() && !jeu.getCartesBatailleJ2().empty()) {
            window.clear(sf::Color(255, 102, 102));  // Rouge clair (RGB)
			sf::Text texteBataille("Bataille en cours !", font, 24);
            const std::vector<Carte*>& cartesBatailleJ1 = jeu.getCartesBatailleJ1();
            const std::vector<Carte*>& cartesBatailleJ2 = jeu.getCartesBatailleJ2();

            for (size_t i = 0; i < cartesBatailleJ1.size(); ++i) {
                sf::Texture texture1;
                if (texture1.loadFromFile(cartesBatailleJ1[i]->getCheminImage())) {
                    sf::Sprite sprite1(texture1);
                    sprite1.setScale(100.f / texture1.getSize().x, 145.f / texture1.getSize().y);
                    sprite1.setPosition(200 + i * 30, 250); // à gauche
                    window.draw(sprite1);
                }
            }

            for (size_t i = 0; i < cartesBatailleJ2.size(); ++i) {
                sf::Texture texture2;
                if (texture2.loadFromFile(cartesBatailleJ2[i]->getCheminImage())) {
                    sf::Sprite sprite2(texture2);
                    sprite2.setScale(100.f / texture2.getSize().x, 145.f / texture2.getSize().y);
                    sprite2.setPosition(500 + i * 30, 250); // à droite
                    window.draw(sprite2);
                }
            }
        }

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

        // Afficher message de tour ou victoire
        sf::Text texteResultat(jeu.getMessageTour(), font, 24);
        texteResultat.setFillColor(sf::Color::Yellow);
        texteResultat.setStyle(sf::Text::Bold);
        texteResultat.setPosition(100, 500);
        window.draw(texteResultat);

        // Feux d’artifice si partie finie
        if (jeu.estFini()) {
            for (int i = 0; i < 100; ++i) {
                sf::CircleShape firework(rand() % 5 + 2);
                firework.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                firework.setPosition(rand() % 800, rand() % 600);
                window.draw(firework);
            }
        }

        // Si popup visible, on affiche la popup
        if (popupVisible) {
            dessinerPopupHistorique(window, font, jeu.getHistorique());
        }
        else {
            boutonQuitter.dessiner(window);
            boutonHistorique.dessiner(window);
        }

        window.display();
    }
}

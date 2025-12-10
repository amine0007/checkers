#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> // Nécessaire pour abs() (valeur absolue)

const int TILE_SIZE = 80;

// Fonction pour vérifier si un mouvement est autorisé
bool mouvementValide(int x1, int y1, int x2, int y2, int plateau[8][8], int joueurActuel) {
    
    // 1. On ne peut aller que sur une case vide
    if (plateau[y2][x2] != 0) return false;

    // 2. Calcul des écarts
    int diffX = abs(x2 - x1);
    int diffY = y2 - y1; // Pas de abs() ici car le sens compte !

    // 3. Vérification pour les BLANCS (1) : Doivent monter (diffY négatif)
    if (joueurActuel == 1) {
        if (diffY == -1 && diffX == 1) return true; // Déplacement simple
    }
    
    // 4. Vérification pour les ROUGES (2) : Doivent descendre (diffY positif)
    if (joueurActuel == 2) {
        if (diffY == 1 && diffX == 1) return true; // Déplacement simple
    }

    // Si aucune condition n'est remplie
    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Jeu de Dames - Regles de base");

    // 0 = Vide, 1 = Blanc, 2 = Rouge
    int plateau[8][8] = {
        {0, 2, 0, 2, 0, 2, 0, 2},
        {2, 0, 2, 0, 2, 0, 2, 0},
        {0, 2, 0, 2, 0, 2, 0, 2},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0}
    };

    sf::Vector2i selection(-1, -1);
    
    // Variable pour savoir à qui c'est le tour (1 = Blanc commence)
    int tourActuel = 1; 

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = event.mouseButton.x / TILE_SIZE;
                    int y = event.mouseButton.y / TILE_SIZE;

                    if (x >= 0 && x < 8 && y >= 0 && y < 8)
                    {
                        // SÉLECTION
                        if (selection.x == -1) 
                        {
                            // On vérifie que le joueur clique bien sur SA couleur
                            if (plateau[y][x] == tourActuel) {
                                selection = sf::Vector2i(x, y);
                                std::cout << "Pion selectionne" << std::endl;
                            } else if (plateau[y][x] != 0) {
                                std::cout << "Ce n'est pas votre tour !" << std::endl;
                            }
                        }
                        // DÉPLACEMENT
                        else 
                        {
                            // Si on clique sur la même case, on désélectionne
                            if (x == selection.x && y == selection.y) {
                                selection = sf::Vector2i(-1, -1);
                            }
                            // Sinon on tente le mouvement
                            else if (mouvementValide(selection.x, selection.y, x, y, plateau, tourActuel)) {
                                
                                // On déplace le pion
                                plateau[y][x] = plateau[selection.y][selection.x];
                                plateau[selection.y][selection.x] = 0;
                                
                                // Fin du tour : on change de joueur
                                if (tourActuel == 1) tourActuel = 2;
                                else tourActuel = 1;

                                selection = sf::Vector2i(-1, -1); // Désélectionner
                                std::cout << "Mouvement valide ! Au tour de " << tourActuel << std::endl;
                            }
                            else {
                                std::cout << "Mouvement IMPOSSIBLE (Regles non respectees)" << std::endl;
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        // --- DESSIN ---
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                
                sf::RectangleShape caseCarre(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                caseCarre.setPosition(x * TILE_SIZE, y * TILE_SIZE);

                if ((x + y) % 2 == 0) caseCarre.setFillColor(sf::Color(238, 238, 210));
                else caseCarre.setFillColor(sf::Color(118, 150, 86));
                window.draw(caseCarre);

                // Surlignage
                if (selection.x == x && selection.y == y) {
                    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    highlight.setFillColor(sf::Color(0, 0, 255, 100));
                    window.draw(highlight);
                }

                // Pions
                if (plateau[y][x] != 0) {
                    sf::CircleShape pion(TILE_SIZE / 2 - 10);
                    pion.setPosition(x * TILE_SIZE + 10, y * TILE_SIZE + 10);
                    
                    if (plateau[y][x] == 1) pion.setFillColor(sf::Color::White);
                    else pion.setFillColor(sf::Color(200, 0, 0));
                    
                    window.draw(pion);
                }
            }
        }
        window.display();
    }
    return 0;
}
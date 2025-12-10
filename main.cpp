#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 

const int TILE_SIZE = 80;

// Fonction mise à jour pour gérer les sauts
bool mouvementValide(int x1, int y1, int x2, int y2, int plateau[8][8], int joueurActuel) {
    
    // 1. Destination doit être vide
    if (plateau[y2][x2] != 0) return false;

    // 2. Calcul des écarts
    int diffX = x2 - x1;
    int diffY = y2 - y1; 

    // Direction autorisée (1 = vers le haut (-), 2 = vers le bas (+))
    int direction = (joueurActuel == 1) ? -1 : 1;

    // --- CAS 1 : DÉPLACEMENT SIMPLE (1 case) ---
    if (abs(diffX) == 1 && diffY == direction) {
        return true;
    }

    // --- CAS 2 : MANGER (2 cases) ---
    if (abs(diffX) == 2 && diffY == 2 * direction) {
        // On doit vérifier qu'il y a un ennemi au milieu
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int contenuMilieu = plateau[midY][midX];

        // L'ennemi est le chiffre opposé (Si je suis 1, ennemi est 2. Si je suis 2, ennemi est 1)
        int ennemi = (joueurActuel == 1) ? 2 : 1;

        if (contenuMilieu == ennemi) {
            return true; // Saut autorisé !
        }
    }

    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Jeu de Dames - Prise de pions");

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
                            if (plateau[y][x] == tourActuel) {
                                selection = sf::Vector2i(x, y);
                            }
                        }
                        // DÉPLACEMENT
                        else 
                        {
                            if (x == selection.x && y == selection.y) {
                                selection = sf::Vector2i(-1, -1); // Annuler
                            }
                            else if (mouvementValide(selection.x, selection.y, x, y, plateau, tourActuel)) {
                                
                                // --- NOUVEAU : GESTION DE LA PRISE ---
                                // Si on a bougé de 2 cases, c'est qu'on a mangé quelqu'un
                                if (abs(x - selection.x) == 2) {
                                    int midX = (x + selection.x) / 2;
                                    int midY = (y + selection.y) / 2;
                                    plateau[midY][midX] = 0; // On supprime le pion mangé !
                                    std::cout << "MIAM ! Pion mange en " << midX << "," << midY << std::endl;
                                }

                                // Déplacement standard
                                plateau[y][x] = plateau[selection.y][selection.x];
                                plateau[selection.y][selection.x] = 0;
                                
                                // Changement de tour
                                tourActuel = (tourActuel == 1) ? 2 : 1;
                                selection = sf::Vector2i(-1, -1);
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

                if (selection.x == x && selection.y == y) {
                    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    highlight.setFillColor(sf::Color(0, 0, 255, 100));
                    window.draw(highlight);
                }

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
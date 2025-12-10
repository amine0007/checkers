#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 
#include <vector>

const int TILE_SIZE = 80;

// --- FONCTIONS UTILITAIRES ---

// Vérifie si une pièce appartient au joueur actuel
// Joueur 1 (Blanc) possède les pièces 1 (Pion) et 3 (Dame)
// Joueur 2 (Rouge) possède les pièces 2 (Pion) et 4 (Dame)
bool estPieceDuJoueur(int piece, int joueur) {
    if (piece == 0) return false;
    if (joueur == 1) return (piece == 1 || piece == 3);
    if (joueur == 2) return (piece == 2 || piece == 4);
    return false;
}

// Vérifie si c'est une Dame (3 ou 4)
bool estDame(int piece) {
    return (piece == 3 || piece == 4);
}

// Logique de mouvement complète
bool mouvementValide(int x1, int y1, int x2, int y2, int plateau[8][8], int joueurActuel) {
    
    // 1. Destination doit être vide
    if (plateau[y2][x2] != 0) return false;

    int piece = plateau[y1][x1];
    int diffX = x2 - x1;
    int diffY = y2 - y1; 
    
    // Sens obligatoire pour les PIONS seulement
    // Blanc (1) monte (-1), Rouge (2) descend (+1)
    int sensPion = (joueurActuel == 1) ? -1 : 1;

    // --- CAS A : DÉPLACEMENT SIMPLE (1 case) ---
    if (abs(diffX) == 1) {
        if (estDame(piece)) {
            // La Dame peut aller en haut ou en bas, tant que c'est 1 case diagonale
            if (abs(diffY) == 1) return true; 
        } else {
            // Le Pion doit respecter son sens
            if (diffY == sensPion) return true;
        }
    }

    // --- CAS B : MANGER (2 cases) ---
    if (abs(diffX) == 2) {
        // Pour manger, il faut bouger de 2 cases en Y aussi
        // Si c'est un pion, sens obligatoire. Si c'est une Dame, peu importe le sens (+2 ou -2)
        bool directionOk = false;
        
        if (estDame(piece)) {
            if (abs(diffY) == 2) directionOk = true;
        } else {
            if (diffY == 2 * sensPion) directionOk = true;
        }

        if (directionOk) {
            // Vérifier le pion au milieu
            int midX = (x1 + x2) / 2;
            int midY = (y1 + y2) / 2;
            int pieceMilieu = plateau[midY][midX];

            // Si la case du milieu n'est pas vide et n'est pas à moi, c'est un ennemi
            if (pieceMilieu != 0 && !estPieceDuJoueur(pieceMilieu, joueurActuel)) {
                return true;
            }
        }
    }

    return false;
}

// Compter les pions restants pour vérifier la victoire
int compterPions(int plateau[8][8], int joueur) {
    int count = 0;
    for(int y=0; y<8; y++) {
        for(int x=0; x<8; x++) {
            if (estPieceDuJoueur(plateau[y][x], joueur)) count++;
        }
    }
    return count;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Jeu de Dames - FINAL");

    // 1=Pion Blanc, 2=Pion Rouge, 3=Dame Blanche, 4=Dame Rouge
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
    bool finDuJeu = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Gestion du clic (seulement si le jeu n'est pas fini)
            if (!finDuJeu && event.type == sf::Event::MouseButtonPressed)
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
                            if (estPieceDuJoueur(plateau[y][x], tourActuel)) {
                                selection = sf::Vector2i(x, y);
                            }
                        }
                        // DÉPLACEMENT
                        else 
                        {
                            if (x == selection.x && y == selection.y) {
                                selection = sf::Vector2i(-1, -1);
                            }
                            else if (mouvementValide(selection.x, selection.y, x, y, plateau, tourActuel)) {
                                
                                // GESTION MANGER
                                if (abs(x - selection.x) == 2) {
                                    int midX = (x + selection.x) / 2;
                                    int midY = (y + selection.y) / 2;
                                    plateau[midY][midX] = 0; 
                                }

                                // DÉPLACEMENT EFFECTIF
                                plateau[y][x] = plateau[selection.y][selection.x];
                                plateau[selection.y][selection.x] = 0;

                                // --- PROMOTION EN DAME ---
                                // Si Blanc arrive en haut (y=0) -> Devient 3
                                if (tourActuel == 1 && y == 0) {
                                    plateau[y][x] = 3;
                                    std::cout << "PROMOTION ! Les Blancs ont une Dame !" << std::endl;
                                }
                                // Si Rouge arrive en bas (y=7) -> Devient 4
                                else if (tourActuel == 2 && y == 7) {
                                    plateau[y][x] = 4;
                                    std::cout << "PROMOTION ! Les Rouges ont une Dame !" << std::endl;
                                }

                                // CHANGEMENT DE TOUR
                                tourActuel = (tourActuel == 1) ? 2 : 1;
                                selection = sf::Vector2i(-1, -1);

                                // VÉRIFICATION VICTOIRE
                                int pionsBlancs = compterPions(plateau, 1);
                                int pionsRouges = compterPions(plateau, 2);

                                if (pionsBlancs == 0) {
                                    std::cout << "VICTOIRE DES ROUGES !" << std::endl;
                                    finDuJeu = true;
                                } else if (pionsRouges == 0) {
                                    std::cout << "VICTOIRE DES BLANCS !" << std::endl;
                                    finDuJeu = true;
                                }
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
                
                // 1. Cases
                sf::RectangleShape caseCarre(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                caseCarre.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                if ((x + y) % 2 == 0) caseCarre.setFillColor(sf::Color(238, 238, 210));
                else caseCarre.setFillColor(sf::Color(118, 150, 86));
                window.draw(caseCarre);

                // 2. Sélection
                if (selection.x == x && selection.y == y) {
                    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    highlight.setFillColor(sf::Color(0, 0, 255, 100));
                    window.draw(highlight);
                }

                // 3. Pions
                int piece = plateau[y][x];
                if (piece != 0) {
                    sf::CircleShape pion(TILE_SIZE / 2 - 10);
                    pion.setPosition(x * TILE_SIZE + 10, y * TILE_SIZE + 10);
                    
                    // Couleur de base
                    if (estPieceDuJoueur(piece, 1)) pion.setFillColor(sf::Color::White);
                    else pion.setFillColor(sf::Color(200, 0, 0));
                    
                    window.draw(pion);

                    // SI C'EST UNE DAME (3 ou 4) : On dessine une couronne (cercle doré au milieu)
                    if (estDame(piece)) {
                        sf::CircleShape couronne(TILE_SIZE / 4);
                        couronne.setOrigin(couronne.getRadius(), couronne.getRadius()); // Centrer
                        couronne.setPosition(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2);
                        couronne.setFillColor(sf::Color(255, 215, 0)); // Or (Gold)
                        window.draw(couronne);
                    }
                }
            }
        }
        window.display();
    }
    return 0;
}
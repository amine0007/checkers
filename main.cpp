#include <SFML/Graphics.hpp>
#include <vector>

// Taille d'une case en pixels
const int TILE_SIZE = 80;

int main()
{
    // La fenêtre fera 8 cases de large * 80 pixels = 640x640
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Jeu de Dames C++");

    // 1. DÉFINITION DU PLATEAU (0 = Vide, 1 = Blanc, 2 = Rouge)
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); // Effacer l'écran

        // --- DESSIN DU JEU ---

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                
                // A. DESSINER LA CASE (LE DAMIER)
                sf::RectangleShape caseCarre(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                caseCarre.setPosition(x * TILE_SIZE, y * TILE_SIZE);

                // Si la somme de x et y est paire, c'est une case claire, sinon sombre
                if ((x + y) % 2 == 0)
                    caseCarre.setFillColor(sf::Color(238, 238, 210)); // Couleur Crème
                else
                    caseCarre.setFillColor(sf::Color(118, 150, 86));  // Couleur Verte/Bois

                window.draw(caseCarre);

                // B. DESSINER LE PION (S'il y en a un)
                if (plateau[y][x] != 0) {
                    sf::CircleShape pion(TILE_SIZE / 2 - 10); // Rayon un peu plus petit que la case
                    // Centrer le pion dans la case
                    pion.setPosition(x * TILE_SIZE + 10, y * TILE_SIZE + 10);

                    if (plateau[y][x] == 1) 
                        pion.setFillColor(sf::Color::White); // Pions du bas
                    else 
                        pion.setFillColor(sf::Color(200, 0, 0)); // Pions du haut (Rouge)
                    
                    window.draw(pion);
                }
            }
        }

        window.display(); // Afficher le tout
    }

    return 0;
}
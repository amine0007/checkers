#include <SFML/Graphics.hpp>
#include <iostream>

const int TILE_SIZE = 80;

int main()
{
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Jeu de Dames - Interaction");

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

    // Variables pour mémoriser la sélection
    // -1 signifie "Aucune case sélectionnée"
    sf::Vector2i selection(-1, -1);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // --- GESTION DU CLIC SOURIS ---
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // 1. On convertit les Pixels en coordonnée de grille (0-7)
                    int x = event.mouseButton.x / TILE_SIZE;
                    int y = event.mouseButton.y / TILE_SIZE;

                    // Vérification de sécurité (pour ne pas cliquer hors du tableau)
                    if (x >= 0 && x < 8 && y >= 0 && y < 8)
                    {
                        // CAS A : Rien n'est sélectionné -> On sélectionne
                        if (selection.x == -1) 
                        {
                            // On ne sélectionne que s'il y a un pion (case != 0)
                            if (plateau[y][x] != 0) {
                                selection = sf::Vector2i(x, y);
                                std::cout << "Pion selectionne en : " << x << ", " << y << std::endl;
                            }
                        }
                        // CAS B : Un pion est DÉJÀ sélectionné -> On bouge
                        else 
                        {
                            // Si on clique sur la case vide, on déplace
                            if (plateau[y][x] == 0) {
                                // Déplacement : La case d'arrivée prend la valeur du pion
                                plateau[y][x] = plateau[selection.y][selection.x];
                                // La case de départ devient vide
                                plateau[selection.y][selection.x] = 0;
                                
                                std::cout << "Deplacement vers : " << x << ", " << y << std::endl;
                                selection = sf::Vector2i(-1, -1); // On désélectionne
                            }
                            // Si on clique sur un autre pion ou le même -> On change la sélection
                            else if (plateau[y][x] != 0) {
                                selection = sf::Vector2i(x, y);
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

                // Couleur du damier
                if ((x + y) % 2 == 0) caseCarre.setFillColor(sf::Color(238, 238, 210));
                else caseCarre.setFillColor(sf::Color(118, 150, 86));

                window.draw(caseCarre);

                // SURBRILLANCE : Si cette case est celle sélectionnée, on ajoute un filtre bleu
                if (selection.x == x && selection.y == y) {
                    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    highlight.setFillColor(sf::Color(0, 0, 255, 100)); // Bleu transparent
                    window.draw(highlight);
                }

                // DESSIN DU PION
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
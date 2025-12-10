#include <SFML/Graphics.hpp>

int main()
{
    // Crée une fenêtre de 800x600 pixels
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mon Jeu de Dames");

    // Crée un pion (un cercle rouge)
    sf::CircleShape pion(40.f); // Rayon de 40
    pion.setFillColor(sf::Color::Red);
    pion.setPosition(100.f, 100.f); // Position X, Y

    // Boucle principale du jeu (tant que la fenêtre est ouverte)
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Si on clique sur la croix, on ferme
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 1. Effacer l'écran (en noir par défaut)
        window.clear();

        // 2. Dessiner les objets
        window.draw(pion);

        // 3. Afficher le tout
        window.display();
    }

    return 0;
}
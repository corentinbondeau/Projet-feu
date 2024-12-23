#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

// Dimensions de la fenêtre
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Vitesse des voitures
const float CAR_SPEED = 0.3f;

// Durées des états des feux
const sf::Time GREEN_DURATION = sf::seconds(5);
const sf::Time ORANGE_DURATION = sf::seconds(2);
const sf::Time RED_DURATION = sf::seconds(7); // Inclut le temps d'orange d'un autre feu

// Énumération pour les états des feux
enum LightState { GREEN, ORANGE, RED };

// Classe Feu de circulation
class TrafficLight {
public:
    sf::RectangleShape lightShape;
    LightState state;

    TrafficLight(float x, float y) {
        lightShape = sf::RectangleShape(sf::Vector2f(20, 20));
        lightShape.setPosition(x, y);
        state = RED;
        updateColor();
    }

    void setState(LightState newState) {
        state = newState;
        updateColor();
    }

    void updateColor() {
        if (state == GREEN) {
            lightShape.setFillColor(sf::Color::Green);
        }
        else if (state == ORANGE) {
            lightShape.setFillColor(sf::Color(255, 165, 0)); // Couleur orange
        }
        else {
            lightShape.setFillColor(sf::Color::Red);
        }
    }

    bool isRedOrOrange() const {
        return state == RED || state == ORANGE;
    }

    bool isGreen() const {
        return state == GREEN;
    }

    bool isOrange() const {
        return state == ORANGE;
    }
};

class RandomGenerator {
private:
    std::mt19937 generator; // Générateur pseudo-aléatoire
public:
    // Constructeur pour initialiser le générateur avec un seed aléatoire
    RandomGenerator() : generator(std::random_device{}()) {}

    // Méthode pour générer un entier dans une plage donnée
    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<int> distrib(min, max);
        return distrib(generator);
    }

    // Méthode pour générer un nombre réel dans une plage donnée
    double getRandomDouble(double min, double max) {
        std::uniform_real_distribution<double> distrib(min, max);
        return distrib(generator);
    }
};

// Classe Voiture
class Car {

public:
    sf::RectangleShape shape;
    float speed;
    bool comingFromTop;  // Si la voiture vient du haut (ou d'une autre direction)
    bool comingFromBottom;
    bool comingFromLeft;
    bool comingFromRight;
    int nbRandom;
    RandomGenerator randGen; // RandomGenerator comme membre privé
    std::string direction; // Paramètre pour la direction (tout droit, droite, gauche)

    Car(float x, float y, float width, float height, float speed, sf::Color color, bool top, bool bottom, bool left, bool right)
        : comingFromTop(top), comingFromBottom(bottom), comingFromLeft(left), comingFromRight(right), direction("tout droit") {
        this->shape = sf::RectangleShape(sf::Vector2f(width, height));
        this->shape.setPosition(x, y);
        this->speed = speed;
        this->shape.setFillColor(color);
    }

    bool checkCollision(const Car& other) {
        // Vérifie si les voitures se touchent (collision simple sur les bords)
        return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
    }

    void setDirection(const std::string& newDirection) {
        if (newDirection == "tout droit" || newDirection == "droite" || newDirection == "gauche") {
            direction = newDirection;
            std::cout << "Direction changée : " << direction << std::endl;
        }
        else {
            std::cout << "Direction invalide. Choisissez parmi : tout droit, droite, gauche." << std::endl;
        }
    }

    void move(bool horizontal, const std::vector<TrafficLight>& lights, const std::vector<Car>& cars) {
        bool canMove = true;
        float currentSpeed = speed;

        // Vérifier uniquement le feu correspondant à la direction de la voiture
        for (const auto& light : lights) {

            // Déclaration de variables nécessaires
            float targetRotation = 0.0f; // Rotation cible de la voiture
            float rotationSpeed = 2.0f;  // Vitesse de rotation progressive

            // Si la voiture vient de la gauche sur une trajectoire horizontale
            if (comingFromLeft && horizontal) {
                // Vérification du feu de circulation (position et état rouge/orange)
                if (light.lightShape.getPosition().x == 130 && light.isRedOrOrange()) { // Feu de gauche
                    if (shape.getPosition().x < 150) {
                        canMove = false; // Arrêter la voiture
                        if (light.isOrange()) {
                            currentSpeed = speed / 3;  // Réduire la vitesse si le feu est orange
                        }
                    }
                    return; // Sortir de la condition, inutile de continuer plus loin
                }

                // Vérification si la voiture a dépassé le feu
                if (shape.getPosition().x > 270) {
                    canMove = true;  // Permettre le mouvement

                    // Générer un nombre aléatoire pour déterminer la direction
                    int nbRandom = randGen.getRandomInt(0, 2);

                    // Déterminer l'action en fonction de la valeur aléatoire
                    switch (nbRandom) {
                    case 0: // Tourner à gauche
                        horizontal = false;            // Passer à une trajectoire verticale
                        targetRotation = 90.0f;        // Rotation cible de 90°
                        setDirection("gauche");
                        break;

                    case 1: // Tourner à droite
                        horizontal = false;            // Passer à une trajectoire verticale
                        targetRotation = -90.0f;       // Rotation cible de -90°
                        setDirection("droite");
                        break;

                    case 2: // Continuer tout droit
                        currentSpeed = speed;          // Garder la vitesse actuelle
                        setDirection("tout droit");
                        break;
                    }

                }
            }

            // Voiture venant du haut (regarde le feu du haut)
            else if (comingFromTop && !horizontal) {
                if (light.lightShape.getPosition().y == 130 && light.isRedOrOrange()) { // Feu du haut
                    if (shape.getPosition().y < 150) {
                        canMove = false;
                        if (light.isOrange()) {
                            currentSpeed = speed / 3;  // Réduit la vitesse si le feu est orange
                        }
                    }
                    break;
                }
            }
            // Voiture venant du bas (regarde le feu du bas)
            else if (comingFromBottom && !horizontal) {
                if (light.lightShape.getPosition().y == 650 && light.isRedOrOrange()) { // Feu du bas
                    if (shape.getPosition().y > 550) {
                        canMove = false;
                        if (light.isOrange()) {
                            currentSpeed = speed / 3;  // Réduit la vitesse si le feu est orange
                        }
                    }
                    break;
                }
            }
            // Voiture venant de la droite (regarde le feu de droite)
            else if (comingFromRight && horizontal) {
                if (light.lightShape.getPosition().x == 650 && light.isRedOrOrange()) { // Feu de droite
                    if (shape.getPosition().x > 550) {
                        canMove = false;
                        if (light.isOrange()) {
                            currentSpeed = speed / 3;  // Réduit la vitesse si le feu est orange
                        }
                    }
                    break;
                }
            }
        }

        // Vérifie la collision avec les autres voitures dans la même direction
        for (const auto& other : cars) {
            if (&other != this) { // Ne pas se comparer à soi-même
                if (this->comingFromLeft && horizontal && shape.getPosition().x < other.shape.getPosition().x) {
                    if (checkCollision(other)) {
                        canMove = false; // Empêche le mouvement si il y a une collision
                        break;
                    }
                }
                else if (this->comingFromTop && !horizontal && shape.getPosition().y < other.shape.getPosition().y) {
                    if (checkCollision(other)) {
                        canMove = false; // Empêche le mouvement si il y a une collision
                        break;
                    }
                }
                else if (this->comingFromRight && horizontal && shape.getPosition().x > other.shape.getPosition().x) {
                    if (checkCollision(other)) {
                        canMove = false; // Empêche le mouvement si il y a une collision
                        break;
                    }
                }
                else if (this->comingFromBottom && !horizontal && shape.getPosition().y > other.shape.getPosition().y) {
                    if (checkCollision(other)) {
                        canMove = false; // Empêche le mouvement si il y a une collision
                        break;
                    }
                }
            }
        }

        // Si le feu est vert, la voiture peut avancer
        if (canMove) {
            if (horizontal) {
                shape.move(currentSpeed, 0);
                if (shape.getPosition().x > WINDOW_WIDTH) {
                    shape.setPosition(-shape.getSize().x, shape.getPosition().y); // Réapparaît à gauche
                }
            }
            else {
                shape.move(0, currentSpeed);
                if (shape.getPosition().y > WINDOW_HEIGHT) {
                    shape.setPosition(shape.getPosition().x, -shape.getSize().y); // Réapparaît en haut
                }
            }
        }
    }
};


bool isPositionOccupied(float x, float y, const std::vector<Car>& cars, float carWidth, float carHeight) {
    for (const auto& car : cars) {
        if (car.shape.getGlobalBounds().intersects(sf::FloatRect(x, y, carWidth, carHeight))) {
            return true; // Position occupée
        }
    }
    return false; // Position libre
}


int main() {
    // Initialisation aléatoire
    std::srand(static_cast<unsigned>(std::time(0)));
    std::random_device rd; // Générateur basé sur l'entropie matérielle
    std::mt19937 gen(rd()); // Mersenne Twister, un générateur pseudo-aléatoire
    std::uniform_int_distribution<> distrib(1, 3); // Plage de nombres aléatoires [1, 100]

    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simulation Circulation avec Feux");

    // Chargement de l'image de fond
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Program Files/SFML/img/photo_prog.png")) {
        std::cerr << "Erreur : Impossible de charger l'image photo_prog.png" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Création des feux de circulation
    std::vector<TrafficLight> trafficLights;
    trafficLights.emplace_back(260, 130); // Feu pour les voitures venant du haut
    trafficLights.emplace_back(520, 650); // Feu pour les voitures venant du bas
    trafficLights.emplace_back(130, 520); // Feu pour les voitures venant de la gauche
    trafficLights.emplace_back(650, 260); // Feu pour les voitures venant de la droite

    // Timer global pour les feux
    sf::Clock lightClock;
    int lightPhase = 0; // 0 = haut/bas verts, 1 = gauche/droite verts

    // Vecteur pour stocker les voitures
    std::vector<Car> cars;

    // Ajout de voitures initiales sur la partie droite des routes
    cars.emplace_back(50, 440, 80, 50, CAR_SPEED, sf::Color::Blue, false, false, true, false);  // Voiture bleue venant de la gauche
    cars.emplace_back(300, 100, 50, 80, CAR_SPEED, sf::Color::Green, true, false, false, false); // Voiture verte venant du haut
    cars.emplace_back(700, 300, 80, 50, -CAR_SPEED, sf::Color::Yellow, false, false, false, true);  // Voiture jaune venant de la droite
    cars.emplace_back(440, 800, 50, 80, -CAR_SPEED, sf::Color::Red, false, true, false, false); // Voiture rouge venant du bas

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Ajout de voitures avec la touche A
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                int road = std::rand() % 4;
                float carWidth = 80;
                float carHeight = 50;
                if (road == 0) { // Route horizontale haut
                    float x = 700, y = 300;
                    if (!isPositionOccupied(x, y, cars, carWidth, carHeight)) {
                        cars.emplace_back(x, y, carWidth, carHeight, -CAR_SPEED, sf::Color::Yellow, false, false, false, true);
                    }
                }
                else if (road == 1) { // Route horizontale bas
                    float x = 50, y = 440;
                    if (!isPositionOccupied(x, y, cars, carWidth, carHeight)) {
                        cars.emplace_back(x, y, carWidth, carHeight, CAR_SPEED, sf::Color::Cyan, false, false, true, false);
                    }
                }
                else if (road == 2) { // Route verticale gauche
                    float x = 300, y = 50;
                    carWidth = 50;
                    carHeight = 80;
                    if (!isPositionOccupied(x, y, cars, carWidth, carHeight)) {
                        cars.emplace_back(x, y, carWidth, carHeight, CAR_SPEED, sf::Color::Magenta, true, false, false, false);
                    }
                }
                else { // Route verticale droite
                    float x = 440, y = 800;
                    carWidth = 50;
                    carHeight = 80;
                    if (!isPositionOccupied(x, y, cars, carWidth, carHeight)) {
                        cars.emplace_back(x, y, carWidth, carHeight, -CAR_SPEED, sf::Color::Red, false, true, false, false);
                    }
                }
            }


            // Suppression de voitures avec la touche S
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                if (!cars.empty()) {
                    cars.pop_back(); // Supprime la dernière voiture
                }
            }
        }

        // Mise à jour des feux de circulation
        if (lightClock.getElapsedTime() > GREEN_DURATION + ORANGE_DURATION + RED_DURATION) {
            lightClock.restart();
        }

        // Déterminer la phase en fonction du temps écoulé
        // Mise à jour des feux de circulation
        sf::Time elapsed = lightClock.getElapsedTime();

        if (elapsed < GREEN_DURATION) {
            if (lightPhase == 0) {
                // Phase 0 : Haut/Bas verts
                trafficLights[0].setState(GREEN);  // Haut
                trafficLights[1].setState(GREEN);  // Bas
                trafficLights[2].setState(RED);    // Gauche
                trafficLights[3].setState(RED);    // Droite
            }
            else {
                // Phase 1 : Gauche/Droite verts
                trafficLights[0].setState(RED);    // Haut
                trafficLights[1].setState(RED);    // Bas
                trafficLights[2].setState(GREEN);  // Gauche
                trafficLights[3].setState(GREEN);  // Droite
            }
        }
        else if (elapsed < GREEN_DURATION + ORANGE_DURATION) {
            if (lightPhase == 0) {
                // Phase 0 : Haut/Bas oranges
                trafficLights[0].setState(ORANGE); // Haut
                trafficLights[1].setState(ORANGE); // Bas
            }
            else {
                // Phase 1 : Gauche/Droite oranges
                trafficLights[2].setState(ORANGE); // Gauche
                trafficLights[3].setState(ORANGE); // Droite
            }
        }
        else {
            // Après ORANGE_DURATION, on change de phase
            lightClock.restart();
            lightPhase = 1 - lightPhase; // Basculer la phase
        }


        // Déplacement des voitures
        for (auto& car : cars) {
            if (car.shape.getSize().x > car.shape.getSize().y) {
                car.move(true, trafficLights, cars); // Déplacement horizontal
            }
            else {
                car.move(false, trafficLights, cars); // Déplacement vertical
            }
        }

        // Dessin dans la fenêtre
        window.clear();
        window.draw(backgroundSprite); // Dessine le fond

        for (const auto& light : trafficLights) {
            window.draw(light.lightShape); // Dessine chaque feu
        }

        for (const auto& car : cars) {
            window.draw(car.shape); // Dessine chaque voiture
        }

        window.display();
    }
    return 0;
}

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include "constants.hpp"
#include "TrafficLight.hpp"
#include "RandomGenerator.hpp"
#include "Car.hpp"
#include "Bike.hpp"
#include "Pedestrian.hpp"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simulation Circulation avec Feux");
    sf::Texture backgroundTexture;

    if (!backgroundTexture.loadFromFile("C:/Program Files/SFML/img/photo_prog.png"))
    {
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);

    // Feux de circulation à différentes positions
    std::vector<TrafficLight> trafficLights = {
        TrafficLight(260, 130),  // Feu 1
        TrafficLight(520, 650),  // Feu 2
        TrafficLight(130, 520),  // Feu 3
        TrafficLight(650, 260)}; // Feu 4

    std::vector<Car> cars;
    std::vector<Bike> bikes;
    std::vector<Pedestrian> pedestrians;

    RandomGenerator randGen;

    sf::Clock entityClock;
    sf::Clock lightClock;
    int lightPhase = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Mise à jour des feux de circulation
        if (lightClock.getElapsedTime().asSeconds() > LIGHT_CHANGE_INTERVAL)
        {
            lightPhase = (lightPhase + 1) % 4;
            for (size_t i = 0; i < trafficLights.size(); ++i)
            {
                trafficLights[i].setState((i == lightPhase) ? GREEN : RED);
            }
            lightClock.restart();
        }

        // Ajout d'entités aléatoires
        if (entityClock.getElapsedTime().asSeconds() > ENTITY_SPAWN_INTERVAL)
        {
            int entityType = randGen.getRandomInt(0, 2); // 0: Car, 1: Bike, 2: Pedestrian
            int trafficLightIndex = randGen.getRandomInt(0, 3);

            switch (entityType)
            {
            case 0: // Voiture
            //on va crée une voiture à un endroit aléatoire entre les 4 feux de circulation
                //on fait un random entre 0 et 3 pour choisir un feu de circulation
                switch (0)//trafficLightIndex
                {
                case 0: // Feu 1
                    cars.emplace_back(
                        260,130,
                        60, 30,
                        CAR_SPEED,
                        sf::Color::Blue,
                        false, false, false, true);
                    break;
                case 1: // Feu 2
                    cars.emplace_back(
                        520,
                        650,
                        60, 30,
                        CAR_SPEED,
                        sf::Color::Yellow,
                        false, true, false, false);
                    break;
                case 2: // Feu 3
                    cars.emplace_back(
                        130, 520,
                        60, 30,
                        CAR_SPEED,
                        sf::Color::Magenta,
                        true, false, false, false);
                    break;
                case 3: // Feu 4
                    cars.emplace_back(
                        650, 260,
                        60, 30,
                        CAR_SPEED,
                        sf::Color::Cyan,
                        false, false, true, false);
                    break;
                }
                break;
            case 1: // Vélo
                bikes.emplace_back(
                    randGen.getRandomInt(0, WINDOW_WIDTH),
                    randGen.getRandomInt(0, WINDOW_HEIGHT),
                    30, 60,
                    BIKE_SPEED,
                    sf::Color::Green
                    // randGen.getRandomBool(),
                    // randGen.getRandomBool(),
                    // randGen.getRandomBool(),
                    // randGen.getRandomBool()
                    );
                break;
            case 2: // Piéton
                pedestrians.emplace_back(
                    randGen.getRandomInt(0, WINDOW_WIDTH),
                    randGen.getRandomInt(0, WINDOW_HEIGHT),
                    10,
                    PEDESTRIAN_SPEED,
                    sf::Color::Red);
                break;
            }
            entityClock.restart();
        }

        // Mise à jour des entités
        for (auto &car : cars)
        {
            car.move(true, trafficLights, cars);
        }
        for (auto &bike : bikes)
        {
            bike.move(true, trafficLights, bikes);
        }
        for (auto &pedestrian : pedestrians)
        {
            pedestrian.move(true, trafficLights, pedestrians);
        }

        // Suppression des entités hors écran
        cars.erase(remove_if(cars.begin(), cars.end(), [](Car &c)
                             { return c.isOffScreen(); }),
                   cars.end());
        bikes.erase(remove_if(bikes.begin(), bikes.end(), [](Bike &b)
                              { return b.isOffScreen(); }),
                    bikes.end());
        pedestrians.erase(remove_if(pedestrians.begin(), pedestrians.end(), [](Pedestrian &p)
                                    { return p.isOffScreen(); }),
                          pedestrians.end());

        // Dessin des entités et de l'arrière-plan
        window.clear();
        window.draw(backgroundSprite);

        for (const auto &light : trafficLights)
        {
            window.draw(light.lightShape);
        }
        for (const auto &car : cars)
        {
            window.draw(car.shape);
        }
        for (const auto &bike : bikes)
        {
            window.draw(bike.shape);
        }
        for (const auto &pedestrian : pedestrians)
        {
            window.draw(pedestrian.shape);
        }

        window.display();
    }

    return 0;
}

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const float CAR_SPEED = 0.3f;

const float BIKE_SPEED = 1.0f;       // Vitesse des vélos
const float PEDESTRIAN_SPEED = 0.5f; // Vitesse des piétons

const float LIGHT_CHANGE_INTERVAL = 5.0f; // Temps en secondes pour changer les feux
const float ENTITY_SPAWN_INTERVAL = 2.0f; // Temps en secondes pour faire apparaître des entités

const sf::Time GREEN_DURATION = sf::seconds(5);
const sf::Time ORANGE_DURATION = sf::seconds(2);
const sf::Time RED_DURATION = sf::seconds(7);

#endif // CONSTANTS_HPP


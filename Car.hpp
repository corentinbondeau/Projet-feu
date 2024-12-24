#pragma once
#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "TrafficLight.hpp"
#include "RandomGenerator.hpp"

class Car
{
public:
    sf::RectangleShape shape;
    float speed;
    bool comingFromTop;
    bool comingFromBottom;
    bool comingFromLeft;
    bool comingFromRight;
    RandomGenerator randGen;

    Car(float x, float y, float width, float height, float speed, sf::Color color,
        bool top, bool bottom, bool left, bool right);

    bool checkCollision(const Car &other);
    void move(bool horizontal, const std::vector<TrafficLight> &lights, const std::vector<Car> &cars);

    bool isOffScreen() const; // Détecte si la voiture est sortie de l'écran
};

bool isPositionOccupied(float x, float y, const std::vector<Car> &cars, float carWidth, float carHeight);

#endif // CAR_HPP

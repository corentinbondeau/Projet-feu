#ifndef BIKE_HPP
#define BIKE_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Bike
{
public:
    sf::RectangleShape shape;
    bool isActive;
    float speed;
    bool comingFromTop;
    bool comingFromBottom;
    bool comingFromLeft;
    bool comingFromRight;
    RandomGenerator randGen;

    Bike(float x, float y, float width, float height, float speed, sf::Color color)
        : speed(speed)//, isActive(true)
    {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(color);
        shape.setPosition(x, y);
    }

    bool isOffScreen() const
    {
        // Vérifie si la voiture est sortie de l'écran
        sf::Vector2f position = shape.getPosition();
        return position.x < -shape.getSize().x || position.x > WINDOW_WIDTH ||
               position.y < -shape.getSize().y || position.y > WINDOW_HEIGHT;
    }
    bool checkCollision(const Bike &other)
    {
        return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
    }

    // void update()
    // {
    //     shape.move(speed, 0);
    //     if (shape.getPosition().x < 0 || shape.getPosition().x > WINDOW_WIDTH ||
    //         shape.getPosition().y < 0 || shape.getPosition().y > WINDOW_HEIGHT)
    //     {
    //         isActive = false; // Marque pour suppression
    //     }
    // }

    void move(bool horizontal, const std::vector<TrafficLight> &lights, const std::vector<Bike> &bikes)
    {
        // Implémentation du mouvement du vélo
        bool canMove = true;
        float currentSpeed = speed;

        // Vérification des feux de circulation
        for (const auto &light : lights)
        {
            if (comingFromLeft && horizontal && light.lightShape.getPosition().x == 130 && light.isRedOrOrange())
            {
                if (shape.getPosition().x < 150)
                {
                    canMove = false;
                    if (light.isOrange())
                        currentSpeed = speed / 3;
                }
            }
            else if (comingFromTop && !horizontal && light.lightShape.getPosition().y == 130 && light.isRedOrOrange())
            {
                if (shape.getPosition().y < 150)
                {
                    canMove = false;
                    if (light.isOrange())
                        currentSpeed = speed / 3;
                }
            }
            else if (comingFromBottom && !horizontal && light.lightShape.getPosition().y == 650 && light.isRedOrOrange())
            {
                if (shape.getPosition().y > 550)
                {
                    canMove = false;
                    if (light.isOrange())
                        currentSpeed = speed / 3;
                }
            }
            else if (comingFromRight && horizontal && light.lightShape.getPosition().x == 650 && light.isRedOrOrange())
            {
                if (shape.getPosition().x > 550)
                {
                    canMove = false;
                    if (light.isOrange())
                        currentSpeed = speed / 3;
                }
            }
        }

        // Vérification des collisions avec d'autres voitures
        for (const auto &other : bikes)
        {
            if (&other != this)
            { // Ignorer la comparaison avec soi-même
                if (checkCollision(other))
                {
                    canMove = false;
                    break;
                }
            }
        }

        // Déplacement si autorisé
        if (canMove)
        {
            if (horizontal)
            {
                shape.move(currentSpeed, 0);
            }
            else
            {
                shape.move(0, currentSpeed);
            }
        }
    }
};

#endif // BIKE_HPP

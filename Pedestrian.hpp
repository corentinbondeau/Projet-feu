#ifndef PEDESTRIAN_HPP
#define PEDESTRIAN_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Pedestrian
{
public:
    sf::CircleShape shape;
    float speed;
    bool isActive;
    bool comingFromTop;
    bool comingFromBottom;
    bool comingFromLeft;
    bool comingFromRight;
    RandomGenerator randGen;

    Pedestrian(float x, float y, float radius, float speed, sf::Color color)
        : speed(speed)//, isActive(true)
    {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setPosition(x, y);
    }

    bool checkCollision(const Pedestrian &other)
    {
        return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
    }

    // void update()
    // {
    //     shape.move(0, speed);
    //     if (shape.getPosition().x < 0 || shape.getPosition().x > WINDOW_WIDTH ||
    //         shape.getPosition().y < 0 || shape.getPosition().y > WINDOW_HEIGHT)
    //     {
    //         isActive = false; // Marque pour suppression
    //     }
    // }
    void move(bool horizontal, const std::vector<TrafficLight> &lights, const std::vector<Pedestrian> &pedestrians)
    {
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
        for (const auto &other : pedestrians)
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

    bool isOffScreen() const
{
    // Vérifie si la voiture est sortie de l'écran
    sf::Vector2f position = shape.getPosition();

    // return position.x < -shape.getSize().x || position.x > WINDOW_WIDTH ||
    //        position.y < -shape.getSize().y || position.y > WINDOW_HEIGHT;
    return position.x < -shape.getRadius() || position.x > WINDOW_WIDTH ||
    position.y < -shape.getRadius() || position.y > WINDOW_HEIGHT;}
};

#endif //PEDESTRIAN_HPP

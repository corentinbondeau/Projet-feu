#include "Car.hpp"
#include "constants.hpp"

Car::Car(float x, float y, float width, float height, float speed, sf::Color color,
         bool top, bool bottom, bool left, bool right)
    : comingFromTop(top), comingFromBottom(bottom), comingFromLeft(left), comingFromRight(right)
{
    shape = sf::RectangleShape(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    this->speed = speed;
    shape.setFillColor(color);
}

bool Car::checkCollision(const Car &other)
{
    return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
}

void Car::move(bool horizontal, const std::vector<TrafficLight> &lights, const std::vector<Car> &cars)
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
    for (const auto &other : cars)
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

bool Car::isOffScreen() const
{
    // Vérifie si la voiture est sortie de l'écran
    sf::Vector2f position = shape.getPosition();
    return position.x < -shape.getSize().x || position.x > WINDOW_WIDTH ||
           position.y < -shape.getSize().y || position.y > WINDOW_HEIGHT;
}

bool isPositionOccupied(float x, float y, const std::vector<Car> &cars, float carWidth, float carHeight)
{
    for (const auto &car : cars)
    {
        if (car.shape.getGlobalBounds().intersects(sf::FloatRect(x, y, carWidth, carHeight)))
        {
            return true;
        }
    }
    return false;
}

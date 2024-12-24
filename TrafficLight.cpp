#include "TrafficLight.hpp"

TrafficLight::TrafficLight(float x, float y)
{
    lightShape = sf::RectangleShape(sf::Vector2f(20, 20));
    lightShape.setPosition(x, y);
    state = RED;
    updateColor();
}

void TrafficLight::setState(LightState newState)
{
    state = newState;
    updateColor();
}

void TrafficLight::updateColor()
{
    if (state == GREEN)
    {
        lightShape.setFillColor(sf::Color::Green);
    }
    else if (state == ORANGE)
    {
        lightShape.setFillColor(sf::Color(255, 165, 0)); // Orange couleur
    }
    else if (state == RED)
    {
        lightShape.setFillColor(sf::Color::Red);
    }
}

bool TrafficLight::isRedOrOrange() const
{
    return state == RED || state == ORANGE;
}

bool TrafficLight::isGreen() const
{
    return state == GREEN;
}

bool TrafficLight::isOrange() const
{
    return state == ORANGE;
}

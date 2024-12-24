#pragma once

#ifndef TRAFFIC_LIGHT_HPP
#define TRAFFIC_LIGHT_HPP

#include <SFML/Graphics.hpp>

enum LightState
{
    GREEN, ORANGE, RED
};

class TrafficLight
{
public:
    sf::RectangleShape lightShape;
    LightState state;

    TrafficLight(float x, float y);
    void setState(LightState newState);
    void updateColor();
    bool isRedOrOrange() const;
    bool isGreen() const;
    bool isOrange() const;
};

#endif // TRAFFIC_LIGHT_HPP

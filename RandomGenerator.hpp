#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP

#include <random>

class RandomGenerator
{
public:
    RandomGenerator();                  // Constructeur
    int getRandomInt(int min, int max); // Fonction pour générer un entier aléatoire
    bool getRandomBool();               // Fonction pour générer un booléen aléatoire
};

#endif // RANDOM_GENERATOR_HPP

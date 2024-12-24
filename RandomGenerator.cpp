#include "RandomGenerator.hpp"
#include <ctime>

RandomGenerator::RandomGenerator()
{
    std::srand(static_cast<unsigned>(std::time(0))); // Initialise le générateur avec le temps actuel
}

int RandomGenerator::getRandomInt(int min, int max)
{
    return min + std::rand() % ((max + 1) - min); // Génère un nombre entier dans la plage donnée
}

bool getRandomBool()
{
    return rand() % 2 == 0;
}

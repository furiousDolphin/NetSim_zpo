
#pragma once


#include <functional>
#include <random>

#include "types.hpp"

namespace NetSim 
{

    double default_probability_generator();

    //definiujemy globalny wskaznik na funckje ( bo to jest wskaznik jak co )
    extern ProbabilityGenerator probability_generator; 

} 

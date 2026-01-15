
#pragma once

#include <functional>

namespace NetSim 
{
    using ElementID = int;
    using ProbabilityGenerator = std::function<double()>;
    using Time = int;
    using TimeOffset = int;
    using size_type = std::size_t;
} 

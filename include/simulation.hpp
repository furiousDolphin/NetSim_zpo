#pragma once

#include "factory.hpp"


namespace NetSim 
{
    void simulate(Factory& factory, TimeOffset d, std::function<void(Factory&, Time)> rf);

    class IntervalReportNotifier 
    {
        
    };

    class SpecificTurnsReportNotifier 
    {
        
    };
} 

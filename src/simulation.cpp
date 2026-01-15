

#include "simulation.hpp"

#include <stdexcept>

namespace NetSim 
{

    void simulate(Factory& factory, TimeOffset d, std::function<void(Factory&, Time)> rf) 
    {
        if (!factory.is_consistent()) 
        { throw std::logic_error("Factory is inconsistent"); }
        
        for (Time t = 1; t <= d; ++t) 
        {
            factory.do_deliveries(t);
            factory.do_package_passing();
            factory.do_work(t);
            rf(factory, t);
        }
    }

} 

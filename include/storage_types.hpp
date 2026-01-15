#pragma once

#include "package.hpp"


namespace NetSim 
{

    enum class PackageQueueType 
    { 
        FIFO, 
        LIFO 
    };

    //-----------------------------------------------------------------------------------

    class IPackageStockpile 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class IPackageQueue : public IPackageStockpile 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class PackageQueue : public IPackageQueue 
    {
        
    };

}

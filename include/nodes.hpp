#pragma once

#include "config.hpp"
#include "helpers.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"



namespace NetSim 
{

    enum class ReceiverType 
    { 
        WORKER, 
        STOREHOUSE 
    };

    //-----------------------------------------------------------------------------------

    class IPackageReceiver;

    //-----------------------------------------------------------------------------------

    class ReceiverPreferences 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class PackageSender 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class IPackageReceiver 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class Ramp : public PackageSender 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class Worker : public PackageSender, public IPackageReceiver 
    {
        
    };

    //-----------------------------------------------------------------------------------

    class Storehouse : public IPackageReceiver 
    {
        
    };

}
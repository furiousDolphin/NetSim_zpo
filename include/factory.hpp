
#pragma once



#include "nodes.hpp"
#include "types.hpp" 

namespace NetSim 
{

    template <typename Node>
    class NodeCollection 
    {
        
    };

    //-----------------------------------------------------------------------------------

    enum class ElementType 
    {
        LOADING_RAMP, 
        WORKER, 
        STOREHOUSE, 
        LINK
    };

    //-----------------------------------------------------------------------------------

    struct ParsedLineData 
    {
        
    };

    ParsedLineData parse_line(std::string line);

    //-----------------------------------------------------------------------------------

    class Factory 
    {
        
    };

    Factory load_factory_structure(std::istream &is);

    void save_factory_structure(const Factory &factory, std::ostream &os);

} 

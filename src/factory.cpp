#include "factory.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace NetSim 
{

    enum class NodeColor 
    { 
        UNVISITED, 
        VISITED, 
        VERIFIED 
    };

    //-----------------------------------------------------------------------------------

    bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_colors) 
    {
        
    }

    //-----------------------------------------------------------------------------------

    void Factory::remove_worker(ElementID id) 
    { remove_receiver(workers_, id); }

    void Factory::remove_storehouse(ElementID id) 
    { remove_receiver(storehouses_, id); }

    //-----------------------------------------------------------------------------------

    void Factory::do_deliveries(Time t) 
    {
        
    }

    void Factory::do_package_passing() 
    {
        
    }

    void Factory::do_work(Time t) 
    {
        
    }

    //-----------------------------------------------------------------------------------

    bool Factory::is_consistent() 
    {
        
    }

    //-----------------------------------------------------------------------------------

    ParsedLineData parse_line(std::string line) 
    {
        
    }

    //-----------------------------------------------------------------------------------

    Factory load_factory_structure(std::istream &is) 
    {
        
    }

    //-----------------------------------------------------------------------------------

    void save_links(const PackageSender &sender, const std::string &src_tag, ElementID src_id, std::ostream &os) 
    {
        
    }

    //-----------------------------------------------------------------------------------

    void save_factory_structure(const Factory &factory, std::ostream &os) 
    {
        
    }

}
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
        if (node_colors.at(sender) == NodeColor::VERIFIED) 
        { return true; }

        node_colors.at(sender) = NodeColor::VISITED;

        if (sender->get_receiver_preferences().get_preferences().empty()) 
        { throw std::logic_error("No receivers"); }

        bool has_other_receiver = false;

        for (auto &receiver_pair : sender->get_receiver_preferences().get_preferences()) 
        {
            IPackageReceiver *receiver = receiver_pair.first;

            if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) 
            {
                has_other_receiver = true;
            } 
            else 
            {
                Worker *worker = dynamic_cast<Worker *>(receiver);

                if (worker == sender) 
                { continue; }

                has_other_receiver = true;

                if (node_colors.find(worker) == node_colors.end() || node_colors.at(worker) == NodeColor::UNVISITED) 
                { has_reachable_storehouse(worker, node_colors); }
            }
        }

        node_colors.at(sender) = NodeColor::VERIFIED;

        if (!has_other_receiver) 
        { throw std::logic_error("No other receiver"); }

        return true;        
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
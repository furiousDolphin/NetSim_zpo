#include "package.hpp"

namespace NetSim 
{

    std::set<ElementID> Package::assigned_ids_ = {};
    std::set<ElementID> Package::freed_ids_ = {};

    //-----------------------------------------------------------------------------------

    Package::Package() 
    {
        
    }

    Package::Package(ElementID id) : 
        id_(id) 
    {
        assigned_ids_.insert(id_);
        freed_ids_.erase(id);
    }

    //-----------------------------------------------------------------------------------

    Package::Package(Package &&other) noexcept : 
        id_(other.id_) 
    {
        other.id_ = -1;
    }

    Package &Package::operator=(Package &&other) noexcept 
    {
        
    }

    //-----------------------------------------------------------------------------------

    ElementID Package::get_id() const 
    { return id_; }

    Package::~Package() 
    {
        
    }

}

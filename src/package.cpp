#include "package.hpp"

namespace NetSim 
{
    //sattici musze albo tutaj inicjalizowac ( albo moznaby zrobic static inline ale jak kto woli )
    std::set<ElementID> Package::assigned_ids_ = {};
    std::set<ElementID> Package::freed_ids_ = {};

    //-----------------------------------------------------------------------------------

    Package::Package() 
    {
        //tutaj sie nie przeraz ze nie ma -> i ze zapomnialem o nawiasach 
        //( jest dobrze tylko moglem to na dwie linijki rozpisac, zrobic it = ....begin() i potem id_ = *it ale trudo )
        if (!freed_ids_.empty()) 
        {
            // it = freed_ids_.begin();
            // id_ = *it;
            // freed_ids_.erase(it);  mowie jak kto woli
            id_ = *freed_ids_.begin();
            freed_ids_.erase(freed_ids_.begin());
        } 
        else 
        { id_ = (assigned_ids_.empty()) ? 1 : (*assigned_ids_.rbegin() + 1); } //same

        assigned_ids_.insert(id_);        
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
        if (this != &other) 
        {
            if (id_ != -1) 
            {
                assigned_ids_.erase(id_);
                freed_ids_.insert(id_);
            }

            id_ = other.id_;
            other.id_ = -1;  //sprzatamy przy przenoszeniu hihi
        }

        return *this;        
    }

    //-----------------------------------------------------------------------------------

    ElementID Package::get_id() const 
    { return id_; }

    Package::~Package() 
    {
        if (id_ != -1)  //sprawdzam czy wgl id moglo istniec
        {
            assigned_ids_.erase(id_);  //usuwam z wykorzystywanych
            freed_ids_.insert(id_);   // dodaje do wolnych
        }        
    }

}

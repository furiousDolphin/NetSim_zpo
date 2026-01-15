

#include "storage_types.hpp"
#include <stdexcept>

namespace NetSim 
{

    PackageQueue::PackageQueue(PackageQueueType type) : 
        queue_type_(type) 
    {}

    //-----------------------------------------------------------------------------------

    void PackageQueue::push(Package &&package) 
    { container_.emplace_back(std::move(package)); }

    bool PackageQueue::empty() const 
    { return container_.empty(); }

    size_t PackageQueue::size() const 
    { return container_.size(); }

    PackageQueueType PackageQueue::get_queue_type() const 
    { return queue_type_; }

    //-----------------------------------------------------------------------------------

    Package PackageQueue::pop() 
    {
        
    }

    //-----------------------------------------------------------------------------------

    IPackageStockpile::const_iterator PackageQueue::begin() const 
    { return container_.begin(); }

    IPackageStockpile::const_iterator PackageQueue::end() const 
    { return container_.end(); }

    IPackageStockpile::const_iterator PackageQueue::cbegin() const 
    { return container_.cbegin(); }

    IPackageStockpile::const_iterator PackageQueue::cend() const 
    { return container_.cend(); }

}
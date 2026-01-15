

#pragma once

#include "types.hpp"
#include <set>


namespace NetSim 
{

    class Package 
    {
        public:
            Package();

            explicit Package(ElementID id);

            Package(const Package &) = delete;
            Package &operator=(const Package &) = delete;

            Package(Package &&) noexcept;
            Package &operator=(Package &&) noexcept;

            ElementID get_id() const;

            ~Package();

        private:
            ElementID id_;

            static std::set<ElementID> assigned_ids_;
            static std::set<ElementID> freed_ids_;        
    };

}

#pragma once

#include "config.hpp"
#include "helpers.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"

#include <map>
#include <memory>
#include <optional>

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
        public:
            using preferences_t = std::map<IPackageReceiver *, double>;
            using const_iterator = preferences_t::const_iterator;

            explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);

            void add_receiver(IPackageReceiver *receiver_ptr);

            void remove_receiver(IPackageReceiver *receiver_ptr);

            IPackageReceiver *choose_receiver();

            const preferences_t & get_preferences() const;


            const_iterator begin() const;
            const_iterator end() const;
            const_iterator cbegin() const;
            const_iterator cend() const;

        private:
            preferences_t preferences_;
            ProbabilityGenerator pg_;        
    };

    //-----------------------------------------------------------------------------------

    class PackageSender 
    {
        public:
            PackageSender() = default;

            PackageSender(PackageSender &&) = default;

            void send_package();

            const ReceiverPreferences &get_receiver_preferences() const;

            const std::optional<Package>& get_sending_buffer() const 
            { return buffer_; }

            ReceiverPreferences &get_receiver_preferences();

            ReceiverPreferences receiver_preferences_;

        protected:
            void push_package(Package &&package);

            std::optional<Package> buffer_;        
    };

    //-----------------------------------------------------------------------------------

    class IPackageReceiver 
    {
        public:
            using const_iterator = IPackageStockpile::const_iterator;

            virtual void receive_package(Package &&p) = 0;

            virtual ElementID get_id() const = 0;
            virtual std::size_t get_queue_size() const = 0;

            virtual const_iterator begin() const = 0;
            virtual const_iterator end() const = 0;

            virtual const_iterator cbegin() const = 0;
            virtual const_iterator cend() const = 0;

            virtual ~IPackageReceiver() = default;

            virtual ReceiverType get_receiver_type() const = 0;        
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
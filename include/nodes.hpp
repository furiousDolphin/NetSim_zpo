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

    //kocham enumy hihi
    enum class ReceiverType 
    { 
        WORKER, 
        STOREHOUSE 
    };

    //-----------------------------------------------------------------------------------

    class IPackageReceiver;

    //--------------RECEIVER_PREFERENCES - WRAPPER na std::map<IPackageReceiver *, double> z dodatkowymi funckjami----------------

    class ReceiverPreferences 
    {
        public:
            //definicja aliasow
            using preferences_t = std::map<IPackageReceiver *, double>;
            using const_iterator = preferences_t::const_iterator;

            //korzystam z explicita bo to ogolnie dobra praktyka
            explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);

            void add_receiver(IPackageReceiver *receiver_ptr);
            void remove_receiver(IPackageReceiver *receiver_ptr);

            IPackageReceiver *choose_receiver();

            const preferences_t & get_preferences() const;

            //iteratory nic specjalnego ale podpisac mozna
            const_iterator begin() const;
            const_iterator end() const;
            const_iterator cbegin() const;
            const_iterator cend() const;

        private:
            preferences_t preferences_;
            ProbabilityGenerator pg_;        
    };

    //-------Klasa macierzysta do WORKER i RAMP ( worker dziedziczy dwokrotnie )-------------------------------

    class PackageSender 
    {
        public:
            PackageSender() = default;
            PackageSender(PackageSender &&) = default;

            void send_package();

            //gettery
            const ReceiverPreferences &get_receiver_preferences() const;
            ReceiverPreferences &get_receiver_preferences();

            const std::optional<Package>& get_sending_buffer() const 
            { return buffer_; }

            ReceiverPreferences receiver_preferences_;

        protected:
            void push_package(Package &&package);

            std::optional<Package> buffer_;        
    };

    //----------------------IPackageReceiver - Interface do RECEIVEROW ( worker, storehouse )------------------------


    //klasa interface dla workera i storehousea ( wskazniki do tej klasy trzymamy w instancjach ReceiverPreferences )
    class IPackageReceiver 
    {
        public:
            using const_iterator = IPackageStockpile::const_iterator;

            virtual void receive_package(Package &&p) = 0;

            virtual ElementID get_id() const = 0;
            virtual std::size_t get_queue_size() const = 0;


            //generyczne iteratory nic takiego
            virtual const_iterator begin() const = 0;
            virtual const_iterator end() const = 0;
            virtual const_iterator cbegin() const = 0;
            virtual const_iterator cend() const = 0;

            virtual ~IPackageReceiver() = default;

            //analogicznie do komantarza z nad klasy, tu zwracamy ReceiverType::WORKER/STOREHOUSE
            virtual ReceiverType get_receiver_type() const = 0;        
    };

    //--------------RAMPA--------------------------------------------------------------

    class Ramp : public PackageSender 
    {
        public:
            explicit Ramp(ElementID id, TimeOffset di);

            void deliver_goods(Time t);

            //gettery do zasobow
            ElementID get_id() const;
            TimeOffset get_delivery_interval() const;

        private:
            ElementID id_;
            TimeOffset delivery_interval_;        
    };

    //-----------WORKER---------------------------------------------------------------

    class Worker : public PackageSender, public IPackageReceiver 
    {
        public:
            explicit Worker(ElementID id, TimeOffset pd,
                            std::unique_ptr<IPackageQueue> q);

            void receive_package(Package &&p) override;
            void do_work(Time t);

            //gettery
            ElementID get_id() const;
            TimeOffset get_processing_duration() const;
            Time get_product_processing_start_time() const;
            IPackageQueue* get_queue() const;
            std::size_t get_queue_size() const override;
            ReceiverType get_receiver_type() const override;

            const std::optional<Package>& get_processing_buffer() const 
            { return processing_buffer_; }

            //iteratory
            const_iterator begin() const override;
            const_iterator end() const override;
            const_iterator cbegin() const override;
            const_iterator cend() const override;

        private:
            ElementID id_;
            TimeOffset processing_duration_;
            Time package_processing_start_time_ = 0;
            std::unique_ptr<IPackageQueue> q_;
            std::optional<Package> processing_buffer_;        
    };

    //-----------------------------STOREHOUSE---------------------------------------

    class Storehouse : public IPackageReceiver 
    {
        public:
            explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
                                              std::make_unique<PackageQueue>(
                                              PackageQueueType::FIFO));

            void receive_package(Package &&p) override;

            //gettery
            ReceiverType get_receiver_type() const override;
            ElementID get_id() const override;
            std::size_t get_queue_size() const override;

            //iteratory
            const_iterator begin() const override;
            const_iterator end() const override;
            const_iterator cbegin() const override;
            const_iterator cend() const override;

        private:
            ElementID id_;
            std::unique_ptr<IPackageStockpile> d_;     
    };

}
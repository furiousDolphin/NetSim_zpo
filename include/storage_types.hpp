#pragma once

#include "package.hpp"
#include <deque>
#include <list>

namespace NetSim 
{

    enum class PackageQueueType 
    { 
        FIFO, 
        LIFO 
    };

    //-----------------------------------------------------------------------------------

    class IPackageStockpile 
    {
        public:
            using const_iterator = std::list<Package>::const_iterator;

            virtual ~IPackageStockpile() {}

            virtual void push(Package &&package) = 0;

            virtual bool empty() const = 0;

            virtual size_t size() const = 0;

            virtual const_iterator begin() const = 0;
            virtual const_iterator end() const = 0;
            virtual const_iterator cbegin() const = 0;
            virtual const_iterator cend() const = 0;
    };

    //-----------------------------------------------------------------------------------

    class IPackageQueue : public IPackageStockpile 
    {
        public:
            virtual Package pop() = 0;

            virtual PackageQueueType get_queue_type() const = 0;

            virtual ~IPackageQueue() = default;
    };

    //-----------------------------------------------------------------------------------

    class PackageQueue : public IPackageQueue 
    {
        public:
            explicit PackageQueue(PackageQueueType type);

            void push(Package &&package) override;

            bool empty() const override;

            size_t size() const override;

            Package pop() override;

            PackageQueueType get_queue_type() const override;

            const_iterator begin() const override;
            const_iterator end() const override;
            const_iterator cbegin() const override;
            const_iterator cend() const override;

            ~PackageQueue() override = default;

        private:
            PackageQueueType queue_type_;
            std::list<Package> container_;
    };

}

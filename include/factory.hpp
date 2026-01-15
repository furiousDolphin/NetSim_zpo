
#pragma once



#include "nodes.hpp"
#include "types.hpp" 

namespace NetSim 
{

    template <typename Node>
    class NodeCollection 
    {
        public:
            using container_t = std::list<Node>;
            using iterator = typename container_t::iterator;
            using const_iterator = typename container_t::const_iterator; 
                                                    
            //-----------------------------------------------------------------------------------

            iterator begin() { return container_.begin(); }
            iterator end() { return container_.end(); }

            const_iterator begin() const { return container_.begin(); }
            const_iterator end() const { return container_.end(); }
            
            const_iterator cbegin() const { return container_.cbegin(); }
            const_iterator cend() const { return container_.cend(); }

            //-----------------------------------------------------------------------------------

            void add(Node &&node) 
            { container_.emplace_back(std::move(node)); }


            void remove_by_id(ElementID id) 
            {
                auto it = find_by_id(id);
                if (it != container_.end()) 
                { container_.erase(it); }
            }

            //-----------------------------------------------------------------------------------

            iterator find_by_id(ElementID id) 
            {
                return std::find_if(
                    container_.begin(), container_.end(),
                    [id](const Node &node) { return node.get_id() == id; });
            }


            const_iterator find_by_id(ElementID id) const 
            {
                return std::find_if(
                    container_.cbegin(), container_.cend(),
                    [id](const Node &node) { return node.get_id() == id; });
            }

            //-----------------------------------------------------------------------------------

        private:
            container_t container_;        
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
        ElementType element_type;
        std::map<std::string, std::string> parameters;        
    };

    ParsedLineData parse_line(std::string line);

    //-----------------------------------------------------------------------------------

    class Factory 
    {
        public:
            //-----------------------------------------------------------------------------------

            void add_ramp(Ramp &&r) 
            { ramps_.add(std::move(r)); }

            void remove_ramp(ElementID id) 
            { ramps_.remove_by_id(id); }


            NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) 
            { return ramps_.find_by_id(id); }

            NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const 
            { return ramps_.find_by_id(id); }

            NodeCollection<Ramp>::const_iterator ramp_cbegin() const 
            { return ramps_.cbegin(); }

            NodeCollection<Ramp>::const_iterator ramp_cend() const 
            { return ramps_.cend(); }

            //-----------------------------------------------------------------------------------

            void add_worker(Worker &&w) 
            { workers_.add(std::move(w)); }

            void remove_worker(ElementID id);


            NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) 
            { return workers_.find_by_id(id); }

            NodeCollection<Worker>::const_iterator
            find_worker_by_id(ElementID id) const 
            { return workers_.find_by_id(id); }

            NodeCollection<Worker>::const_iterator worker_cbegin() const 
            { return workers_.cbegin(); }

            NodeCollection<Worker>::const_iterator worker_cend() const 
            { return workers_.cend(); }


            //-----------------------------------------------------------------------------------

            void add_storehouse(Storehouse &&s) 
            { storehouses_.add(std::move(s)); }

            void remove_storehouse(ElementID id); 

            NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) 
            { return storehouses_.find_by_id(id); }

            NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const 
            { return storehouses_.find_by_id(id); }

            NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const 
            { return storehouses_.cbegin(); }

            NodeCollection<Storehouse>::const_iterator storehouse_cend() const 
            { return storehouses_.cend(); }

            //-----------------------------------------------------------------------------------

            bool is_consistent();
            void do_deliveries(Time t);
            void do_package_passing();
            void do_work(Time t);

            //-----------------------------------------------------------------------------------

        private:

            template <typename Node>
            void remove_receiver(NodeCollection<Node> &collection, ElementID id) 
            {
                auto it = collection.find_by_id(id);

                if (it != collection.end()) 
                {
                    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>( &(*it) );

                    if (receiver_ptr) 
                    {
                        for (auto& ramp : ramps_) 
                        { ramp.get_receiver_preferences().remove_receiver(receiver_ptr); }

                        for (auto& worker : workers_) 
                        { worker.get_receiver_preferences().remove_receiver(receiver_ptr); }
                    }

                    collection.remove_by_id(id);
                }
            }

            NodeCollection<Ramp> ramps_;
            NodeCollection<Worker> workers_;
            NodeCollection<Storehouse> storehouses_;        
    };

    Factory load_factory_structure(std::istream &is);

    void save_factory_structure(const Factory &factory, std::ostream &os);

} 

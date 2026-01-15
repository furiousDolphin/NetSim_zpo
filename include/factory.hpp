
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
        
    };

    Factory load_factory_structure(std::istream &is);

    void save_factory_structure(const Factory &factory, std::ostream &os);

} 

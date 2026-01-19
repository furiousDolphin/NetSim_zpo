#include "reports.hpp"
#include <algorithm>
#include <vector>

namespace NetSim 
{
    void generate_structure_report(const Factory& factory, std::ostream& os) 
    {
        os << "\n";
        os << "== LOADING RAMPS ==\n";

        std::vector<ElementID> ramp_ids;

        //rownie dobrze mogles zrobic range for jak dla mnie ale ok
        for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) 
        { ramp_ids.push_back(it->get_id()); }

        std::sort(ramp_ids.begin(), ramp_ids.end());   //sortujemy wedlug id

        for (auto id : ramp_ids) 
        {
            auto it = factory.find_ramp_by_id(id);

            os << "\nLOADING RAMP #" << it->get_id() << "\n";
            os << "  Delivery interval: " << it->get_delivery_interval() << "\n";
            os << "  Receivers:\n";
            
            std::vector<std::pair<std::string, ElementID>> receivers;

            //nastepnym razem robimy iteratory kalsie ReceiverPreferences i tyle bo to wygladda strasznei
            for (const auto& [receiver_ptr, prob] : it->get_receiver_preferences().get_preferences()) 
            {
                std::string type_str = (receiver_ptr->get_receiver_type() == ReceiverType::WORKER ? "worker" : "storehouse");

                receivers.push_back( {type_str, receiver_ptr->get_id()} );
            }

            std::sort(receivers.begin(), receivers.end(), [](const auto& a, const auto& b) 
            {
                // sortujemy najpierw alfabetycznie ( czyli czy worker czy storehouse ) a potem po id
                if (a.first != b.first) 
                { return a.first < b.first; }

                return a.second < b.second;
            });
            
            for (const auto & [receiver_str, receiver_id] : receivers) 
            { os << "    " << receiver_str << " #" << receiver_id << "\n"; }
        }

        //-----------------------------------------------------------------------------------

        os << "\n\n== WORKERS ==\n";

        std::vector<ElementID> worker_ids;

        for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) 
        { worker_ids.push_back(it->get_id()); }

        std::sort(worker_ids.begin(), worker_ids.end());

        for (auto id : worker_ids) 
        {
            auto it = factory.find_worker_by_id(id);

            os << "\nWORKER #" << it->get_id() << "\n";
            os << "  Processing time: " << it->get_processing_duration() << "\n";
            os << "  Queue type: " << (it->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO") << "\n";
            os << "  Receivers:\n";
            
            std::vector<std::pair<std::string, ElementID>> receivers;

            for (const auto& [receiver_ptr, prob] : it->get_receiver_preferences().get_preferences()) 
            {
                std::string type_str = (receiver_ptr->get_receiver_type() == ReceiverType::WORKER ? "worker" : "storehouse");

                receivers.push_back( {type_str, receiver_ptr->get_id()} );
            }

            std::sort(receivers.begin(), receivers.end(), [](const auto& a, const auto& b) 
            {
                // sortujemy najpierw alfabetycznie ( czyli czy worker czy storehouse bo worker moze wysylac tez do workera ) a potem po id
                if (a.first != b.first) 
                { return a.first < b.first; }
                
                return a.second < b.second;
            });
            
            for (const auto& [receiver_str, receiver_id] : receivers) 
            { os << "    " << receiver_str << " #" << receiver_id << "\n"; }
        }

        //-----------------------------------------------------------------------------------

        os << "\n\n== STOREHOUSES ==\n";

        std::vector<ElementID> storehouse_ids;

        for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) 
        { storehouse_ids.push_back(it->get_id()); }

        std::sort(storehouse_ids.begin(), storehouse_ids.end());

        for (auto id : storehouse_ids) 
        { os << "\nSTOREHOUSE #" << id << "\n"; }

        os << "\n";
    }

    //-----------------------------------------------------------------------------------

    void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time t) 
    {
        os << "=== [ Turn: " << t << " ] ===\n";
        os << "\n== WORKERS ==\n";

        std::vector<ElementID> worker_ids;

        for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) 
        { worker_ids.push_back(it->get_id()); }

        std::sort(worker_ids.begin(), worker_ids.end());

        for (auto id : worker_ids) 
        {
            auto it = factory.find_worker_by_id(id);

            os << "\nWORKER #" << it->get_id() << "\n";

            os << "  PBuffer: ";

            if (it->get_processing_buffer().has_value())   //po to bylo robienie tego restet przy std:;optional w workerze ( zeby teraz to dzialalo )
            {
                //tutaj ( bo to moze dziwnie wygladac )  mamy bufer->metoda()  to robimy val = bufer.value() i potem val.metoda(), ale tak jest w jednej linijce wiec zostawiem
                os << "#" << it->get_processing_buffer()->get_id() << " (pt = " 
                   << (t - it->get_product_processing_start_time() + 1) << ")\n";
            } 
            else 
            {
                os << "(empty)\n";
            }

            //------------------------

            os << "  Queue: ";

            if (it->cbegin() == it->cend()) 
            { os << "(empty)\n"; } 

            else 
            {
                for (auto queue_it = it->cbegin(); queue_it != it->cend(); ++queue_it) 
                { os << "#" << queue_it->get_id() << (std::next(queue_it) == it->cend() ? "" : ", "); }
                os << "\n";
            }

            //------------------------

            os << "  SBuffer: ";

            // to samo co wczesneij ( patrz na PBuffer )
            if (it->get_sending_buffer().has_value()) 
            { os << "#" << it->get_sending_buffer()->get_id() << "\n"; } 

            else 
            { os << "(empty)\n"; }
        }

        //-----------------------------------------------------------------------------------

        os << "\n\n== STOREHOUSES ==\n";

        std::vector<ElementID> storehouse_ids;

        for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) 
        { storehouse_ids.push_back(it->get_id()); }

        std::sort(storehouse_ids.begin(), storehouse_ids.end()); //sortujemy po id

        for (auto id : storehouse_ids) 
        {
            auto it = factory.find_storehouse_by_id(id);

            os << "\nSTOREHOUSE #" << id << "\n";
            os << "  Stock: ";

            if (it->cbegin() == it->cend()) 
            { os << "(empty)\n"; } 

            else 
            {
                for (auto storehouse_it = it->cbegin(); storehouse_it != it->cend(); ++storehouse_it) 
                { os << "#" << storehouse_it->get_id() << (std::next(storehouse_it) == it->cend() ? "" : ", "); }
                os << "\n";
            }
        }

        os << "\n";
    }
}

#include "factory.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace NetSim 
{

    enum class NodeColor 
    { 
        UNVISITED, 
        VISITED, 
        VERIFIED 
    };

    //-----------------------------------------------------------------------------------

    //wole to napisac poza klasa jakby mocki mialy sie buntowac
    bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_colors) 
    {
        //jesli wywolalismy to dla sendera ktory byl juz sprawdzany to skipujemy to wywolanie
        if (node_colors.at(sender) == NodeColor::VERIFIED) 
        { return true; }

        //tak jak w dfs musimy odznaczyc jako odwiedzone na poczatku bo potem nie bedzie sposobnosci
        node_colors.at(sender) = NodeColor::VISITED;

        if (sender->get_receiver_preferences().get_preferences().empty()) 
        { throw std::logic_error("No receivers"); }

        bool has_other_receiver = false;

        for (auto & [receiver_ptr, prob] : sender->get_receiver_preferences().get_preferences())
        {
            //IPackageReceiver *receiver_ptr = receiver_ptr; // uzywanie pari jak dal mnie jest nieczytelne sorki musialem pozmieniac

            if (receiver_ptr->get_receiver_type() == ReceiverType::STOREHOUSE) 
            { has_other_receiver = true; } //tutaj moge swrocic od razu true bo storehouse nei wysyla nic dalej ( pnadto tylko to mialem sprawdzic )

            else 
            {
                Worker *worker_ptr = dynamic_cast<Worker *>(receiver_ptr); // tutaj przez to ze trzymam rzeczy jako IPackageReceiver* to musze to zrzutowac na Worker*

                if (worker_ptr == sender)  // zmeinilbym na sender ptr ale boje sie tych testow ze znow im cos nie bedzie pasowac jak zawsze
                { continue; } //ogolnie jesi wskazniki sendera i workera w ktorego pobralem sa takie same to znaczy ze wysylam sam do siebie i nic wiecej sie nie dowiem

                //skoro przeszedl dalej to znaczy ze nie wysyla sam do siebie wiec otrzymany worker_ptr to faktycznie inny receiver
                has_other_receiver = true;

                //sprawdzamy czy odwiedzony, jak nie to idziemy glebiej z DFS'em
                if (node_colors.find(worker_ptr) == node_colors.end() || node_colors.at(worker_ptr) == NodeColor::UNVISITED) 
                { has_reachable_storehouse(worker_ptr, node_colors); }
            }
        }

        //korzystam z .at() zeby przypadkiem nie dopisac nowego wpisu
        node_colors.at(sender) = NodeColor::VERIFIED;

        //to musze wywolac po przypisaniu stanu odwiedzenia
        if (!has_other_receiver) 
        { throw std::logic_error("No other receiver"); }

        return true;        
    }

    //-----------------------------------------------------------------------------------

    void Factory::remove_worker(ElementID id) 
    { this->remove_receiver(workers_, id); }

    void Factory::remove_storehouse(ElementID id) 
    { this->remove_receiver(storehouses_, id); }

    //-----------------------------------------------------------------------------------

    void Factory::do_deliveries(Time t) 
    {
        for (auto &ramp : ramps_) 
        { ramp.deliver_goods(t); }        
    }

    void Factory::do_package_passing() 
    {
        for (auto &ramp : ramps_) 
        { ramp.send_package(); }

        for (auto &worker : workers_) 
        { worker.send_package(); }        
    }

    void Factory::do_work(Time t) 
    {
        for (auto &worker : workers_) 
        { worker.do_work(t); }        
    }

    //-----------------------------------------------------------------------------------

    //taki dfs'ik troche, nawet troche bardzo
    bool Factory::is_consistent() 
    {
        std::map<const PackageSender *, NodeColor> node_colors;


        //ustawiamy wszystko na WHITE/UNVISITED
        for (const auto &ramp : ramps_) 
        { node_colors[&ramp] = NodeColor::UNVISITED; }

        for (const auto &worker : workers_) 
        { node_colors[&worker] = NodeColor::UNVISITED; }

        try 
        {
            //dla kazdej rampy musze stprawdzic czy paczka z niej wyslana dojdzie do storehouse obojetnie jakiego
            for (const auto &ramp : ramps_) 
            {
                //sluzy d o tego funkcja widoczna poniezej ktorej ciala nalezy szukac powyzej
                if (!has_reachable_storehouse(&ramp, node_colors)) 
                { return false; }
            }
        } 
        catch (const std::logic_error &e) 
        {
            return false;
        }

        return true;        
    }

    //-----------------------------------------------------------------------------------

    ParsedLineData parse_line(std::string line) 
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream token_stream(line);

        while (std::getline(token_stream, token, ' ')) 
        {
            if (!token.empty()) 
            { tokens.push_back(token); }
        }

        //oznaczenie za co odpowiadza linijka to pierwszy string po ich zesplitowaniu
        std::string tag = tokens[0];
        ElementType type;

        if (tag == "LOADING_RAMP") 
        { type = ElementType::LOADING_RAMP; }

        else if (tag == "WORKER") 
        { type = ElementType::WORKER; }

        else if (tag == "STOREHOUSE") 
        { type = ElementType::STOREHOUSE; }

        else if (tag == "LINK") 
        { type = ElementType::LINK; }

        else 
        { throw std::logic_error("Unknown tag: " + tag); }

        std::map<std::string, std::string> parameters;


        //pierwszy indeks wzielismy jako tag powyzej wiec robimy od 1
        for (size_t i = 1; i < tokens.size(); ++i) 
        {
            size_t sep = tokens[i].find('=');

            if (sep != std::string::npos) 
            {
                std::string key = tokens[i].substr(0, sep);
                std::string value = tokens[i].substr(sep + 1);
                parameters[key] = value;
            }
        }

        //dodaje zeby nie szukac

        // struct ParsedLineData 
        // {
        //     ElementType element_type;
        //     std::map<std::string, std::string> parameters;        
        // };

        return {type, parameters};        
    }

    //-----------------------------------------------------------------------------------

    Factory load_factory_structure(std::istream &is) 
    {
        Factory factory;
        std::string line;

        while (std::getline(is, line)) 
        {
            if (line.empty() || line[0] == ';') //tu nie musze chyba nic tlumaczyc
            { continue; }

            ParsedLineData data = parse_line(line);

            if (data.element_type == ElementType::LOADING_RAMP) 
            {
                ElementID id = std::stoi(data.parameters["id"]);
                TimeOffset di = std::stoi(data.parameters["delivery-interval"]);

                factory.add_ramp(Ramp(id, di));
            } 

            else if (data.element_type == ElementType::WORKER) 
            {
                ElementID id = std::stoi(data.parameters["id"]);
                TimeOffset pd = std::stoi(data.parameters["processing-time"]);

                std::string q_type_str = data.parameters["queue-type"];
                PackageQueueType q_type = (q_type_str == "LIFO") ? PackageQueueType::LIFO : PackageQueueType::FIFO;

                factory.add_worker( Worker(id, pd, std::make_unique<PackageQueue>(q_type)) );
            } 

            else if (data.element_type == ElementType::STOREHOUSE) 
            {
                ElementID id = std::stoi(data.parameters["id"]);

                factory.add_storehouse(Storehouse(id));
            } 

            else if (data.element_type == ElementType::LINK) 
            {
                std::string src_str = data.parameters["src"];
                std::string dest_str = data.parameters["dest"];

                size_t src_sep = src_str.find('-');
                std::string src_type = src_str.substr(0, src_sep);
                ElementID src_id = std::stoi(src_str.substr(src_sep + 1));

                size_t dest_sep = dest_str.find('-');
                std::string dest_type = dest_str.substr(0, dest_sep);
                ElementID dest_id = std::stoi(dest_str.substr(dest_sep + 1));

                //------------na podstawie src_type-dest_type pozyskanego z pliku, musze sprawdzic czy wczesniej stworzylem danego src i dest---------------
                //------------i jesli stworzylem ( bo LINK bedzie na koncu ) to musze pobrac wskaznik do receiver'a i dodac go do sender'a ( src )----------
                IPackageReceiver *receiver;


                if (dest_type == "worker") 
                {
                    auto it = factory.find_worker_by_id(dest_id);
                    if (it == factory.worker_cend()) 
                    { throw std::logic_error("Receiver not found"); }

                    receiver = &(*it);
                } 

                else if (dest_type == "storehouse" || dest_type == "store") 
                {
                    auto it = factory.find_storehouse_by_id(dest_id);
                    if (it == factory.storehouse_cend()) 
                    { throw std::logic_error("Receiver not found"); }

                    receiver = &(*it);
                } 

                else 
                { throw std::logic_error("Unknown receiver type: " + dest_type); }

                //-----dodaje receivera do worker albo do ramp w zaleznosci od tego czy src_type jest workerem czy ramp wlasnie-----------

                if (src_type == "ramp") 
                {
                    auto it = factory.find_ramp_by_id(src_id);
                    if (it == factory.ramp_cend()) 
                    { throw std::logic_error("Sender not found"); }

                    it->get_receiver_preferences().add_receiver(receiver);
                } 

                else if (src_type == "worker") 
                {
                    auto it = factory.find_worker_by_id(src_id);
                    if (it == factory.worker_cend()) 
                    { throw std::logic_error("Sender not found"); }

                    it->get_receiver_preferences().add_receiver(receiver);
                } 

                else 
                { throw std::logic_error("Unknown sender type"); }

                //---------------------------
            }
        }
        return factory;        
    }

    //-----------------------------------------------------------------------------------

    void save_links(const PackageSender &sender, const std::string &src_tag, ElementID src_id, std::ostream &os) 
    {
        for (const auto & [receiver_ptr, prob] : sender.get_receiver_preferences().get_preferences()) 
        {
            os << "LINK src=" << src_tag << "-" << src_id << " dest=";
            os << (receiver_ptr->get_receiver_type() == ReceiverType::WORKER ? "worker-" : "store-");
            os << receiver_ptr->get_id() << "\n";
        }        
    }

    //-----------------------------------------------------------------------------------

    void save_factory_structure(const Factory &factory, std::ostream &os) 
    {
        for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) 
        { os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << "\n"; }

        for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) 
        {
            os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration() << " queue-type=";
            os << (it->get_queue()->get_queue_type() == PackageQueueType::LIFO ? "LIFO" : "FIFO") << "\n";
        }

        for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) 
        { os << "STOREHOUSE id=" << it->get_id() << "\n"; }

        for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) 
        { save_links(*it, "ramp", it->get_id(), os); }

        for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) 
        { save_links(*it, "worker", it->get_id(), os); }

        os.flush();        
    }
}
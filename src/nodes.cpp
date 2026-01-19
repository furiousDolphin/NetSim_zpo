#include "nodes.hpp"

namespace NetSim 
{

    ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg) : 
        pg_(pg) 
    {}

    //-----------------------------------------------------------------------------------

    // void ReceiverPreferences::set_random_probabilities()
    // {
    //     std::list<double> probs;
    //     size_type n = preferences_.size();
    //     double prob_sum = 0.0;

    //     std::generate_n(
    //         std::back_inserter(probs), n, 
    //         [&prob_sum, this]()
    //         { 
    //             auto prob = probability_generator(); 
    //             prob_sum+=prob; 
    //             return prob; 
    //         }     
    //     );

    //     for ( auto& [ preference_ptr, preference_prob ] : preferences_ )
    //     {
    //         auto prob = probs.front();
    //         probs.pop_front();

    //         preference_prob = prob / prob_sum;
    //     }       
    // }


    //po dodaniu receivery ustwaiamy rowne prawdopodobienstawa na wszystkich receiverow
    //z kodem powyrzej byloby ciekawiej, ale TESTY takk oczywiscie testy nie przechodza
    void ReceiverPreferences::add_receiver(IPackageReceiver *receiver_ptr) 
    {
        preferences_[receiver_ptr] = 1.0;

        double new_prob = 1.0 / preferences_.size();

        for (auto& [receiver_ptr, prob] : preferences_) 
        { prob = new_prob; }
    }

    //to samo co wczesniej
    void ReceiverPreferences::remove_receiver(IPackageReceiver *receiver_ptr) 
    {
        preferences_.erase(receiver_ptr);

        if (preferences_.empty()) 
        { return; }

        double new_prob = 1.0 / preferences_.size();

        for (auto& [receiver_ptr, prob] : preferences_) 
        { prob = new_prob; }        
    }

    //-----------------------------------------------------------------------------------

    //tu robimy uproszczone wyszkukiwanie liniowe ( czyli dodajemy nasze probs tak dlugo jak wylosowany prob nie jest mniejszy )
    IPackageReceiver* ReceiverPreferences::choose_receiver() 
    {
        double p = pg_();
        double distribution = 0.0;

        for (const auto &[ receiver_ptr, prob ] : preferences_) 
        {
            distribution += prob;

            if (p <= distribution) 
            { return receiver_ptr; }
        }

        if (!preferences_.empty()) 
        { return preferences_.rbegin()->first; }

        return nullptr;        
    }

    //-----------------------------------------------------------------------------------

    const ReceiverPreferences::preferences_t & ReceiverPreferences::get_preferences() const 
    { return preferences_; }

    //iteratory
    ReceiverPreferences::const_iterator ReceiverPreferences::begin() const 
    { return preferences_.begin(); }

    ReceiverPreferences::const_iterator ReceiverPreferences::end() const 
    { return preferences_.end(); }

    ReceiverPreferences::const_iterator ReceiverPreferences::cbegin()  const 
    { return preferences_.cbegin(); }

    ReceiverPreferences::const_iterator ReceiverPreferences::cend() const 
    { return preferences_.cend(); }


    //-----------------------------------------------------------------------------------

    void PackageSender::send_package() 
    {
        if ( buffer_ ) 
        {
            IPackageReceiver* receiver_ptr = receiver_preferences_.choose_receiver();

            if (receiver_ptr != nullptr) 
            {
                receiver_ptr->receive_package(std::move( *buffer_ ));
                buffer_.reset(); //zeby .has_value() zwrocilo false zgodnie z prawda, jezeli chodzi o czyszczenie to i tak konstruktor przenoszacy wyczysci
            }
        }        
    }

    const ReceiverPreferences &PackageSender::get_receiver_preferences() const 
    { return receiver_preferences_; }

    ReceiverPreferences &PackageSender::get_receiver_preferences() 
    { return receiver_preferences_;  }

    void PackageSender::push_package(Package&& package) 
    { buffer_.emplace( std::move(package) ); }

    //-----------------------------------------------------------------------------------

    Ramp::Ramp(ElementID id, TimeOffset di) : 
        id_(id), 
        delivery_interval_(di) 
    {}

    void Ramp::deliver_goods(Time t) 
    {
        if ((t - 1) % delivery_interval_ == 0) 
        {
            Package p;
            this->push_package( std::move(p) );
        }    
    }

    ElementID Ramp::get_id() const 
    { return id_; }

    TimeOffset Ramp::get_delivery_interval() const 
    { return delivery_interval_; }

    //-----------------------------------------------------------------------------------

    Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : 
        id_(id), 
        processing_duration_(pd), 
        q_( std::move(q) ) 
    {}

    void Worker::receive_package(Package &&p) 
    { q_->push( std::move(p) ); }


    void Worker::do_work(Time t) 
    {
        if ( !processing_buffer_ && !q_->empty() ) 
        {
            //jesli pracownik nad niczym nei pracuje to sciagamy z zgodnie z typem kolejki odpowiednia z niej wartosc i aktualizujemy start time
            processing_buffer_.emplace( q_->pop() );

            package_processing_start_time_ = t;
        }

        if (processing_buffer_) 
        {

            //jesli workem ma nad czy mpracowac to musimy sprawdzic czy jego praca trwa juz okreslona liczbe tur przetwrzania package'ea
            if (t - package_processing_start_time_ >= processing_duration_ - 1) 
            {
                this->push_package(std::move( *processing_buffer_) );
                processing_buffer_.reset();
            }
        }
    }

    //-----------------------------------------------------------------------------------


    //gettery
    ReceiverType Worker::get_receiver_type() const 
    { return ReceiverType::WORKER; }

    ElementID Worker::get_id() const 
    { return id_; }


    std::size_t Worker::get_queue_size() const 
    { return q_->size(); }

    TimeOffset Worker::get_processing_duration() const 
    { return processing_duration_; }

    Time Worker::get_product_processing_start_time() const 
    { return package_processing_start_time_; }

    IPackageQueue* Worker::get_queue() const 
    { return q_.get(); }


    //iteratory
    Worker::const_iterator Worker::begin() const 
    { return q_->begin(); }


    Worker::const_iterator Worker::end() const 
    { return q_->end(); }

    Worker::const_iterator Worker::cbegin() const 
    { return q_->cbegin(); }

    Worker::const_iterator Worker::cend() const 
    { return q_->cend(); }

    //-----------------------------------------------------------------------------------

    Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d): 
        id_(id), 
        d_(std::move(d)) 
    {}

    //gettery
    ReceiverType Storehouse::get_receiver_type() const 
    { return ReceiverType::STOREHOUSE; }

    void Storehouse::receive_package(Package &&p) 
    { d_->push(std::move(p)); }

    ElementID Storehouse::get_id()  const 
    { return id_; }

    std::size_t Storehouse::get_queue_size() const 
    { return d_->size(); }


    //iteratory
    Storehouse::const_iterator Storehouse::begin() const 
    { return d_->begin(); }

    Storehouse::const_iterator Storehouse::end()  const 
    { return d_->end(); }

    Storehouse::const_iterator Storehouse::cbegin()  const 
    { return d_->cbegin(); }


    Storehouse::const_iterator Storehouse::cend() const 
    { return d_->cend(); }

}
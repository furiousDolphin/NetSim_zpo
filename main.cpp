#include "factory.hpp"
#include "reports.hpp"
#include "simulation.hpp"
#include <iostream>
#include <fstream>

int main() 
{
    std::cout << "NetSim Simulation starting..." << std::endl;
    
    //-----------------------------------------------------------------------------------

    std::ifstream input_file("factory_structure.txt");

    if (!input_file.is_open()) 
    {
        std::cerr << "Error: Could not open factory_structure.txt" << std::endl;
        return 1;
    }

    //-----------------------------------------------------------------------------------

    try 
    {
        NetSim::Factory factory = NetSim::load_factory_structure(input_file);
        std::cout << "Factory structure loaded successfully." << std::endl;

        if (factory.is_consistent()) 
        {
            std::cout << "Network is consistent." << std::endl;
        } 
        else 
        {
            std::cerr << "Warning: Network is inconsistent!" << std::endl;
        }
        
        std::cout << "\n--- Initial Factory Structure ---\n" << std::endl;
        NetSim::generate_structure_report(factory, std::cout);

        //-----------------------------------------------------------------------------------

        std::ofstream output_file("factory_structure_saved.txt");

        if (output_file.is_open()) 
        {
            NetSim::save_factory_structure(factory, output_file);
            std::cout << "Structure saved to factory_structure_saved.txt" << std::endl;
        }
        
        //-----------------------------------------------------------------------------------

        NetSim::IntervalReportNotifier notifier(1); 

        std::cout << "\n--- Running Simulation ---\n" << std::endl;

        NetSim::simulate(factory, 5, [&notifier](NetSim::Factory& f, NetSim::Time t) 
        {
            if (notifier.should_generate_report(t)) 
            { NetSim::generate_simulation_turn_report(f, std::cout, t); }
        });

    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    //-----------------------------------------------------------------------------------

    std::cout << "NetSim Simulation finished." << std::endl;
    
    return 0;
}

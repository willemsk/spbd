#include <iostream>
#include <string>
#include "langevin.h"
#include "fileio.h"



int main(
         int argc,
         char **argv
         )
{
    /* Instructions */
    char header[] = {"\n\n\
----------------------------------------------------------------------\n\
    SPBD -- Single Protein Brownian Dynamics\n\
    \n\
    Kherim Willems (kherim@kher.im)\n\
    \n"};
    char usage[] = {"\n\n\
----------------------------------------------------------------------\n\
    This program performs a 1D brownian dynamics simulation on a\n\
    single particle trapped inside a force profile.\n\
        Usage:\n\n\
        spdb [options] spdb.in\n\n\
        where spdb.in is a formatted input file and [options] are:\n\n\
--output-file=<name>     Enables output logging to the path\n\
    listed in <name>.  Uses flat-file format.\n\
--help                   Display this help information.\n\
--version                Display the current SPDB version.\n\
----------------------------------------------------------------------\n\n"};

    //std::cout << header;
    //std::cout << usage;
    
    bool debug = false;
    std::string conf_file("test/test_conf.txt");
    
    // Create new simulation object
    langevin_simulation simulation;
    
    
    std::cout << "Loading configuration file... ";
    loadConfiguration(conf_file, simulation.conf);
    
    if (debug)
    {
    std::cout << "done.\n";
    printConfiguration(simulation.conf);
    }
    
    std::cout << "Reserving memory space... ";
    unsigned long long output_elements =
        int((simulation.conf.steps/simulation.conf.saveFreq)+1);
    simulation.out.positionVector.reserve(output_elements);
    simulation.out.timeVector.reserve(output_elements);
    std::cout << "done.\n";
    
    computeLangevinTrajectory(simulation);
    
    std::cout << "Writing data to disk (" << simulation.conf.trajectoryOutputFile <<")... ";
    writeSimulationResultsToFile(simulation);
    std::cout << "done.\n";

    return 0;
}



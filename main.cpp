#include <iostream>
#include <string>
#include "langevin.h"


int main(
         int argc,
         char **argv
         )
{
    /* Instructions */
    char header[] = {"\n\n\
----------------------------------------------------------------------\n\
    SPBD -- Single Protein Bownian Dynamics\n\
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

    std::cout << header;
    std::cout << usage;


    std::cout << "Initializing simulation parameters...";
    
    /* User input */
    
    unsigned long long steps = 10000000000;
    unsigned long long saveFreq = 1000;
    float timestep = 0.1;
    float temperature = 0.05;
    float posStart = 2; // At first minimum
    float posSpacing = 0.01;
    float damping = 1;
    
    /* Other input */
    
    float posMin = 0;
    float posMax = 6;
    
    //std::vector<float> forceVector = {24.000,21.489,19.152,16.983,14.976,13.125,11.424,9.867,8.448,7.161,6.000,4.959,4.032,3.213,2.496,1.875,1.344,0.897,0.528,0.231,0.000,-0.171,-0.288,-0.357,-0.384,-0.375,-0.336,-0.273,-0.192,-0.099,0.000,0.099,0.192,0.273,0.336,0.375,0.384,0.357,0.288,0.171,0.000,-0.231,-0.528,-0.897,-1.344,-1.875,-2.496,-3.213,-4.032,-4.959,-6.000,-7.161,-8.448,-9.867,-11.424,-13.125,-14.976,-16.983,-19.152,-21.489,-24.000};
    std::vector<float> forceVector(int((posMax-posMin)/posSpacing+1));
    buildForceVector(posSpacing,posMin,posMax,forceVector);
    
    std::cout << "Printing vectors:\n";
    std::cout << "forceVec: ";
    printVector(forceVector);
    
    std::vector<float> dampingVector(forceVector.size(),damping);
    
    //std::vector<float> positionVector(int(steps/saveFreq)+1);
    //std::vector<float> timeVector(int(steps/saveFreq)+1);
    std::vector<float> positionVector;
    positionVector.reserve(int(steps/saveFreq)+1);
    std::vector<float> timeVector;
    timeVector.reserve(int(steps/saveFreq)+1);
    int method = 1;
    std::cout << " done.\n";
    
    computeLangevinTrajectory( steps,
                               saveFreq,
                               timestep,
                               temperature,
                               posStart,
                               posSpacing,
                               forceVector,
                               dampingVector,
                               positionVector,
                               timeVector,
                               method);
    
    std::cout << "Simulation results:\n";
    std::cout << "time position\n";
    for (unsigned int i =0; i<positionVector.size(); i++)
    {
        std::cout << timeVector[i] << " " << positionVector[i] << "\n";
    }     
    
    
    

    return 0;
}



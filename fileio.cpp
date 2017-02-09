/**
 * @file    fileio.cpp
 * @ingroup Langevin
 * @brief   Routines for loading and saving files
 * @version $spbd_version$
 * @author  Kherim Willems
 * 
 * @attention
 * @verbatim
 * 
 * SPBD -- Single Protein Bownian Dynamics
 * 
 *  Kherim Willems (kherim@kher.im)
 * 
 * @endverbatim
 */
 
#include "fileio.h"


int readFileToStrings(
    std::string const &filename,
    std::vector<std::string> &lines
    )
{
    // Allocate variables
    std::ifstream infile;
    std::string line;
    
    // Read in file contents
    try
    {
        // Opening the file for reading
        infile.open(filename.c_str(), std::ios::in);
        
        // Read all the lines
        while (std::getline(infile, line))
        {
            lines.push_back(line);
        }
        
        infile.close();
    }
    catch (std::exception e)
    {
        std::cout << "Exception opening file: " << filename << "\n"
                  << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}

void writeSimulationResultsToFile(
    langevin_simulation simu
    )
{
    writeTrajectoryToFile(
        simu.conf.trajectoryOutputFile,
        simu.out.positionVector,
        simu.out.timeVector);
}

void writeTrajectoryToFile(
    std::string const &filename,
    std::vector<float> const &positionVector,
    std::vector<unsigned long long> const &timeVector
    )
{
    // Allocate variables
    std::ofstream outfile;
    unsigned long long step;
    float position;
    
    try
    {
        // Open file for writing
        outfile.open(filename.c_str(), std::ios::out | std::ios::trunc);
        // Print out header
        outfile << "step" << ", " << "position" << "\n";
        // Print out all values to file
        for (unsigned long long i = 0; i < timeVector.size(); i++)
        {
            // Load step number and postion
            step = timeVector[i];
            position = positionVector[i];
            // Save data to file
            outfile << step << ", " << position << "\n";
        }
        outfile.close();
    }
    catch (std::exception e)
    {
        std::cout << "Exception writing to file: " << filename << "\n"
                  << "Exception: " << e.what() << std::endl;
    }
}

template<typename Out>
void split(
    const std::string &s,
    char delim,
    Out result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            *(result++) = item;
        }
        
    }
}


std::vector<std::string> split(
    const std::string &s,
    char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::vector<float> convertStringToFloatVector(
    const std::vector<std::string> &v
    )
{
    
    // Create new vector
    std::vector<float> float_v;
    float_v.reserve(v.size());
    
    // Loop over all elements and convert
    for (std::vector<std::string>::size_type i = 0; i != v.size(); i++)
    {
        float_v.push_back(std::stof(v[i]));
        
    }
    
    return float_v;
}
 
 
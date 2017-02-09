/**
 * @defgroup  IO  FileIO class
 * @brief     Numerical solver for langevin equation
*/
/**
 * @file    fileio.h
 * @ingroup IO
 * @brief   Contains declarations for class FileIO
 * @version $spbd_version$
 * @author  Kherim Willems
 * 
 * @attention
 * @verbatim
 * 
 * SPBD -- Single Protein Brownian Dynamics
 * 
 *  Kherim Willems (kherim@kher.im)
 * 
 * @endverbatim
 */

#ifndef _LANGEVINFILEIO_H_
#define _LANGEVINFILEIO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>

#include "langevin.h"


int readFileToStrings(
    std::string const &filename,
    std::vector<std::string> &lines
    );

void writeSimulationResultsToFile(
    langevin_simulation simu
    );

void writeTrajectoryToFile(
    std::string const &filename,
    std::vector<float> const &positionVector,
    std::vector<unsigned long long> const &timeVector
    );

template<typename Out>
void split(
    const std::string &s,
    char delim,
    Out result);

std::vector<std::string> split(
    const std::string &s,
    char delim
    );

std::vector<float> convertStringToFloatVector(
    const std::vector<std::string> &v
    );

#endif
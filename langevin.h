/**
 * @defgroup  Langevin  Langevin class
 * @brief     Numerical solver for langevin equation
*/
/**
 * @file    langevin.h
 * @ingroup Langevin
 * @brief   Contains declarations for class Langevin
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

#ifndef _LANGEVINROUTINES_H_
#define _LANGEVINROUTINES_H_

#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

struct langevin_configuration {
    std::string name;
    unsigned long long steps;
    unsigned long long saveFreq;
    float timestep;
    float temperature;
    float damping;
    float positionStart;
    float positionSpacing;
    std::vector<float> forceVector;
    std::vector<float> dampingVector;
    int method;
    std::string trajectoryOutputFile;
};

struct langevin_output {
    std::vector<float> positionVector;
    std::vector<unsigned long long> timeVector;
};

struct langevin_simulation {
    langevin_configuration conf;
    langevin_output out;
};

/** 
 * @brief   Computes a Langevin trajectory for given parameters
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   simulation     Simulation object
 * @returns 0 on success
 */
int computeLangevinTrajectory(
    langevin_simulation &simu
    );

/** 
 * @brief   Computes a Langevin trajectory for given parameters
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   steps           Number of simulation steps to perform [1]
 * @param   saveFreq        Save after every 'saveFreq' timestep [1]
 * @param   timestep        Size of the integration timestep [ns]
 * @param   temperature     Temperature of the simulation kB*T [pN*nm]
 * @param   damping         Damping constant of particle [pN*ns/nm]
 * @param   positionStart   Starting position of particle [nm]
 * @param   positionSpacing Spacing between each force point [nm]
 * @param   forceVector     Vector containing the force distribution [pN]
 * @param   dampingVector   Vector containing the distribution of relative damping constants at each position [1]
 * @param   positionVector  Vector containing the simulated particle positions [nm]
 * @param   timeVector      Vector containing the simulated particle simulation times [ns]
 * @param   method          Integration method to use
 * @returns 0 on success
 */
int computeLangevinTrajectory(
    const unsigned long long steps,
    const unsigned long long saveFreq,
    const float timestep,
    const float temperature,
    const float damping,
    const float positionStart,
    const float positionSpacing,
    std::vector<float> const &forceVector,
    std::vector<float> const &dampingVector,
    std::vector<float> &positionVector,
    std::vector<unsigned long long> &timeVector,
    const int method
    );


/** 
 * @brief   Compute external force langevin step size vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   timestep        Size of the integration timestep [ns]
 * @param   damping         Damping constant of particle [pN*ns/nm]
 * @param   forceVector     Vector containing the force distribution [pN]
 * @param   dampingVector   Vector containing the distribution of relative damping constants at each position [1]
 * @param   externalVector  Output vector containing thermal step [nm]
 * @returns 0 on success
 */    
int calcExternalStepVector(
    float timestep,
    float damping,
    std::vector<float> const &forceVector,
    std::vector<float> const &dampingVector,
    std::vector<float> &externalVector
    );

/** 
 * @brief   Compute thermal force langevin step size vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   timestep        Size of the integration timestep [ns]
 * @param   temperature     Temperature of the simulation kB*T [pN*nm]
 * @param   damping         Damping constant of particle [pN*ns/nm]
 * @param   dampingVector   Vector containing the distribution of relative damping constants at each position [1]
 * @param   thermalVector   Output vector containing thermal step [nm]
 * @returns 0 on success
 */
int calcThermalStepVector(
    float timestep,
    float temperature,
    float damping,
    std::vector<float> const &dampingVector,
    std::vector<float> &thermalVector
    );
    
/** 
 * @brief   Compute thermal force langevin step size vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   vec             Vector to print
 * @param   delimiter       Delimiter to use
 * */
template <typename T>
void printVector(
    std::vector<T> const &vec,
    char delimiter
    );

/** 
 * @brief   Compute a force vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   spacing         Spacing between points
 * @param   min             Minimum position to evaluate
 * @param   max             Maximum position to evaluate
 * @param   forceVector     Vector to save force in
 * */    
void buildForceVector(
    float spacing,
    float min,
    float max,
    std::vector<float> &forceVector
    );

/** 
 * @brief   Loads a langevin simulation configuration from a file
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   filename         Spacing between points
 * @param   conf             Minimum position to evaluate
 * @returns 0 on succes
 * */    
int loadConfiguration(
    std::string const &filename,
    langevin_configuration &conf
    );
    
/** 
 * @brief   Prints the parameters of a langevin simulation configuration
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   conf         Langevin configuration to print
 * */     
void printConfiguration(
    langevin_configuration conf
    );

#endif

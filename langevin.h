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
 * SPBD -- Single Protein Bownian Dynamics
 * 
 *  Kherim Willems (kherim@kher.im)
 * 
 * @endverbatim
 */

#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cmath>

/** 
 * @brief   Computes a Langevin trajectory for given parameters
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   steps           Number of simulation steps to perform [1]
 * @param   saveFreq        Save after every 'saveFreq' timestep [1]
 * @param   timestep        Size of the integration timestep [ns]
 * @param   temperature     Temperature of the simulation kB*T [pN*nm]
 * @param   positionStart   Starting position of particle [nm]
 * @param   positionSpacing Spacing between each force point [nm]
 * @param   forceVector     Vector containing the force distribution [pN]
 * @param   dampingVector   Vector containing the distribution of damping constants at each position [pN*ns/nm]
 * @param   positionVector  Vector containing the simulated particle positions [nm]
 * @param   timeVector      Vector containing the simulated particle simulation times [ns]
 * @param   method          Integration method to use
 * @returns 0 on success
 */
int computeLangevinTrajectory(
    unsigned long long steps,
    unsigned long long saveFreq,
    float timestep,
    float temperature,
    float positionStart,
    float positionSpacing,
    std::vector<float> const &forceVector,
    std::vector<float> const &dampingVector,
    std::vector<float> &positionVector,
    std::vector<float> &timeVector,
    int method
    );


/** 
 * @brief   Compute external force langevin step size vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   timestep        Size of the integration timestep [ns]
 * @param   forceVector     Vector containing the force distribution [pN]
 * @param   dampingVector   Vector containing the distribution of damping constants at each position [pN*ns/nm]
 * @param   externalVector  Output vector containing thermal step [nm]
 * @returns 0 on success
 */    
int calcExternalStepVector(
    float timestep,
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
 * @param   dampingVector   Vector containing the distribution of damping constants at each position [pN*ns/nm]
 * @param   thermalVector   Output vector containing thermal step [nm]
 * @returns 0 on success
 */
int calcThermalStepVector(
    float timestep,
    float temperature,
    std::vector<float> const &dampingVector,
    std::vector<float> &thermalVector
    );
    
/** 
 * @brief   Compute thermal force langevin step size vector
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   vec             Vector to print
 * */
template <typename T>
void printVector(
    std::vector<T> const &vec
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
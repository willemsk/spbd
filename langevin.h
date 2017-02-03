/**
 * @defgroup  Langevin  Langevin class
 * @brief     Numerical solver for langevin equation

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

/** 
 * @brief   Computes a Langevin trajectory
 * @ingroup Langevin
 * @author  Kherim Willems
 * @param   steps           Number of simulation steps to perform
 * @param   timestep        Size of the integration timestep [ns]
 * @param   saveFreq        Save after every 'saveFreq' timestep
 * @param   temperature     Temperature of the simulation [kB*T]
 * @param   damping         Damping constant of particle [kB*T/ns]
 * @param   positionStart   Starting position of particle [nm]
 * @param   spacingVector   Vector containing the locations of each force point [nm]
 * @param   forceVector     Vector containing the force distribution [pN]
 * @param   dampingVector   Vector containing the distribution of damping correction factors [1]
 * @param   positionVector  Vector containing the simulated particle positions [nm]
 * @param   timeVector      Vector containing the simulated particle simulation times [ns]
 * @returns 0 on success
 */
int computeLangevinTrajectory (
    long steps,
    int timestep
    int saveFreq
    float temperature
    float damping
    float positionStart
    const *float[] spacingVector
    const *float[] forceGrid 
    const *float[] dampingGrid
    const *float[] positionVector
    const *float[] timeVector
    )
/**
 * @file    langevin.c
 * @ingroup Langevin
 * @brief   Routines for numerically solving the langevin equation
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
#include "langevin.h"

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
    ) {
        
    }
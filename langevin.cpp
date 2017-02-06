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
    )
{   
    std::cout << "Initializing simulation...\n";
    /* Check sanity of arguments */
    std::cout << "  Checking arguments...";
    
    // size of positionVector, timeVector == int_round_down(steps/saveFreq)+1
    // size of forceVector == dampingVector
    // size of positionVector == timeVector
    // spacingVector[0] <= positionStart <= spaceVector[end]
    
    std::cout << " done.\n";
    
    /* Allocate memory for force vectors */
    std::vector<float> externalVector(forceVector.size());
    std::vector<float> thermalVector(forceVector.size());
    
    
    /* Populate local force vectors */
    
    // F/gamma
    std::cout << "  External force...";
    
    calcExternalStepVector( timestep,
                            forceVector,
                            dampingVector,
                            externalVector);
                            
    std::cout << " done.\n";
    
    // (kB*T*Dt/gamma)^0.5
    std::cout << "  Thermal force...";
    calcThermalStepVector( timestep,
                           temperature,
                           dampingVector,
                           thermalVector);
    std::cout << " done.\n";
    
    /* Initialize random gaussian distribution */
    std::cout << "  Random distribution...";
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,1.0);
    std::cout << " done.\n";
    
    /* Set the initial position */
    std::cout << "  Initial position...";
    positionVector.push_back(positionStart);
    timeVector.push_back(0);
    
    float newPos = positionStart;
    float oldPos = positionStart;
    std::cout << " done.\n";
    
    std::cout << "Printing vectors:\n";
    std::cout << "exF: ";
    printVector(externalVector);
    std::cout << "thF: ";
    printVector(thermalVector);
    
    unsigned long long percentFreq = steps/20;
    float minPos = 0.0;
    float maxPos = (forceVector.size()-1)*positionSpacing;
    unsigned long index = long(oldPos/positionSpacing);
    float eForceStep = 0.0;
    float tForceStep = 0.0;
    int percent = 0;

    /* Perform steps */
    std::cout << "Running simulation..." << std::endl;
    for (unsigned long long s = 1; s <= steps; s++) {
        
        // Calculate the index for this position and check for out-of-bounds
        if (oldPos < minPos) {
            index = long(minPos/positionSpacing);
        } else if (oldPos > maxPos) {
            index = long(maxPos/positionSpacing);
        } else {
            index = long(oldPos/positionSpacing);
        }        
        
        //std::cout << "Index: " << index << std::endl;
        
        // Grab external force at this position
        eForceStep = externalVector[index];
        //std::cout << "eForceStep: " << eForceStep << std::endl;
        // Grab thermal force at this position
        tForceStep = thermalVector[index]*distribution(generator);
        //std::cout << "tForceStep: " << tForceStep << std::endl;
        
        // Calculate new position
        newPos = oldPos + eForceStep + tForceStep;
        //std::cout << "newPos: " << newPos << std::endl;
        
        // Save position and timestamp if needed
        if (s % saveFreq == 0) {
            positionVector.push_back(newPos);
            timeVector.push_back(s*timestep);
        }
        if (s % percentFreq == 0) {
            
            percent = (float)(s)/(float)(steps)*100;
            std::cout << "  " << percent << "% completed." << std::endl;
        }
        
        // Overwrite oldPos for next iteration
        oldPos = newPos;
        
    }
    /* Cleanup */
    
    return 0;
}

int calcExternalStepVector(
    float timestep,
    std::vector<float> const &forceVector,
    std::vector<float> const &dampingVector,
    std::vector<float> &externalVector)
{
    float force = 0;
    float damping = 0;
    float external = 0;
    
    for (unsigned int i = 0; i < externalVector.size(); i++) {
        force = forceVector[i];
        damping = dampingVector[i];
        external = force/damping*timestep;
        externalVector[i] = external;
    }
    
    return 0;
}

int calcThermalStepVector(
    float timestep,
    float temperature,
    std::vector<float> const &dampingVector,
    std::vector<float> &thermalVector)
{
    float damping = 0.0;
    float thermal = 0.0;
    
    for (unsigned int i = 0; i < thermalVector.size(); i++) {
        damping = dampingVector[i];
        thermal = std::sqrt(2*temperature*timestep/damping);
        thermalVector[i] = thermal;
    }
    
    return 0;
}

template <typename T>
void printVector(std::vector<T> const &vec) {
    for (auto val : vec)
    {
        std::cout << val << ", ";
    }
    std::cout << std::endl;
}

void buildForceVector(
    float spacing,
    float min,
    float max,
    std::vector<float> &forceVector)
{
    unsigned int numElements = (max-min)/spacing + 1;
    
    float position = 0.0;
    
    for (unsigned int i = 0; i<numElements; i++)
    {
        position = spacing*i + min;
        forceVector[i] = -std::pow( (position-3) ,3) + (position-3);
    }
}


    
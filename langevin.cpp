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
#include "fileio.h"

int computeLangevinTrajectory(
    langevin_simulation &simu
    )
{
    return computeLangevinTrajectory(
        simu.conf.steps,
        simu.conf.saveFreq,
        simu.conf.timestep,
        simu.conf.temperature,
        simu.conf.damping,
        simu.conf.positionStart,
        simu.conf.positionSpacing,
        simu.conf.forceVector,
        simu.conf.dampingVector,
        simu.out.positionVector,
        simu.out.timeVector,
        simu.conf.method);
}

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
    )
{   
    std::cout << "Initializing...\n";
    
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
                            damping,
                            forceVector,
                            dampingVector,
                            externalVector);
                            
    std::cout << " done.\n";
    
    // (kB*T*Dt/gamma)^0.5
    std::cout << "  Thermal force...";
    calcThermalStepVector( timestep,
                           temperature,
                           damping,
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
    
    /*
    std::cout << "Printing step vectors:\n";
    std::cout << "Damped external force stepsize per position:\n";
    printVector(externalVector,',');
    std::cout << "Damped thermal force stepsize per position:\n";
    printVector(thermalVector,',');
    */
    
    unsigned long long percentFreq = steps/20;
    float minPos = 0.0;
    float maxPos = (forceVector.size()-1)*positionSpacing;
    unsigned long index = long(oldPos/positionSpacing);
    float eForceStep = 0.0;
    float tForceStep = 0.0;
    int percent = 0;

    /* Perform steps */
    std::cout << "Running simulation..." << std::endl;
    for (unsigned long long s = 1; s != steps+1; s++) {

        
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
            timeVector.push_back(s);
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
    float damping,
    std::vector<float> const &forceVector,
    std::vector<float> const &dampingVector,
    std::vector<float> &externalVector)
{
    float forceV = 0.0;
    float dampingV = 0.0;
    float externalV = 0.0;
    
    for (unsigned int i = 0; i < externalVector.size(); i++) {
        forceV = forceVector[i];
        dampingV = dampingVector[i];
        externalV = (forceV*timestep)/(damping*dampingV);
        externalVector[i] = externalV;
    }
    
    return 0;
}

int calcThermalStepVector(
    float timestep,
    float temperature,
    float damping,
    std::vector<float> const &dampingVector,
    std::vector<float> &thermalVector)
{
    float dampingV = 0.0;
    float thermalV = 0.0;
    
    for (unsigned int i = 0; i < thermalVector.size(); i++) {
        dampingV = dampingVector[i];
        thermalV = std::sqrt((2*temperature*timestep)/(damping*dampingV));
        thermalVector[i] = thermalV;
    }
    return 0;
}

template <typename T>
void printVector(std::vector<T> const &vec, char delimiter) {
    for (auto val : vec)
    {
        std::cout << val << delimiter;
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



int loadConfiguration(
    std::string const &filename,
    langevin_configuration &conf
    )
{
    // Allocate variables
    std::vector<std::string> param_value;
    std::vector<std::string> lines;
    
    // Save filename to conf
    conf.name = filename;
    
    // Load all lines from configuration file
    readFileToStrings(conf.name, lines);
    
    // Convert all lines to configuration parameters
    for (unsigned int i=0; i<lines.size(); i++)
    {
        std::string line = lines[i];
        // Extract parameter and value from line
        param_value = split(line, ' ');
        const char* param = param_value[0].c_str();
        std::string value = param_value[1];
        
        // Go over each of the possible parameters
        if(std::strcmp(param, "steps") == 0)
        {
            conf.steps = std::stoull(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "saveFreq") == 0)
        {
            conf.saveFreq = std::stoull(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "timestep") == 0)
        {
            conf.timestep = std::stof(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "temperature") == 0)
        {
            conf.temperature = std::stof(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "damping") == 0)
        {
            conf.damping = std::stof(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "positionStart") == 0)
        {
            conf.positionStart = std::stof(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "positionSpacing") == 0)
        {
            conf.positionSpacing = std::stof(param_value[1]);
            continue;
        }
        if(std::strcmp(param, "forceVector") == 0)
        {
            // Convert the value part to float vector 
            std::vector<float> forceVector =
                convertStringToFloatVector(split(value,','));
            // Save vector in configuration
            conf.forceVector = forceVector;
            continue;
        }
        if(std::strcmp(param, "dampingVector") == 0)
        {
            // Convert the value part to float vector 
            std::vector<float> dampingVector =
                convertStringToFloatVector(split(value,','));
            // Save vector in configuration
            conf.dampingVector = dampingVector;
            continue;
        }
        if(std::strcmp(param, "method") == 0)
        {
            int method = 0;
            const char* val = param_value[1].c_str();
            
            if (std::strcmp(val, "first") == 0)
            {
                method = 1;
            }
            if (std::strcmp(val, "second") == 0)
            {
                method = 2;
            }
            
            conf.method = method;
            continue;
        }
        if(std::strcmp(param, "trajectoryOutputFile") == 0)
        {
            conf.trajectoryOutputFile = param_value[1];
            continue;
        }
        
    
    }
    
    return 0;
}

void printConfiguration(
    langevin_configuration conf
    )
{
    std::cout << "Configuration file parameters:\n";
    std::cout << "                     name: " << conf.name                 << "\n";
    std::cout << "                    steps: " << conf.steps                << "\n";
    std::cout << "                 saveFreq: " << conf.saveFreq             << "\n";
    std::cout << "                 timestep: " << conf.timestep             << "\n";
    std::cout << "              temperature: " << conf.temperature          << "\n";
    std::cout << "                  damping: " << conf.damping              << "\n";
    std::cout << "            positionStart: " << conf.positionStart        << "\n";
    std::cout << "          positionSpacing: " << conf.positionSpacing      << "\n";
    std::cout << "                   method: " << conf.method               << "\n";
    std::cout << "     trajectoryOutputFile: " << conf.trajectoryOutputFile << "\n";
    std::cout << "              forceVector:\n";
    printVector(conf.forceVector, ',');
    std::cout << "            dampingVector:\n";
    printVector(conf.dampingVector, ',');
    std::cout << std::endl;
}
    
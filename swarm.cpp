#include <yaml-cpp/yaml.h>
#include <iostream>

#include "swarm.h"

/******************\
|* PUBLIC METHODS *|
\******************/

Swarm::Swarm()
{
    // Turn flag to be false
    isMemoryAlloc = false;
}

Swarm::Swarm(ofstream *log)
{
    this->log = log;
    // Turn flag to be false
    isMemoryAlloc = false;
}

Swarm::~Swarm()
{
    if (isMemoryAlloc)
    {
        delete [] min;
        delete [] max;
    }
}

int Swarm::getN()
{
    return N;
}

int Swarm::getMin(unsigned int i)
{
    return min[i];
}

int Swarm::getMax(unsigned int i)
{
    return max[i];
}

void Swarm::start()
{
    // Trying to read params from yaml-file
    if (!readConfig())
    {
        *log << "Failed to read config file. Terminating..." << endl;
        return;
    }
}

/*******************\
|* PRIVATE METHODS *|
\*******************/

bool Swarm::readConfig(const string filename)
{
    try
    {
        // Create map "config" from yaml-file
        YAML::Node config = YAML::LoadFile(filename);

        // Logging all values into log-file
        *log << "Values from \"" << filename << "\"" << endl;
        for (auto v : config)
            *log << v.first << ": " << v.second << endl;

        N = config["N"].as<int>();
        min = new double [N];
        max = new double [N];
        // If we are here without exceptions, memory has been allocated
        isMemoryAlloc = true;

        // Read mins and maxes from yaml-file
        for (int i = 0; i < N; ++i)
        {
            string temp { "min" };    // This is a string for key-value of "config"
            temp += to_string(i + 1); // Append index to the end of "temp"
            min[i] = config[temp];    // Then get the mins' value
            temp = "max";             // Again with max
            temp += to_string(i + 1);
            max[i] = config[temp];
        }
        nIterations = config["nIterations"].as<int>();
        nBees = config["nBees"].as<int>();
        pCoef = config["pCoef"].as<double>();
        gCoef = config["gCoef"].as<double>();
        vCoef = config["vCoef"].as<double>();
    }
    catch (exception &e)
    {
        *log << e.what() << endl;
        return false;
    }
    return true;
}

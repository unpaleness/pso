#include <yaml-cpp/yaml.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "swarm.h"
#include "etc.h"

using namespace BSO;

/******************\
|* PUBLIC METHODS *|
\******************/

Swarm::Swarm() {}

Swarm::Swarm(ofstream *log)
{
    this->log = log;
}

Swarm::~Swarm()
{
    if (isMemoryAlloc)
    {
        delete [] min;
        delete [] max;
        delete [] g;
    }
    if (areBeesCreated)
        for (int i = 0; i < nBees; ++i)
            delete bees[i];
    if (areBeesAllocated)
        delete [] bees;
}

int Swarm::getN() { return N; }

double Swarm::getMin(unsigned int i) { return min[i]; }

double Swarm::getMax(unsigned int i) { return max[i]; }

double Swarm::getPCoef() { return pCoef; }

double Swarm::getGCoef() { return gCoef; }

double Swarm::getVCoef() { return vCoef; }

double Swarm::getG(unsigned int i) { return g[i]; }

const double *Swarm::getG() { return g; }

void Swarm::setG(unsigned int i, double val) { g[i] = val; }

bool Swarm::getEnableBeeLog() { return enableBeeLog; }

void Swarm::start()
{
    // Trying to read params from yaml-file
    *log << "Reading config file..." << endl;
    if (!readConfig())
    {
        *log << "Failed to read config file. Terminating..." << endl;
        return;
    }
    *log << "Success!" << endl << endl;
    *log << "=================================================================\
===============" << endl;
    // Trying to create bees
    *log << "Creating bees..." << endl;
    if (!createBees())
    {
        *log << "Failed to create bees. Terminating..." << endl;
        return;
    }
    *log << "Success!" << endl << endl;

    // Run iterations
    for (int it = 0; it < nIterations; ++it)
    {
        *log << "=============================================================\
===================" << endl;
        *log << "Starting " << it + 1 << " iteration..." << endl;
        for (int iBee = 0; iBee < nBees; ++iBee)
        {
            if (!enableBeeLog)
            {
                *log << " Bee " << iBee + 1 << "(#" << bees[iBee] << "): ";
                switch(bees[iBee]->update())
                {
                    case 0:
                        *log << "out of bounds, skipping..." << endl;
                        break;
                    case 2:
                        *log << "found new bee's best position..." << endl;
                        break;
                    case 3:
                        *log << "found new swarm's position: (";
                        for (int i = 0; i < N; ++i)
                        {
                            if (i > 0)
                                *log << ";";
                            *log << g[i];
                        }
                        *log << ")" << endl;
                        break;
                    default:
                        *log << "OK" << endl;
                        break;
                }
            }
            else
                bees[iBee]->update();
        }
    }
}

double Swarm::f(const double *x)
{
    return x[0] * x[0] + 2.0 * x[1] * x[1] + 3.0 * x[0] * x[1];
}

/*******************\
|* PRIVATE METHODS *|
\*******************/

bool Swarm::readConfig(const string filename)
{
    bool status { true };

    try
    {
        // Create map "config" from yaml-file
        YAML::Node config = YAML::LoadFile(filename);

        // Logging all values into log-file
        *log << " Values from \"" << filename << "\":" << endl;
        for (const auto &v : config)
        {
            *log << "  ";
            log->width(BSO::W);
            *log << v.first.as<string>() << ": " << v.second.as<string>()
                 << endl;
        }

        N = config["N"].as<int>();
        min = new double [N];
        max = new double [N];
        g = new double [N];
        // If we are here without exceptions, memory has been allocated
        isMemoryAlloc = true;

        // Read mins and maxes from yaml-file
        for (int i = 0; i < N; ++i)
        {
            string temp { "min" };    // String for key-value of "config"
            temp += to_string(i + 1); // Append index to the end of "temp"
            min[i] = config[temp].as<double>(); // Then get the mins' value
            temp = "max";             // Again with max
            temp += to_string(i + 1);
            max[i] = config[temp].as<double>();
        }

        // Set up initial values for g
        for (int i = 0; i < N; ++i)
            g[i] = min[i] + (max[i] - min[i]) / 2.0;

        nIterations = config["nIterations"].as<int>();
        nBees = config["nBees"].as<int>();
        pCoef = config["pCoef"].as<double>();
        gCoef = config["gCoef"].as<double>();
        vCoef = config["vCoef"].as<double>();
        enableBeeLog = config["enableBeeLog"].as<bool>();
    }
    catch (exception &e)
    {
        *log << e.what() << endl;
        return false;
    }
    /* Some naughty occasions */

    // 1) min <= max
    for (int i = 0; i < N; ++i)
        if (min[i] >= max[i])
        {
            *log << "Oops! min" << i + 1 << " seems to be greater or equal \
than max" << i + 1 << endl;
            status = false;
        }

    // 2) nBees <= 0
    if (nBees <= 0)
    {
        *log << "Oops! nBees = " << nBees << ", I cannot create " << nBees
             << " bees, sorry" << endl;
        status = false;
    }

    // 3) nIterations <= 0
    if (nIterations <= 0)
    {
        *log << "Oops! nIterations = " << nIterations << ", I cannot run "
             << nIterations << " iterations, sorry" << endl;
        status = false;
    }

    return status;
}

bool Swarm::createBees()
{
    try
    {
        bees = new Bee *[nBees];
        areBeesAllocated = true;
        for (int i = 0; i < nBees; ++i)
        {
            bees[i] = new Bee(log, this);
            if (!enableBeeLog) *log << " Bee " << i + 1 << "(#" << bees[i]
                                   << ") created" << endl;
        }
        areBeesCreated = true;
    }
    catch (exception &e)
    {
        *log << e.what() << endl;
        return false;
    }
    return true;
}

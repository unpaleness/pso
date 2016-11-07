#include <yaml-cpp/yaml.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "swarm.h"
#include "etc.h"

using namespace PSO;

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
    if (areParticlesCreated)
        for (int i = 0; i < nParticles; ++i)
            delete particles[i];
    if (areParticlesAllocated)
        delete [] particles;
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

bool Swarm::getEnableParticleLog() { return enableParticleLog; }

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
    // Trying to create particles
    *log << "Creating particles..." << endl;
    if (!createParticles())
    {
        *log << "Failed to create particles. Terminating..." << endl;
        return;
    }
    *log << "Success!" << endl << endl;

    // Run iterations
    for (int it = 0; it < nIterations; ++it)
    {
        *log << "=============================================================\
===================" << endl;
        *log << "Starting " << it + 1 << " iteration..." << endl;
        for (int iParticle = 0; iParticle < nParticles; ++iParticle)
        {
            if (!enableParticleLog)
            {
                *log << " Particle " << iParticle + 1 << "(#" << particles[iParticle] << "): ";
                switch(particles[iParticle]->update())
                {
                    case 0:
                        *log << "out of bounds, skipping..." << endl;
                        break;
                    case 2:
                        *log << "found new particle's best position..." << endl;
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
                particles[iParticle]->update();
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
            log->width(PSO::W);
            *log << v.first.as<string>() << ": " << v.second.as<string>()
                 << endl;
        }

        N = config["N"].as<int>();
        min = new double [N];
        max = new double [N];
        g = new double [N];
        // If we are here without exceptions, memory has particlen allocated
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
        nParticles = config["nParticles"].as<int>();
        pCoef = config["pCoef"].as<double>();
        gCoef = config["gCoef"].as<double>();
        vCoef = config["vCoef"].as<double>();
        enableParticleLog = config["enableParticleLog"].as<bool>();
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

    // 2) nParticles <= 0
    if (nParticles <= 0)
    {
        *log << "Oops! nParticles = " << nParticles << ", I cannot create " << nParticles
             << " particles, sorry" << endl;
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

bool Swarm::createParticles()
{
    try
    {
        particles = new Particle *[nParticles];
        areParticlesAllocated = true;
        for (int i = 0; i < nParticles; ++i)
        {
            particles[i] = new Particle(log, this);
            if (!enableParticleLog) *log << " Particle " << i + 1 << "(#" << particles[i]
                                   << ") created" << endl;
        }
        areParticlesCreated = true;
    }
    catch (exception &e)
    {
        *log << e.what() << endl;
        return false;
    }
    return true;
}

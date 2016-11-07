#ifndef _SWARM_H_
#define _SWARM_H_

#include <string>
#include <fstream>

#include "particle.h"

using namespace std;

namespace PSO
{

class Particle;

class Swarm
{
public:
    Swarm();
    Swarm(ofstream *log);
    ~Swarm();
    int getN();                    // Getter for N
    double getMin(unsigned int i); // Getter for min
    double getMax(unsigned int i); // Getter for max
    double getPCoef();             // Getter for pCoef
    double getGCoef();             // Getter for gCoef
    double getVCoef();             // Getter for vCoef
    double getG(unsigned int i);   // Getter for g
    const double *getG();          // Getter for g
    void setG(unsigned int i, double val); // Setter for g
    bool getEnableParticleLog();        // Getter for enableParticleLog
    void start();                  // Start simulation
    double f(const double *x);           // Function to optimize

private:
    bool enableParticleLog { false };     // Enable/Disable detailed log for every particle
    bool isMemoryAlloc { false };    // Flag for alloc memory to g & min & max
    bool areParticlesAllocated { false }; // Flag for alloc memory to array of particles
    bool areParticlesCreated { false };   // Flag for alloc memory for every particle
    ofstream *log;       // Logging filestream
    int N;               // Dimestion
    double *min;         // Lower boundaries for function
    double *max;         // Upper boundaries for function
    int nIterations;     // Amount of iterations
    int nParticles;           // Amount of particles
    double pCoef;        // Particle's best position coefficient
    double gCoef;        // Particle's best position coefficient
    double vCoef;        // Velocity coefficient
    double *g;           // Best known position
    Particle **particles;          // Particles BZZZZZ!

    bool readConfig(const string filename = "config.yml"); // Reads parameters
    bool createParticles();   // Creates particles
};

}

#endif // _SWARM_H_

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <fstream>

#include "swarm.h"

using namespace std;

namespace PSO
{

class Swarm;

class Particle
{
public:
    Particle();                        // Default constructor
    Particle(ofstream *log, Swarm *s); // Constructor with parameters
    ~Particle();

    int update(); /* Makes one iteration (updates position with previous
                     velocity, velocity due to current position, best particle's
                     position) and returns:
                     0 then particle is OUTSIDE boundaries,
                     1 then particle is INSIDE boundaries,
                     2 then found NEW PARTICLE'S position and
                     3 then found NEW SWARM'S position*/

private:
    ofstream *log; // Logging filestream
    Swarm *swarm;  // Each particle should store pointer to its own swarm
    double *x;     // Vector with particle's current position
    double *p;     // Vector with particle's best known position
    double *v;     // Vector with particle's velocity vector
    bool isMemoryAlloc { false }; // Flag for allocating memory to vectors

    void init();    // Initiate particle, its position and velocity
    double dRand(double min, double max); // Generates double rand in [min; max]
/* NOTE: First update position and THEN velocity */
    void updateV(); // Updates velocity
    int updateX();  // Updates position (0 - outside boundaries, 1 - unside)
    void logInfo(); // Outputs particle's parameters into log-file
};

}

#endif // _PARTICLE_H_

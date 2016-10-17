#ifndef _BEE_H_
#define _BEE_H_

#include <fstream>

#include "swarm.h"

using namespace std;

namespace BSO
{

class Swarm;

class Bee
{
public:
    Bee();                        // Default constructor
    Bee(ofstream *log, Swarm *s); // Constructor with parameters
    ~Bee();

    int update(); /* Makes one iteration (updates position with previous
                     velocity, velocity due to current position, best bee's
                     position) and returns:
                     0 then bee is OUTSIDE boundaries,
                     1 then bee is INSIDE boundaries,
                     2 then found NEW BEE'S position and
                     3 then found NEW SWARM'S position*/

private:
    ofstream *log; // Logging filestream
    Swarm *swarm;  // Each bee should store pointer to its own swarm
    double *x;     // Vecotr with bee's current position
    double *p;     // Vector with bee's best known position
    double *v;     // Vector with bee's velocity vector
    bool isMemoryAlloc { false }; // Flag for allocating memory to vectors

    void init();    // Initiate bee, its position and velocity
    double dRand(double min, double max); // Generates double rand in [min; max]
/* NOTE: First update position and THEN velocity */
    void updateV(); // Updates velocity
    int updateX();  // Updates position (0 - outside boundaries, 1 - unside)
    void logInfo(); // Outputs bee's parameters into log-file
};

}

#endif // _BEE_H_

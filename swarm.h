#ifndef _SWARM_H_
#define _SWARM_H_

#include <string>
#include <fstream>

using namespace std;

class Swarm
{
public:
    Swarm();
    Swarm(ofstream *log);
    ~Swarm();
    int getN();                      // Getter for N
    int getMin(unsigned int i);      // Getter for min
    int getMax(unsigned int i);      // Getter for max
    void start();                    // Start simulation

private:
    ofstream *log;      // Logging filestream
    int N;              // Dimestion
    bool isMemoryAlloc; // Flag for allocating memory to min & max vectors
    double *min;        // Lower boundaries for function
    double *max;        // Upper boundaries for function
    int nIterations;    // Amount of iterations
    int nBees;          // Amount of bees
    double pCoef;       // Bee's best position coefficient
    double gCoef;       // Bee's best position coefficient
    double vCoef;       // Velocity coefficient

    bool readConfig(const string filename = "config.yml"); // Reads parameters
};

#endif // _SWARM_H_

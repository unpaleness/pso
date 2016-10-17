#ifndef _SWARM_H_
#define _SWARM_H_

#include <string>
#include <fstream>

#include "bee.h"

using namespace std;

namespace BSO
{

class Bee;

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
    bool getEnableBeeLog();        // Getter for enableBeeLog
    void start();                  // Start simulation
    double f(const double *x);           // Function to optimize

private:
    bool enableBeeLog { false };     // Enable/Disable detailed log for every bee
    bool isMemoryAlloc { false };    // Flag for alloc memory to g & min & max
    bool areBeesAllocated { false }; // Flag for alloc memory to array of bees
    bool areBeesCreated { false };   // Flag for alloc memory for every bee
    ofstream *log;       // Logging filestream
    int N;               // Dimestion
    double *min;         // Lower boundaries for function
    double *max;         // Upper boundaries for function
    int nIterations;     // Amount of iterations
    int nBees;           // Amount of bees
    double pCoef;        // Bee's best position coefficient
    double gCoef;        // Bee's best position coefficient
    double vCoef;        // Velocity coefficient
    double *g;           // Best known position
    Bee **bees;          // Bees BZZZZZ!

    bool readConfig(const string filename = "config.yml"); // Reads parameters
    bool createBees();   // Creates bees
};

}

#endif // _SWARM_H_

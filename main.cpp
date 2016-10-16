#include <fstream>

#include "swarm.h"

using namespace std;

int main()
{
    ofstream log("pso.log"); // Create logging filestream
    Swarm swarm(&log);       // Initiate swarm
    swarm.start();           // Start it
}
